
<html>
<head><title>startLog</title></head>
<body><?php

/*
 * Program to touch the log file
 */


// HTTP requests to this look like:  http://currentServer/pageRoot/printLog.php?building=house
// Where house is the name of the building.

// Note to self: don't forget "\n" means newline; '\n' means \n...


parse_str($_SERVER['QUERY_STRING']);

if(!strlen($building))
	exit('Error: no building specified for log.');

$logFileName = dirname( dirname(__FILE__) ) . "/WiFiHeating/". $building . "/Data/Log.dat";

if(file_exists($logFileName))
{
	echo "<br>Logging is already turned on.<br>";
} else
{
	touch($logFileName);
	echo "Logging started.";
}


?></body>
</html>





