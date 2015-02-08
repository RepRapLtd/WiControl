<!DOCTYPE HTML>
<html>

<head>
<style type="text/css">

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

</head>

<body>

<iframe id="temperatureFile" src="../workshop-temperatures.dat" onload="LoadTemperatures();" style="display: none;"></iframe>

<iframe id="controlFile" src="../workshop-profile-monday.dat" onload="LoadFile();" style="display: none;"></iframe>


<script type="text/javascript">

var locations = [];
var temperatures = [];
var temperatureLocations = [];
var boost = [];
var boostStamp = [];
var off = [];
var maxLocation = 0;
var boostAll = 0;
var offAll = 0;

function AllBoost()
{
	boostAll = 1 - boostAll;
	for(var i = 0; i < maxLocation; i++)
		BoostButton(locations[i], boostAll);
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
}

function AllOff()
{
	offAll = 1 - offAll;
	for(var i = 0; i < maxLocation; i++)
		OffButton(locations[i], offAll);
   var btn = document.getElementsByName("OffAll")[0];
   if(offAll == 1)
	{
		btn.style.backgroundColor = 'Aqua';
		btn.textContent="All on";
	} else
	{
		btn.style.backgroundColor = 'Yellow';
		btn.textContent="All off";
	}
}
	
function Header()
{
	document.write('<H2>West End Stables: Workshop Heating Settings</H2><br><br>');

   document.write('<ul>');
   document.write('<li><a href="workshop-heating-monday.php"><H3>Monday</H3></a></li>');
	document.write('<li><a href="workshop-heating-tuesday.php">Tuesday</a></li>');
	document.write('<li><a href="workshop-heating-wednesday.php">Wednesday</a></li>');
	document.write('<li><a href="workshop-heating-thursday.php">Thursday</a></li>');
	document.write('<li><a href="workshop-heating-friday.php">Friday</a></li>');
	document.write('<li><a href="workshop-heating-Saturday.php">Saturday</a></li>');
	document.write('<li><a href="workshop-heating-Sunday.php">Sunday</a></li>');
	document.write('</ul><br>');

   document.write('<button style="background-color:Yellow" type="button" name="BoostAll"  onclick="AllBoost()">Boost all</button>');

document.write('<button style="background-color:Yellow" type="button" name="OffAll" onclick="AllOff()" >All off</button>');

	document.write('<form method="post" action="set" name="settings">');

	document.write('<table style="text-align: left;" border="1" cellpadding="2" cellspacing="2"><tbody>');

}

function Footer()
{
	document.write('</tbody></table>');
	document.write('<br><button style="background-color:white" type="button" name="Save" onclick="SaveDay()"><H2>Save</H2></button>');
	document.write('<button style="background-color:white" type="button" name="CopyWeek" onclick="WeekCopy()"><H2>Copy to weekdays</H2></button>');
	document.write('<button style="background-color:white" type="button" name="CopyWeekend" onclick="WeekendCopy()"><H2>Copy to weekends</H2></button>');
	document.write('</form>');
}

function SaveDay()
{
	alert("SaveDay");
}

function WeekCopy()
{
	alert("WeekCopy");

}


function WeekendCopy()
{
	alert("WeekendCopy");
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

function BoostButton(location, cmd)
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
}

function OffButton(location, cmd)
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
}

function SetButtons()
{
	for(var i = 0; i < maxLocation; i++)
	{
		if(off[i])
			OffButton(locations[i], 1);
		else
			OffButton(locations[i], 0);
		if(boost[i])
			BoostButton(locations[i], 1);
		else
			BoostButton(locations[i], 0);
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
	document.write('<td rowspan="2">' + temperatures[maxLocation] + '<sup>o</sup>C</td>');

	document.write('<td align="center"><button style="background-color:Yellow" type="button" name="' + location + 'Boost" onclick="BoostButton(\'' + location + '\', -1)">Boost</button>  </td>');

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

	document.write('<td align="center"><button style="background-color:yellow" type="button" name="' + location + 'Off" onclick="OffButton(\'' + location + '\', -1)">Off</button>  </td>');

	for(var i = 0; i < tc; i++)
	{
		document.write('<td><input size="3" text-align="right" name="' + location + 'temp' + i +'" value = "' + temps[i] + '" > <sup>o</sup>C</td>');
	}

	for(var i = tc; i < rowLength; i++)
		document.write('<td><input size="3" text-align="right" name="' + location + 'temp' + i +'" value = "0" ><sup>o</sup>C</td>');


	document.write('</tr>');

	maxLocation++;
}

function LoadTemperatures() 
{
    var oFrame = document.getElementById("temperatureFile");
    var strRawContents = oFrame.contentWindow.document.body.childNodes[0].innerHTML;
    while (strRawContents.indexOf("\r") >= 0)
        strRawContents = strRawContents.replace("\r", "");
    var arrLines = strRawContents.split("\n");
    for(var i = 0; i < arrLines.length; i++) 
    {
		temperatureLocations[i] = RoomName(arrLines[i]);
		var ts = DataStart(arrLines[i]);
		temperatures[i] = arrLines[i].substring(ts).match(/\d+/)[0];
    }
}


function LoadFile() 
{
    var oFrame = document.getElementById("controlFile");
    var strRawContents = oFrame.contentWindow.document.body.childNodes[0].innerHTML;
    Header();
    while (strRawContents.indexOf("\r") >= 0)
        strRawContents = strRawContents.replace("\r", "");
    var arrLines = strRawContents.split("\n");
    //alert("File " + oFrame.src + " has " + arrLines.length + " lines");
    var deviceCount = -1;
	 var rows = -1;
    var times = -1;
    for(var i = 0; i < arrLines.length; i++) 
    {
        var curLine = arrLines[i];
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
        curLine = arrLines[j];
		  TableLine(curLine, times+2);
    }
	 Footer();

    //alert(Math.floor(Date.now() / 1000));

    SetButtons();
}
</script>
 

</body>
</html>

