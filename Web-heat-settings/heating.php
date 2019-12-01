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

function getUrlVars() {
    var vars = {};
    var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi,    
    function(m,key,value) {
      vars[key] = value;
    });
    return vars;
  }

var today = getUrlVars()["day"];
var building = getUrlVars()["building"];

//alert(today);
//alert(building);


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
var profileList;
var newSystem = 0; // Set to 0 for old Panstamp system; 1 for ESP-8266

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

	var res = { r: "" };
	if(NewFileToSave(res))
		SaveDay(res.r, today , true);
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
	var res = { r: "" };
	if(NewFileToSave(res))
		SaveDay(res.r, today , true);
}
	
function Header()
{
	document.write('<H1>West End Stables: ');
	if(building == "workshop")
		document.write('Workshop');
	else if(building == "aandc")
		document.write('Adrian and Christine\'s');
	else
		document.write('Sally and Ben\'s');
	document.write(' Heating Settings</H1><br>');

	document.write('<a href="index.html"><H2>Home</H2></a><br><br>');

	document.write('<table border="1" border-spacing="5px" >');
	document.write('  <tr>');
	document.write('   <td><a href="heating.php?building=' + building.toLowerCase() + '&day=Monday">Monday</a></td>');
	document.write('   <td><a href="heating.php?building=' + building.toLowerCase() + '&day=Tuesday">Tuesday</a></td>');
	document.write('   <td><a href="heating.php?building=' + building.toLowerCase() + '&day=Wednesday">Wednesday</a></td>');
 	document.write('   <td><a href="heating.php?building=' + building.toLowerCase() + '&day=Thursday">Thursday</a></td>');
 	document.write('   <td><a href="heating.php?building=' + building.toLowerCase() + '&day=Friday">Friday</a></td>');
 	document.write('   <td><a href="heating.php?building=' + building.toLowerCase() + '&day=Saturday">Saturday</a></td>');
 	document.write('   <td><a href="heating.php?building=' + building.toLowerCase() + '&day=Sunday">Sunday</a></td>');
 	document.write(' </tr>');
	document.write('</table> ');

   	document.write('<br><font size="6">' + today + '&#39;s Settings</font> (Today is ');
  	 var myDate = new Date();
   	document.write(["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"][myDate.getDay()]);
   	document.write('.)<br><br>');

   	document.write('<button style="background-color:Yellow" type="button" name="BoostAll"  onclick="AllBoost()">Boost all</button>');

	document.write('<button style="background-color:Yellow" type="button" name="OffAll" onclick="AllOff()" >Switch all off</button><br>');

	document.write('<form method="post" action="set" name="settings">');

	document.write('<table border="0" border-spacing="5px"><tbody>');
	document.write('<tr><td>Room</td><td>Actual</td><td>Set</td> </tr>');
	document.write('<tr class="break"><td colspan="' + (times+8) + '" bgcolor="#000000"></td></tr>');

}


function CheckSaveDay(thisDay)
{
	var res = { r: "" };
	if(NewFileToSave(res))
		SaveDay(res.r, thisDay , true);
}

function CheckSaveProfile()
{
	var res = { r: "" };
	if(NewFileToSave(res))
		SaveProfile(res.r, GetProfileName());
}

function Footer()
{
	document.write('</tbody></table>');
	document.write('<br><button style="background-color:white" type="button" name="Save" onclick="CheckSaveDay( \'' + today + '\')"><H2>Save</H2></button>');
	document.write('<button style="background-color:white" type="button" name="CopyWeek" onclick="WeekCopy()"><H2>Copy to weekdays</H2></button>');
	document.write('<button style="background-color:white" type="button" name="CopyWeekend" onclick="WeekendCopy()"><H2>Copy to weekends</H2></button>');
	document.write('<br><br><button style="background-color:white" type="button" name="SaveToFile" onclick="CheckSaveProfile()"><H2>Save To File:</H2></button>');
	document.write('&nbsp;&nbsp;<input size="30" style="height:60px;font-size:14pt;" text-align="right" name="profileFile" value = "enter filename here" onfocus="if (this.value==\'enter filename here\') this.value=\'\';">');

	ProfileLoadFiles();
	document.write('</form>');
        var logString1 = '<br><a href = "';
	var logString2 = '.php?building=' + building.charAt(0).toUpperCase() + building.substring(1) + '" target="_blank"><H2>'
	var logString3 = '</H2></a>';
	document.write(logString1 + 'printLog' + logString2 + 'Print the log' + logString3);
	document.write(logString1 + 'startLog' + logString2 + 'Start logging' + logString3);
	document.write(logString1 + 'stopLog' + logString2 + 'Stop logging and delete the log' + logString3);
}

