<html>
<head><title>controllednode</title></head>
<body><?php

// Debugging...

ini_set('display_errors', 'On');
error_reporting(E_ALL | E_STRICT);
$debug = True;

// Append debugging info to this string

$debugString = '';

// Where the files are.

$fileRoot = "Data/";
$profileRoot = "aandc-profile-";

// Now

$unixTime = 0 + date_timestamp_get(date_create());


// ***********************************************************************

// This converts $timeOfDay in the time format in heating profile files ("hh:mm:ss")
// to the number of seconds since midnight.

function HMSToSeconds($timeOfDay)
{
   global $profile, $debug, $debugString, $fileRoot, $mySlaves, $iAmOn, $boostTime, $times, $temps, $profileRoot, $unixTime;

   $parsed = date_parse($timeOfDay);
   $seconds = $parsed['hour'] * 3600 + $parsed['minute'] * 60 + $parsed['second'];
   return 0 + $seconds;
}

// This parses the profile file which is written by the web interface.

// If $device is a master:
// The result is an array of dependent slaves for $device, $mySlaves, the boolean $iAmOn set,
// The Unix time $boostTime set, and the day's list of pairs of times and temperatures, $times and $temps.

// If $device is a slave:
// $times is empty().

function ParseProfile($device)
{

   global $profile, $debug, $debugString, $fileRoot, $mySlaves, $iAmOn, $boostTime, $times, $temps, $profileRoot, $unixTime;

   $profileName = $fileRoot . $profileRoot . strtolower(date('l')) . ".dat";
   $profile = file_get_contents($profileName);

/*   $iAmOn = True;
   $boostTime = $unixTime - 20000;
   $times = array(
       HMSToSeconds("00:00:00"),
       HMSToSeconds("07:30:00"),
       HMSToSeconds("09:00:00"),
       HMSToSeconds("18:00:00"),
       HMSToSeconds("19:30:00"),

	);
   $temps = array(
	15.0,
	19.0,
	17.0,
	21.0,
	18.0,


        );

   $mySlaves = array(
        "Boiler",
        "HallWest",
        "HallEast",

        );*/
}

// Return the set temperature for $device at the current time.

function SetTemperature($device) 
{
    global $profile, $debug, $debugString, $fileRoot, $mySlaves, $iAmOn, $boostTime, $times, $temps, $profileRoot, $unixTime;

    if(empty($profile))
	exit('ERROR: SetTemperature() - profile not loaded');

    // Am I a slave device?

    if(empty($times))
	return -300.0; // Yes


    // No

    if(!$iAmOn)
        return 10.0;

    $secondsSinceMidnight = (time() % 86400);

    for ($i = 1; $i < sizeof($times); $i++) 
    {
    	if($times[$i] >= $secondsSinceMidnight)
        {
	   return $temps[$i-1];
	}
    }

    return $temps[sizeof($temps)-1];
}

// If $device is a slave [see TurnOnDependentList() below] and it needs to be on,
// return true.  A slave device needs to be on if it's .dat file has a
// timestamp less than two minutes old.  As every device gets information
// on what it should be doing once a minute +/- 5 seconds, every slave
// device that needs to be on will have had it's .dat file touched less than
// 2 minutes (120 seconds) ago.

function IAmAnOnSlave($device)
{
    global $profile, $debug, $debugString, $fileRoot, $mySlaves, $iAmOn, $boostTime, $times, $temps, $profileRoot, $unixTime;

    if(empty($profile))
	exit('ERROR: IAmAnOnSlave() - profile not loaded');

    $fileName = $fileRoot . $device . ".dat";
    $fileTouched = 0 + filemtime($fileName);
    return ($unixTime - $fileTouched < 120);
}

// Touch the .dat files of slave devices that also need to be turned on if 
// master $device is on.  

function TurnOnDependentList($device)
{
    global $profile, $debug, $debugString, $fileRoot, $mySlaves, $iAmOn, $boostTime, $times, $temps, $profileRoot, $unixTime;

    if(empty($profile))
	exit('ERROR: TurnOnDependentList() - profile not loaded');

    if(empty($mySlaves))
	return;

    for ($i = 0; $i < sizeof($mySlaves); $i++)
    {
	$fileName = $fileRoot . $mySlaves[$i] . ".dat";
	touch($fileName);
    }
}

// Save the status of a master device in its .dat file.

function SaveTemperature($device, $act, $temp, $s)
{
    global $profile, $debug, $debugString, $fileRoot, $mySlaves, $iAmOn, $boostTime, $times, $temps, $profileRoot, $unixTime;

    $fileName = $fileRoot . $device . ".dat";
    $fileHandle = fopen($fileName, "w");
    fwrite($fileHandle, $temp . ' ' . $s . ' ' . $act);
    fclose($fileHandle);
    if($debug)
	$debugString = $debugString . ', ' . $device . ' temp = ' . $temp . ', set = ' . $s;
}

//***************************************************************

// Gather data...

$location = '';
$temperature = '20';

// Get the query sring from the HTTP request.
// This should be: "html://...../controllednode.php?location=where/what-the-device-is&temperature=the-device's-temperature"
// If the device does not have a temperature it should send -300.0 (impossible as below abs zero)

parse_str($_SERVER['QUERY_STRING']);

// Check we have a $device/$location at least...

if($location == '')
	exit('ERROR - $location undefined');

ParseProfile($location);

$t = 0.0 + $temperature;
$set = SetTemperature($location);



// Control the heating.  Default is off...

$action = 'OFF';

if($set < -280.0)
{
  // $location is a slave device

  if(IAmAnOnSlave($location))
  {
     $action = 'ON';
  }

  if($debug)
     $debugString = $debugString . ' (I am a slave.)';
} else
{
  // $location is a master device

  if($set > $t)
  {
    TurnOnDependentList($location);
    $action = 'ON';
  }
  SaveTemperature($location, $action, $t, $set);
}

// Tell the device what to do

echo $action;

// Send helpful info as well if we are debugging

if($debug)
{
  echo $debugString;
}

?></body>
</html>
