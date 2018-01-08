<html>
<head><title>controllednode</title></head>
<body><?php

// Debugging...

ini_set('display_errors', 'On');
error_reporting(E_ALL | E_STRICT);
$debug = true;

// Append debugging info to this string

$debugString = '';

// Where the files are.

$fileRoot = 'Data/';
$profileRoot = 'aandc-profile-';
$fileExtension = '.dat';

// Special characters

$delimiter = '*';
$timeDelimiter = ':';

// Now

$unixTime = 0 + date_timestamp_get(date_create());


// ***********************************************************************

// This converts $timeOfDay in the time format in heating profile files ("hh:mm:ss")
// to the number of seconds since midnight.

function HMSToSeconds($timeOfDay)
{
   $parsed = date_parse($timeOfDay);
   $seconds = $parsed['hour'] * 3600 + $parsed['minute'] * 60 + $parsed['second'];
   return 0 + $seconds;
}

// This removes ' ' from the start of a string.  Useful for parsing
// the heating profile files.

function EatSpaces(&$text)
{
	while(substr($text, 0, 1) == ' ' || substr($text, 0, 1) == "\n")
		$text = substr($text, 1);
}

// This removes any '*' and ' ' from the start of a string.  Useful for parsing
// the heating profile files.

function EatDelimitersAndSpaces(&$text)
{
	global $delimiter;

	while(substr($text, 0, 1) == $delimiter || substr($text, 0, 1) == ' ' || substr($text, 0, 1) == "\n")
		$text = substr($text, 1);
}

// This puts the next line (up to \n) from the $text into $line and removes
// it from $profile.  It also checks that the line starts with a string delimiter.
// If there are no more lines it returns false.

function NextLine(&$text, &$ln)
{
   global $delimiter;

   $lineEnd = strpos($text, "\n");
   if(!$lineEnd)
   {
	$text = 'X';
	return false;
   }

   $ln = substr($text, 0, $lineEnd);
   if(substr($ln, 0, 1) != $delimiter)
	exit('ERROR - NextLine(): line does not start with a '.$delimiter.': ' . $ln);
   $text = substr($text, 1 + $lineEnd);
   return true;
}

// This returns the next name (up to but not includeing $delimiter) from $line and
// removes that from $line (including any trailing spaces and $delimiters).  If there is
// no next name false is returned.

function NextName(&$text, &$nam)
{
   global $delimiter;

   EatDelimitersAndSpaces($text);
   $endName = strpos($text, $delimiter);
   if(!$endName)
	return false;
   $nam = str_replace(' ', '', substr($text, 0, $endName));
   $text = substr($text, $endName);
   EatDelimitersAndSpaces($text);
   return true;
}

// This returns the next number (up to but not includeing ' ') from $line and
// removes that from $line (including any trailing ' ').  If there is no next number false
// is returned.  Note times (hh:mm:ss) are considered numbers and returned.

function NextNumber(&$text, &$num)
{
   if($text == '')
	return false;

   EatSpaces($text);
   $endNumber = strpos($text, ' ');
   if(!$endNumber)
   {
      $num = str_replace('"', '', $text);
      $text = '';
   } else
   {
      $num = str_replace('"', '', substr($text, 0, $endNumber));
      $text = substr($text, $endNumber);
   }
   EatSpaces($text);
   return true;
}


function GetTemperatureFromElsewhere(&$therm)
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, $iAmMyOwnSlave, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter, $thermometer;

   $thermName = $fileRoot . $therm . $fileExtension;
   $thermContents = file_get_contents($thermName);
   EatSpaces($thermContents);
   if(!NextNumber($thermContents, $therm))
	 exit('ERROR: GetTemperatureFromElsewhere() - temperature not found:'.$thermContents);
}


// This parses the profile file which is written by the web interface.

// If $device is a master:
// The result is an array of dependent slaves for $device, $mySlaves, the boolean $iAmOn set,
// The Unix time $boostTime set, and the day's list of pairs of times and temperatures, $times and $temps.

// If $device is a slave:
// $times is empty().