function ProfileLoadFiles()
{

	document.write('<br><br><H2>Load from:</H2><br>');

	for(var i = 0; i < profileList.length; i++)
	{
		if(profileList[i] != "")
		  document.write('<button style="background-color:white" type="button" name="Load" onclick="LoadDay(\'' + profileList[i] + '\')"><H2>' + profileList[i] + '</H2></button>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;');
		if((i+1)%6 == 0)
			document.write('<br>');
	}
}

function GetProfileName()
{
   	var theForm = document.forms["settings"];
	//alert(theForm.elements["profileFile"].value);
	return theForm.elements["profileFile"].value;
}

function SecondsSinceMidnight(aTime)
{
	var h = aTime.substring(0, 2);
	var m = aTime.substring(3, 5);
	var s = aTime.substring(6, 8);
	//alert(aTime + ' ' + h + ' ' + m + ' ' + s);
	h = Number(h);
	m = Number(m);
	s = Number(s);
	var t = Number(h*3600 + m*60 + s);
	//alert(t + ' ' + h + ' ' + m + ' ' + s);
	if(isNaN(h) || isNaN(m) || isNaN(s) || h > 23 || m > 59 || s > 59 || h < 0 || m < 0 || s < 0)
	{
		return -2;
	} 
	return t;
}



function GetTableLine(curline, i, result)
{
	var thisRoom = curline.substring(0, DataStart(curline));
	result.r = thisRoom;
	thisRoom = thisRoom.substring(1);
	thisRoom = thisRoom.substring(0, thisRoom.indexOf("*"));
   	var theForm = document.forms["settings"];
	var rowLength = times+2;

	if(off[i])
		result.r += "0";
	else
		result.r += "1";
	result.r += " ";

	var boostTime = 1800;

	var thisDay = new Date();
	if (thisDay.getTimezoneOffset() != winterOffset) 
	{ 
		boostTime = 3600 + boostTime; 
	}

	if(boost[i])
		result.r += Math.floor(Date.now()/1000) + boostTime;
	else
		result.r += Math.floor(Date.now()/1000) - boostTime - 1000;

	var location = locations[i];
	var lastT = -1;
	for(var j = 0; j < rowLength; j++)
	{
		var timeCell = location + 'time' + j;
		var tempCell = location + 'temp' + j;
		var time = theForm.elements[timeCell].value;
                var temp = Number(theForm.elements[tempCell].value);
		if(isNaN(temp) || temp < -10 || temp > 50)
		{
			alert("There is an error in temperature " + (1+j) + " in room " + thisRoom + ". The settings will not be saved.");
			return false;			
		}
		if(time == '')
			 break;
                if(time.lastIndexOf(':') < 3)
                     time += ":00";
		var ssm = SecondsSinceMidnight(time);
		//alert(ssm + ' ' + lastT);
		if(ssm <= lastT)
		{
			alert("There is an error in time " + (1+j) + " in room " + thisRoom + ". The settings will not be saved.");
			return false;
		}
		lastT = ssm;
		result.r += " " + time + " " + temp;
	}
	return true;
}


function NewFileToSave(result)
{
   result.r = "";

   for(var i = 0; i < controlFileLines.length; i++) 
   {
        var curLine = controlFileLines[i];
		  result.r += curLine + '\n';
        if(curLine == '---')
			break;
	}

	var i = 0;
	
   var res = { r: "" };
   for(j = deviceCount+1; j <= deviceCount+rows; j++)
   {
        curLine = controlFileLines[j];
	if(!GetTableLine(curLine, i, res))
		return false;
	
	result.r += res.r + '\n';
	i++;
   }

   return true;
}


