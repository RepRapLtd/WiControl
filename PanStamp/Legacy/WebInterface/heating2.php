<!DOCTYPE HTML>
<html>

<head>
<style>
  body {background-color:lightgrey}
  h2   {color:green}
  p    {color:grey}
  input{text-align:right; }
</style>

</head>

<body>


<iframe id="frmFile" src="../workshop-profile.dat" onload="LoadFile();" style="display: none;"></iframe>


<script type="text/javascript">

function Header()
{
	document.write('<H2>West End Stables: Workshop Heating Settings</H2><br><br><br>');
	document.write('<form method="post" action="set" name="settings">');
	//document.write('<table style="text-align: left; width: 684px; height: 62px;" border="1" cellpadding="2" cellspacing="2"><tbody>');
	document.write('<table style="text-align: left;" border="1" cellpadding="2" cellspacing="2"><tbody>');
}

function Footer()
{
	document.write("</tbody></table></form>");
}

function timeStart(tableLine)
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

function TableLine(tableLine, rowLength)
{
   document.write('<tr align="right">');
   var location = RoomName(tableLine);
	document.write('<td rowspan="2">' + location + '</td>');
	document.write('<td rowspan="2"> Boost </td>');

	var ts = timeStart(tableLine);
   var tstep = ts;
	var tc = timeCount(tableLine);
   var temps = [];

	for(var i = 0; i < tc; i++)
	{
		document.write('<td><input type="time" name="' + location + 'time' + i +'" value = "' + tableLine.substring(tstep).match(/..:..:../) + '" ></td>');
		tstep += 8;
		temps[i] = i;//tableLine.substring(tstep).match(/\d+$/)[0];
	}

	for(var i = tc; i < rowLength; i++)
		document.write('<td><input type="time" name="' + location + 'time' + i +'" value = "--:--" ></td>');

	document.write('</tr><tr>');

	for(var i = 0; i < tc; i++)
	{
		document.write('<td><input size="3" text-align="right" name="' + location + 'temp' + i +'" value = "' + temps[i] + '" > <sup>o</sup>C</td>');
	}

	for(var i = tc; i < rowLength; i++)
		document.write('<td><input size="3" text-align="right" name="' + location + 'temp' + i +'" value = "0" ><sup>o</sup>C</td>');


	document.write('</tr>');
}


function LoadFile() 
{
    var oFrame = document.getElementById("frmFile");
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
}
</script>
 

</body>
</html>

