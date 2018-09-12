<!DOCTYPE HTML>
<html>

<head>
<style type="text/css">

table, th, td {
    border: 1px solid black;
    padding: 5px;
}
table {
    border-spacing: 15px;
}
td {
    text-align: center;
}

  body {background-color:lightgrey}
  h2   {color:green}
  p    {color:grey}
  input{text-align:right; }
  ul {
    list-style-type: none;
    margin: 0;
    padding: 0;
    overflow: hidden;
  }

  li {
    float: left;
  }

  a:link, a:visited {
    display: block;
    width: 120px;
    font-weight: bold;
    color: #FFFFFF;
    background-color: #98bf21;
    text-align: center;
    padding: 4px;
    text-decoration: none;
    text-transform: uppercase;
  }

  a:hover, a:active {
    background-color: #7A991A;
  }



</style>

<script type="text/javascript">

// Set these two for all scripts
// building is one of 'aandc', 'sandb', or 'Workshop'

var today = 'Sunday';
var building = 'Workshop';

//------------------------------------------

var locations = [];
var temperatures = [];
var setTemperatures = [];
var heatOn = [];
var temperatureLocations = [];
var boost = [];
var boostStamp = [];
var off = [];
var maxLocation = 0;
var boostAll = 0;
var offAll = 0;
var controlFileLines;
var deviceCount = -1;
var rows = -1;
var times = -1;

var juneSolstice = new Date();
juneSolstice.setMonth(5); // zero-based
juneSolstice.setDate(21);

var decemberSolstice = new Date();
decemberSolstice.setMonth(11); // zero-based
decemberSolstice.setDate(21);


var winterOffset = Math.max(juneSolstice.getTimezoneOffset(), decemberSolstice.getTimezoneOffset());
var summerOffset = Math.min(juneSolstice.getTimezoneOffset(), decemberSolstice.getTimezoneOffset());


function AllBoost()
{
	boostAll = 1 - boostAll;
	for(var i = 0; i < maxLocation; i++)
		BoostButton(locations[i], boostAll, false);
   var btn = document.getElementsByName("BoostAll")[0];
   if(boostAll == 1)
	{
		btn.style.backgroundColor = 'IndianRed';
      btn.textContent="Boost none";
	} else
	{
		btn.style.backgroundColor = 'Yellow';
		btn.textContent="Boost all";
	}
	SaveDay(NewFileToSave(), today , true);
}

function AllOff()
{
	offAll = 1 - offAll;
	for(var i = 0; i < maxLocation; i++)
		OffButton(locations[i], offAll, false);
   var btn = document.getElementsByName("OffAll")[0];
   if(offAll == 1)
	{
		btn.style.backgroundColor = 'Aqua';
		btn.textContent="Switch all on";
	} else
	{
		btn.style.backgroundColor = 'Yellow';
		btn.textContent="Switch all off";
	}
	SaveDay(NewFileToSave(), today , true);
}
	