function SaveProfile(toSend, name)
{
   var xhr = new XMLHttpRequest();
   
   xhr.open('POST', 'post.php?building=' + building.toLowerCase() + '&file=' + name, true);
	xhr.setRequestHeader("Content-type","text/plain");
   xhr.onload = 
	function(e) 
  	{
    	if (this.status == 200) 
		{
			//if(lastShot)
				alert(xhr.responseText.split('<!--')[0]);
    	}
		xhr.abort();
  	};

   xhr.send(toSend);
}

function LoadDay(fileName)
{
   fileToCopy = "" + building.toLowerCase() + '-Profiles/' + fileName;
   fileToReplace = "" + building.toLowerCase() + '-profile-' + today.toLowerCase() + '.dat';
   var xhr = new XMLHttpRequest();
   
   xhr.open('POST', 'filecopy.php?from=' + fileToCopy + '&to=' + fileToReplace, true);
	xhr.setRequestHeader("Content-type","text/plain");
   xhr.onload = 
	function(e) 
  	{
    	if (this.status == 200) 
		{
			//if(lastShot)
				alert(xhr.responseText.split('<!--')[0]);
				location.reload(true);
    	}
		xhr.abort();
  	};

   xhr.send(" ");
}

function SaveDay(toSend, day, lastShot)
{
   var xhr = new XMLHttpRequest();
   
   xhr.open('POST', 'post.php?building=' + building.toLowerCase() + '&day=' + day.toLowerCase() + '&file=', true);
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
	var res = { r: "" };
	if(NewFileToSave(res))
	{
		SaveDay(res.r, 'Monday', false);
		SaveDay(res.r, 'Tuesday', false);
		SaveDay(res.r, 'Wednesday', false);
		SaveDay(res.r, 'Thursday', false);
		SaveDay(res.r, 'Friday', true);
	}
}


function WeekendCopy()
{
	var res = { r: "" };
	if(NewFileToSave(res))
	{
		SaveDay(res.r, 'Saturday', false);
		SaveDay(res.r, 'Sunday', true);
	}
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
	{
		var res = { r: "" };
		if(NewFileToSave(res))
			SaveDay(res.r, today , true);
	}
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
	{
		var res = { r: "" };
		if(NewFileToSave(res))
			SaveDay(res.r, today , true);
	}
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


	document.write('</tr><tr class="break"><td colspan="' + (rowLength+6) + '" bgcolor="#000000"></td></tr>');

	maxLocation++;
}

function GetFileAsStringArray(frameName, child)
{
    var oFrame = document.getElementById(frameName);
    //var rand = Math.floor((Math.random()*1000000)+1);
    //oFrame.contentWindow.location.href = oFrame.src + "?uid="+rand;
//alert(JSON.stringify(oFrame.src));
    //oFrame.contentWindow.location.reload();
    var strRawContents;
    if(child)
	strRawContents = oFrame.contentWindow.document.body.childNodes[0].innerHTML;
    else
	strRawContents = oFrame.contentWindow.document.body.innerHTML;
    while (strRawContents.indexOf("\r") >= 0)
        strRawContents = strRawContents.replace("\r", "");
    var arrLines = strRawContents.split("\n");
	 return arrLines;
}

function LoadProfileDirectory()
{
	//alert("In function");
	profileList = GetFileAsStringArray("profileFile", false)
	//alert(JSON.stringify(profileList));
}



function LoadTemperaturesOld() 
{
    var arrLines = GetFileAsStringArray("temperatureFile", true);
    for(var i = 0; i < arrLines.length; i++) 
    {
		if(arrLines[i].length > 2)
		{
			temperatureLocations[i] = RoomName(arrLines[i]);
			var ts = DataStart(arrLines[i]);
			//temperatures[i] = arrLines[i].substring(ts).match(/\d+/)[0];
			temperatures[i] = parseFloat(arrLines[i].substring(ts), 10);
         		//ts += temperatures[i].length+1;
			ts += arrLines[i].substring(ts).indexOf(' ') + 1;
         		//setTemperatures[i] = arrLines[i].substring(ts).match(/\d+/)[0];
			setTemperatures[i] = parseFloat(arrLines[i].substring(ts), 10);
			//ts += setTemperatures[i].length+1;
			ts += arrLines[i].substring(ts).indexOf(' ') + 1;
         		//heatOn[i] = arrLines[i].substring(ts).match(/\d+/)[0];
			heatOn[i] = parseInt(arrLines[i].substring(ts), 10);
			//alert(heatOn[i]);
		}
    }
}

