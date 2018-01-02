<html>
<head><title>controllednode</title></head>
<body><?php
ini_set('display_errors', 'On');
error_reporting(E_ALL | E_STRICT);
$debug = True;
/*

$fileRoot = "Data/";
$fileName = $fileRoot . $location . ".dat";
$fileContents = file_get_contents($fileName);
//echo "file: " . $fileContents . "\n";

$fileHandle = fopen($fileName, "w");
fwrite($fileHandle, $temperature);
fclose($fileHandle); 

*/



function SetTemperature($device) 
{
    return -300.0;
}

function TurnOnDependentList($device)
{

}

function IAmAnOnSlave($device)
{
    return True;
}

function SaveTemperature($device, $temp)
{
   $fileName = $fileRoot . $device . ".dat";
   $fileHandle = fopen($fileName, "w");
   fwrite($fileHandle, $temp);
   fclose($fileHandle); 
}

$location = '';
$temperature = '20';


parse_str($_SERVER['QUERY_STRING']);

if($location == '')
	exit('ERROR');


$t = 0.0 + $temperature;
$set = SetTemperature($location);


if($set < -280.0)
{
  if(IAmAnOnSlave($location))
  {
     echo 'ON';
  } else
  {
     echo 'OFF';
  }
} else
{
  if($set > $t)
  {
    TurnOnDependentList($location);
    echo 'ON';
  } else
  {
    echo 'OFF';
  }
  SaveTemperature($location);
}

if($debug)
{
  echo ', ' . $location . ' temp = ' . $t . ', set = ' . $set;
}

?></body>
</html>