function Header()
{
	document.write('<H1>West End Stables: ');
	if(building == "Workshop")
		document.write('Workshop');
	else if(building == "aandc")
		document.write('Adrian and Christine\'s');
	else
		document.write('Sally and Ben\'s');
	document.write(' Heating Settings</H1><br>');

	document.write('<a href="../index.html"><H2>Home</H2></a><br><br>');

	document.write('<table border="1" border-spacing="5px" >');
	document.write('  <tr>');
	document.write('    <td><a href="' + building.toLowerCase() + '-heating-monday.php">Monday</a></td>');
	document.write('    <td><a href="' + building.toLowerCase() + '-heating-tuesday.php">Tuesday</a></td>');
	document.write('    <td><a href="' + building.toLowerCase() + '-heating-wednesday.php">Wednesday</a></td>');
 	document.write('   <td><a href="' + building.toLowerCase() + '-heating-thursday.php">Thursday</a></td>');
 	document.write('   <td><a href="' + building.toLowerCase() + '-heating-friday.php">Friday</a></td>');
 	document.write('   <td><a href="' + building.toLowerCase() + '-heating-saturday.php">Saturday</a></td>');
 	document.write('   <td><a href="' + building.toLowerCase() + '-heating-sunday.php">Sunday</a></td>');
 	document.write(' </tr>');
	document.write('</table> ');

   	document.write('<br><font size="6">' + today + '&#39;s Settings</font> (Today is ');
  	 var myDate = new Date();
   	document.write(["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"][myDate.getDay()]);
   	document.write('.)<br><br>');

   	document.write('<button style="background-color:Yellow" type="button" name="BoostAll"  onclick="AllBoost()">Boost all</button>');

	document.write('<button style="background-color:Yellow" type="button" name="OffAll" onclick="AllOff()" >Switch all off</button><br>');

	document.write('<form method="post" action="set" name="settings">');

	document.write('<table border="1" border-spacing="5px"><tbody>');
	document.write('<tr><td>Room</td><td>Actual</td><td>Set</td> </tr>');

}

function Footer()
{
	document.write('</tbody></table>');
	document.write('<br><button style="background-color:white" type="button" name="Save" onclick="SaveDay(NewFileToSave(), \'' + today + '\', true)"><H2>Save</H2></button>');
	document.write('<button style="background-color:white" type="button" name="CopyWeek" onclick="WeekCopy()"><H2>Copy to weekdays</H2></button>');
	document.write('<button style="background-color:white" type="button" name="CopyWeekend" onclick="WeekendCopy()"><H2>Copy to weekends</H2></button>');
	document.write('</form>');
}

function GetTableLine(curline, i)
{
	var result = curline.substring(0, DataStart(curline));
   var theForm = document.forms["settings"];
	var rowLength = times+2;

	if(off[i])
		result += "0";
	else
		result += "1";
	result += " ";

	var boostTime = 3600;

	var thisDay = new Date();
	if (thisDay.getTimezoneOffset() != winterOffset) 
	{ 
		boostTime = 2*boostTime; 
	}

	if(boost[i])
		result += Math.floor(Date.now()/1000) + boostTime;
	else
		result += Math.floor(Date.now()/1000) - 4000;

	var location = locations[i];
	for(var j = 0; j < rowLength; j++)
	{
		var timeCell = location + 'time' + j;
		var tempCell = location + 'temp' + j;
		var time = theForm.elements[timeCell].value;
                var temp = theForm.elements[tempCell].value;
		if(time == '')
			 break;
                if(time.lastIndexOf(':') < 3)
                     time += ":00";
		result += " " + time + " " + temp;
	}
	return result;
}

function NewFileToSave()
{
	var result = "";

   for(var i = 0; i < controlFileLines.length; i++) 
   {
        var curLine = controlFileLines[i];
		  result += curLine + '\n';
        if(curLine == '---')
			break;
	}

	var i = 0;
	
   for(j = deviceCount+1; j <= deviceCount+rows; j++)
   {
        curLine = controlFileLines[j];
		  result += GetTableLine(curLine, i) + '\n';
		  i++;
   }

	return result;
}


function SaveDay(toSend, day, lastShot)
{
   var xhr = new XMLHttpRequest();
   
   xhr.open('POST', 'post' + day + '.php', true);
	xhr.setRequestHeader("Content-type","text/plain");
   xhr.onload = 
	function(e) 
  	{
    	if (this.status == 200) 
		{
			if(lastShot)
				alert(xhr.responseText.split('<!--')[0]);
    	}
		xhr.abort();
  	};

   xhr.send(toSend);
}

function WeekCopy()
{
	var toSend = NewFileToSave();
	SaveDay(toSend, 'Monday', false);
	SaveDay(toSend, 'Tuesday', false);
	SaveDay(toSend, 'Wednesday', false);
	SaveDay(toSend, 'Thursday', false);
	SaveDay(toSend, 'Friday', true);
}