/*
  var url1 = "" + building.toLowerCase() + "-temperatures.dat?timestamp=" + Date.now();
  document.write('<iframe id="temperatureFile" src="' + url1 + '" style="display: none;"></iframe>');
*/

function LoadTemperaturesNew() 
{
    var buildingDir = "../WiFiHeating/" + building.charAt(0).toUpperCase() + building.substr(1) + "/Data/";
    var i = 0;
    
    for(j = deviceCount+1; j <= deviceCount+rows; j++)
    {
        curLine = controlFileLines[j];
	temperatureLocations[i] = RoomName(curLine);
	var fileName = buildingDir + temperatureLocations[i].replace(/\s/g, '') + ".dat?timestamp=" + Date.now();
	alert("Loading temperatures for " + fileName);
	document.write('<iframe id="temperatureFile"' + String(i) + ' src="' + fileName + '" style="display: none;"></iframe>');
	//var lin = GetFileAsStringArray("temperatureFile" + String(i), true);
	//var lin = window.frames["temperatureFile" + String(i)].document.body.innerHTML;
	/*alert("Hello" + lin);
	alert(lin[0]);
	temperatures[i] = parseFloat(lin[0]);
	lin[0] = lin[0].substring(1+lin[0].indexOf(" "));
	setTemperatures[i] = parseFloat(lin[0]);
	lin[0] = lin[0].substring(1+lin[0].indexOf(" "));
	if(lin[0].indexOf("ON") >= 0)
		heatOn[i] = 1;
	else
		heatOn[i] = 0;
	i++;*/
    } 
/*
    var arrLines = GetFileAsStringArray("temperatureFile", true);
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
*/
}


function LoadTemperatures()
{
	if(newSystem)
		LoadTemperaturesNew();
	else
		LoadTemperaturesOld();
}


function LoadFile() 
{
    controlFileLines = GetFileAsStringArray("controlFile", true);
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



}


function AllLoaded()
{
	LoadFile();
	LoadTemperatures();
	LoadProfileDirectory();

    Header();

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

/*
var url1 = "" + building.toLowerCase() + "-temperatures.dat?timestamp=" + Date.now();
document.write('<iframe id="temperatureFile" src="' + url1 + '" onload="LoadTemperatures();" style="display: none;"></iframe>');

var url3 = "dirlist.php?dir=" + building.toLowerCase() + "-Profiles";
document.write('<iframe id="profileFile" src="' + url3 + '" onload="LoadProfileDirectory();" style="display: none;"></iframe>');

var url2 = "" + building.toLowerCase() + "-profile-" + today.toLowerCase() + ".dat?timestamp=" + Date.now();
document.write('<iframe id="controlFile" src="' + url2 + '" onload="LoadFile();" style="display: none;"></iframe>');

*/

if(!newSystem)
{
  var url1 = "" + building.toLowerCase() + "-temperatures.dat?timestamp=" + Date.now();
  document.write('<iframe id="temperatureFile" src="' + url1 + '" style="display: none;"></iframe>');
}

var url3 = "dirlist.php?dir=" + building.toLowerCase() + "-Profiles";
document.write('<iframe id="profileFile" src="' + url3 + '" style="display: none;"></iframe>');

var url2 = "" + building.toLowerCase() + "-profile-" + today.toLowerCase() + ".dat?timestamp=" + Date.now();
document.write('<iframe id="controlFile" src="' + url2 + '" style="display: none;"></iframe>');

</script>

&nbsp;&nbsp;<br>

<script>
window.onload = function()
{
	AllLoaded();
}
</script>


</body>
</html>