function ParseProfile($device)
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, $iAmMyOwnSlave, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter, $thermometer;

   $profileName = $fileRoot . $profileRoot . strtolower(date('l')) . $fileExtension;
   $profile = file_get_contents($profileName);

   $listStart = strpos($profile, '---');
   if(!$listStart)
	exit('ERROR: ParseProfile() - string --- not found');

   $profile = substr($profile, 4 + $listStart);
   while(NextLine($profile, $line))
   {
	$thisLine = $line;
	if(!NextName($line, $name))
		exit('Error: ParseProfile() - first name on line missing.' . $thisLine);
	if($name == $device)
	{
		if(!NextName($line, $thermometer))
			exit('Error: ParseProfile() - thermometer name on line missing.' . $thisLine);

		if($thermometer == $device)
		{
			$thermometer = false;
		} else
		{
			GetTemperatureFromElsewhere($thermometer);
		}

		$slaveCount = 0;
		$iAmMyOwnSlave = false;
		while(NextName($line, $name))
		{
			if($name != $device)
			{
				$mySlaves[$slaveCount] = $name;
				$slaveCount++;
			} else
			{
				$iAmMyOwnSlave = true;
			}
		}

		if(!NextNumber($line, $number))
			exit('ERROR: ParseProfile() - on/off digit not found: '.$thisLine);
		$iAmOn = ((0+$number) == 1);

		if(!NextNumber($line, $number))
			exit('ERROR: ParseProfile() - boost time not found: '.$thisLine);
		$boostTime = 0 + $number;

		$timeCount = 0;
		while(NextNumber($line, $time))
		{
			$times[$timeCount] = HMSToSeconds($time);
			if(!NextNumber($line, $temp))
				exit('ERROR: ParseProfile() - time without temp: '.$thisLine);
			$temps[$timeCount] = 0 + $temp;
			$timeCount++;
		}

/* Debug parsing: */

		echo $device.':<br> is ';
		if($iAmOn)
			echo 'ON<br>';
		else
			echo 'OFF<br>';

		if(!($thermometer === false))
			echo 'Temperature from elsewhere: ' . $thermometer . '<br>';

		if($iAmMyOwnSlave)
			echo 'I am my own slave <br>';

		echo ' boost: '.$boostTime.' (Uxt: '.$unixTime.')<br> Ts&Ts:<br>';
		for($i = 0; $i < sizeof($times); $i++)
			echo ' '.$times[$i].' '.$temps[$i].'<br> slaves:';
		for($i = 0; $i < sizeof($mySlaves); $i++)
			echo ' '.$mySlaves[$i];
		echo '<br>';


		$profile = 'X';
		return;
	}
   }
   $profile = 'X';
}

// Return the set temperature for $device at the current time.

function SetTemperature($device) 
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, $iAmMyOwnSlave, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter, $thermometer;

    if(empty($profile))
	exit('ERROR: SetTemperature() - profile not loaded');

    // Am I a slave device?

    if(empty($times))
	return -300.0; // Yes


    // No.  Am I on?

    if(!$iAmOn)
        return 10.0;

    // Am I boosted?

    if($boostTime > $unixTime)
	return 28.0;

    // None of the above - return the temperature from the profile.

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
// return true.  A slave device needs to be on if its .dat file has a
// timestamp less than two minutes old.  As every device gets information
// on what it should be doing once a minute +/- 5 seconds, every slave
// device that needs to be on will have had its .dat file touched less than
// 2 minutes (120 seconds) ago.

function IAmAnOnSlave($device)
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, $iAmMyOwnSlave, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter, $thermometer;

    if(empty($profile))
	exit('ERROR: IAmAnOnSlave() - profile not loaded');

    $fileName = $fileRoot . $device . $fileExtension;
    $t = filemtime($fileName);
    if(!$t)
	exit('ERROR: IAmAnOnSlave() - slave file not found: ' . $fileName);
    $fileTouched = 0 + $t;
    return (($unixTime - $fileTouched) < 120);
}

// Touch the .dat files of slave devices that also need to be turned on if 
// master $device is on.  

function TurnOnDependentList($device)
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, $iAmMyOwnSlave, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter, $thermometer;

    if(empty($profile))
	exit('ERROR: TurnOnDependentList() - profile not loaded');

    if(empty($mySlaves))
	return;

    for ($i = 0; $i < sizeof($mySlaves); $i++)
    {
	if($mySlaves[$i] != $device) // Don't touch my own file as it will be written to anyway
	{
		$fileName = $fileRoot . $mySlaves[$i] . $fileExtension;
		touch($fileName);
	}
    }
}

// Save the status of a master device in its .dat file.

function SaveTemperature($device, $act, $temp, $s)
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, $iAmMyOwnSlave, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter, $thermometer;

    $fileName = $fileRoot . $device . $fileExtension;
    $fileHandle = fopen($fileName, 'w');
    if(!$fileHandle)
	exit('ERROR: SaveTemperature() - can not open file to write: '.$fileName); 
    fwrite($fileHandle, $temp . ' ' . $s . ' ' . $act . "\n");
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

// if $thermometer is true some other device returns my temperature, and that temperature
// will now be in $thermometer.  Note use of === as 0 is a valid temperature.

if($thermometer === false)
{
	$t = 0.0 + $temperature;
} else
{
	$t = 0.0 + $thermometer;
}

// What temperature should I be?

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
	    if($iAmMyOwnSlave)
	    	$action = 'ON';
	    else if($debug)
		$debugString = $debugString . ' (I do not control myself.)';
	    
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
