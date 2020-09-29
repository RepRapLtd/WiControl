
application/x-httpd-php printLog.php
HTML document text
<html>
<head><title>printLog</title></head>
<body><?php

/*
 * Program to print out the log file
 */

// HTTP requests to this look like:  http://currentServer/pageRoot/printLog.php?building=house
// Where house is the name of the building.

// Note to self: don't forget "\n" means newline; '\n' means \n...


parse_str($_SERVER['QUERY_STRING'], $data);

//print_r($data);

if(!strlen($data[building]))
	exit('Error: no building specified for log.');

$logFileName = dirname( dirname(__FILE__) ) . "/WiFiHeating/". $data[building] . "/Data/Log.dat";

//echo($logFileName);

if(!file_exists($logFileName))
{
	echo "<br>Logging is not turned on.<br>";
} else
{
	$logfile = file_get_contents($logFileName);
        $logfile = nl2br($logfile); 
	echo $logfile;
}


?></body>
</html>