function WeekendCopy()
{
	var toSend = NewFileToSave();
	SaveDay(toSend, 'Saturday', false);
	SaveDay(toSend, 'Sunday', true);
}

function DataStart(tableLine)
{
	var start = tableLine.lastIndexOf('*');
   var i = start + 1;
   while(i < tableLine.length && (tableLine.charAt(i) ==' ' || tableLine.charAt(i) =='\t'))
   	i++;
   return i;
}

function RoomName(tableLine)
{
	var name = "";
	var start = tableLine.indexOf('*');
   var i = start + 1;
   while(i < tableLine.length && tableLine.charAt(i) != '*')
	{
		name += tableLine.charAt(i);
		i++;
	}
	return name;
}

function timeCount(tableLine)
{
	return (tableLine.match(/..:..:../g)).length;
}

function LocationIndex(location)
{
	for(var i = 0; i < maxLocation; i++)
   {
		if(locations[i] == location)
			return i;
   }

	alert(location + " not found");
   return 0;
}

function BoostButton(location, cmd, save)
{
   var btn = document.getElementsByName(location + "Boost")[0];
   var ind = LocationIndex(location);

	if(cmd == 0)
		boost[ind] = true;
	else if(cmd == 1)
		boost[ind] = false;   

   if(boost[ind])
   {
		btn.style.backgroundColor = 'Yellow';
      boost[ind] = false;
	} else
   {
 		btn.style.backgroundColor = 'IndianRed';
      boost[ind] = true;
   }
	if(save)
	SaveDay(NewFileToSave(), today , true);
}

function OffButton(location, cmd, save)
{
   var btn = document.getElementsByName(location + "Off")[0];
   var ind = LocationIndex(location);

	if(cmd == 0)
		off[ind] = true;
	else if(cmd == 1)
		off[ind] = false;  

   if(off[ind])
   {
		btn.style.backgroundColor = 'Yellow';
      off[ind] = false;
	} else
   {
 		btn.style.backgroundColor = 'Aqua';
      off[ind] = true;
   }
	if(save)
	SaveDay(NewFileToSave(), today , true);
}

function SetButtons()
{
	for(var i = 0; i < maxLocation; i++)
	{
		if(off[i])
			OffButton(locations[i], 1, false);
		else
			OffButton(locations[i], 0, false);
		if(boost[i])
			BoostButton(locations[i], 1, false);
		else
			BoostButton(locations[i], 0, false);
	}
}

function TableLine(tableLine, rowLength)
{
   document.write('<tr align="right">');
   var location = RoomName(tableLine);

   locations[maxLocation] = location;
	if(location != temperatureLocations[maxLocation])
		alert("Temperature file mismatch: " + location + " != " + temperatureLocations[maxLocation]);

	document.write('<td rowspan="2">' + location + '</td>');

   if(heatOn[maxLocation] == 1)
   {
		document.write('<td rowspan="2" style="background-color:IndianRed">');
		document.write(temperatures[maxLocation] + '<sup>o</sup>C</td>');
   }else
   {
		document.write('<td rowspan="2" style="background-color:Aqua">');
		document.write(temperatures[maxLocation] + '<sup>o</sup>C</td>');
   }

   document.write('<td rowspan="2" style="background-color:white">');
   document.write(setTemperatures[maxLocation] + '<sup>o</sup>C</td>');

	document.write('<td align="center"><button style="background-color:Yellow" type="button" name="' + location + 'Boost" onclick="BoostButton(\'' + location + '\', -1, true)">Boost</button>  </td>');

	var ts = DataStart(tableLine);
   var tstep = ts;

   if(tableLine.substring(tstep).match(/\d+/)[0] == "0")
		off[maxLocation] = true;
	else
		off[maxLocation] = false;		
   tstep += 2;

   boostStamp[maxLocation] = tableLine.substring(tstep).match(/\d+/)[0];
   tstep += boostStamp[maxLocation].length + 1;
	if(boostStamp[maxLocation] < Math.floor(Date.now()/1000))
		boost[maxLocation] = false;
	else
		boost[maxLocation] = true;

	var tc = timeCount(tableLine);
   var temps = [];

	for(var i = 0; i < tc; i++)
	{
		document.write('<td><input type="time" name="' + location + 'time' + i +'" value = "' + tableLine.substring(tstep).match(/..:..:../) + '" ></td>');
		tstep += 8;
		temps[i] = tableLine.substring(tstep).match(/\d+/)[0];
	   tstep += temps[i].length + 2;
	}

	for(var i = tc; i < rowLength; i++)
		document.write('<td><input type="time" name="' + location + 'time' + i +'" value = "--:--" ></td>');

	document.write('</tr><tr>');

	document.write('<td align="center"><button style="background-color:yellow" type="button" name="' + location + 'Off" onclick="OffButton(\'' + location + '\', -1, true)">Off</button>  </td>');

	for(var i = 0; i < tc; i++)
	{
		document.write('<td><input size="3" text-align="right" name="' + location + 'temp' + i +'" value = "' + temps[i] + '" > <sup>o</sup>C</td>');
	}

	for(var i = tc; i < rowLength; i++)
		document.write('<td><input size="3" text-align="right" name="' + location + 'temp' + i +'" value = "0" ><sup>o</sup>C</td>');


	document.write('</tr>');

	maxLocation++;
}

