
<html>
<head><title>stopLog</title></head>
<body><?php

/*
 * Program to delete the log file, which stops logging
 */


// HTTP requests to this look like:  http://currentServer/pageRoot/stopLog.php?building=house
// Where house is the name of the building.

// Note to self: don't forget "\n" means newline; '\n' means \n...


parse_str($_SERVER['QUERY_STRING']);

if(!strlen($building))
	exit('Error: no building specified for log.');

$logFileName = dirname( dirname(__FILE__) ) . "/WiFiHeating/". $building . "/Data/Log.dat";

if(!file_exists($logFileName))
{
	echo "<br>Logging is already turned off.<br>";
} else
{
	unlink($logFileName);
	echo "Logging stopped and the log file has been deleted.<br>";
}


?></body>
</html>