function GetFileAsStringArray(frameName)
{
    var oFrame = document.getElementById(frameName);
    var strRawContents = oFrame.contentWindow.document.body.childNodes[0].innerHTML;
    while (strRawContents.indexOf("\r") >= 0)
        strRawContents = strRawContents.replace("\r", "");
    var arrLines = strRawContents.split("\n");
	 return arrLines;
}

function LoadTemperatures() 
{
    var arrLines = GetFileAsStringArray("temperatureFile");
    for(var i = 0; i < arrLines.length; i++) 
    {
		if(arrLines[i].length > 2)
		{
			temperatureLocations[i] = RoomName(arrLines[i]);
			var ts = DataStart(arrLines[i]);
			temperatures[i] = arrLines[i].substring(ts).match(/\d+/)[0];
         ts += temperatures[i].length+1;
         setTemperatures[i] = arrLines[i].substring(ts).match(/\d+/)[0];
			ts += setTemperatures[i].length+1;
         heatOn[i] = arrLines[i].substring(ts).match(/\d+/)[0];
		}
    }
}


function LoadFile() 
{
    controlFileLines = GetFileAsStringArray("controlFile");
    
    Header();


    for(var i = 0; i < controlFileLines.length; i++) 
    {
        var curLine = controlFileLines[i];
        if(curLine == '---')
        {
				deviceCount = i;
        } else if(curLine == '')
        {
		  } else if(deviceCount >= 0)
        {
				rows = i - deviceCount;
            var t = timeCount(curLine);
            //alert("timecount: " + t);
				if(t > times)
					times = t
		  } else
        {
        }
    }
    //alert("Devices: " + deviceCount + ", Rows: " + rows + ", Times: " + times);
    for(j = deviceCount+1; j <= deviceCount+rows; j++)
    {
        curLine = controlFileLines[j];
		  TableLine(curLine, times+2);
    }
	 Footer();

    //alert(Math.floor(Date.now() / 1000));

    SetButtons();
}



</script>



</head>

<body>

<script>

var url1 = "../../" + building.toLowerCase() + "-temperatures.dat?timestamp=" + Date.now();
document.write('<iframe id="temperatureFile" src="' + url1 + '" onload="LoadTemperatures();" style="display: none;"></iframe>');

var url2 = "../../" + building.toLowerCase() + "-profile-" + today.toLowerCase() + ".dat?timestamp=" + Date.now();
document.write('<iframe id="controlFile" src="' + url2 + '" onload="LoadFile();" style="display: none;"></iframe>');


</script>




</body>
</html>

