<html>
<head><title>controllednode</title></head>
<body><?php

/*
 * Program to serve HTTP requests from a node on the WiFi Heating Control system.
 *  
 * Adrian Bwowyer
 * RepRap Ltd
 * http://reprapltd.com
 * 
 * 29 December 2017
 *
 * Licence: GPL
 */

// HTTP requests to this look like: http://host-url/heating/WiFi/Building/controllednode.php?location=Room&temperature=20[&debugOn=1]

// Debugging...

ini_set('display_errors', 'On');
error_reporting(E_ALL | E_STRICT);
$debug = false;

// Append debugging info to this string

$debugString = '<br><br>';

// Where the files are.

$fileRoot = 'Data/';
$profileRoot = 'workshop-profile-';
$fileExtension = '.dat';

// Special characters

$delimiter = '*';
$timeDelimiter = ':';

// Now
/* This is untested
$juneSolstice = date_create();
$juneSolstice.setMonth(5); // zero-based
$juneSolstice.setDate(21);

$decemberSolstice = date_create();
$decemberSolstice.setMonth(11); // zero-based
$decemberSolstice.setDate(21);


$winterOffset = Math.max($juneSolstice.getTimezoneOffset(), $decemberSolstice.getTimezoneOffset());
$summerOffset = Math.min($juneSolstice.getTimezoneOffset(), $decemberSolstice.getTimezoneOffset());
*/

date_default_timezone_set("Europe/London");

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

// This puts the next line (up to \n) from the $text into $ln and removes
// it from $text.  It also checks that the line starts with a string delimiter.
// If there are no more lines it returns false. It assumes that the last line ends with a "\n".

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

// This returns the next name (up to but not includeing $delimiter) from $text and
// removes that from $text (including any trailing spaces and $delimiters).  If there is
// no next name false is returned.  If the name contains ' ' these are removed.

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

// This returns the next number (up to but not includeing ' ') from $text and
// removes that from $text (including any trailing ' ').  If there is no next number false
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

// Some devices use another device ($therm) to measure the temperature they should be using.  This loads
// that temperature from the little file recorded by the other device.

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
// $iAmMyOwnSlave true means I need to turn myself on or off.  False means I don't need to throw my
// internal switch. If !($thermometer === false) then it contains the temperature I need to
// use in preference to any that my own device has supplied.  (Note the === is needed because 0 is a
// valid temperature.)

// If $device is a slave:

// $times is empty().


function ParseProfile($device)
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, $iAmMyOwnSlave, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter, $thermometer;

   $profileName = $fileRoot . $profileRoot . strtolower(date('l')) . $fileExtension;
   $profile = file_get_contents($profileName);

   // Ignore the old Panstamp control information at the start of the file. There is a line
   // with --- on between that and the stuff we want.

   $listStart = strpos($profile, '---');
   if(!$listStart)
	exit('ERROR: ParseProfile() - string --- not found.');

   $profile = substr($profile, 4 + $listStart);
   while(NextLine($profile, $line))
   {
	$thisLine = $line;
	if(!NextName($line, $name))
		exit('Error: ParseProfile() - first name on line missing.' . $thisLine);

	// Is this the line for this device?

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

// Debug parsing

		if($debug)
		{
			$debugString = $debugString . $device.':<br> is ';
			if($iAmOn)
				$debugString = $debugString . 'active<br>';
			else
				$debugString = $debugString . 'inactive (10C)<br>';

			if(!($thermometer === false))
				$debugString = $debugString . 'Temperature from elsewhere: ' . $thermometer . '<br>';

			if($iAmMyOwnSlave)
				$debugString = $debugString . 'I am my own slave <br>';

			$debugString = $debugString . ' boost: '.$boostTime.' (Uxt: '.$unixTime.')<br> Ts&Ts:<br>';
			for($i = 0; $i < sizeof($times); $i++)
				$debugString = $debugString . ' '.$times[$i].' '.$temps[$i].'<br>';
			$debugString = $debugString . 'slaves:<br>';
			for($i = 0; $i < sizeof($mySlaves); $i++)
				$debugString = $debugString . ' '.$mySlaves[$i];
			$debugString = $debugString . '<br>';
		}

		// No more information needed from $profile, but it has to contain something.

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

    if(empty($profile)) // That's why $profile has to contain something...
	exit('ERROR: SetTemperature() - profile not loaded');

    // Am I a slave device?

    if(empty($times))
	return -300.0; // Yes - return an impossible (< abs 0) temperature to flag the fact


    // No.  Am I on?

    if(!$iAmOn)
        return 10.0; // Just to prevent pipes freezing in cold weather...

    // Am I boosted?

    if($boostTime > $unixTime)
	return 28.0; // I think that's hot enought to guarantee that it turns on...

    // None of the above - return the temperature from the profile.

    $secondsSinceMidnight = $unixTime % 86400; //(time() % 86400);
    
    if($debug)
    {
        $h = intval($secondsSinceMidnight/3600);
        $m = intval(($secondsSinceMidnight - $h*3600)/60);
        $s = intval($secondsSinceMidnight - $h*3600 - $m*60);
        $debugString = $debugString . 'Server time: ' . $h . ':' . $m . ':' . $s .'<br>'; 
    }

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
	$debugString = $debugString . $device . ' state: temp = ' . $temp . ', set = ' . $s;
}

//***************************************************************

// Gather data...

// Get the query sring from the HTTP request.
// This should be: "html://...../controllednode.php?location=where/what-the-device-is&temperature=the-device's-temperature"
// If the device does not have a temperature it can send -300.0 (impossible as below abs zero)

parse_str($_SERVER['QUERY_STRING']);

// Check we have a $device/$location and $temperature from the HTTP query...

if(empty($location) || empty($temperature))
	exit('ERROR - $location or $temperature undefined');

$debug = !empty($debugOn);

ParseProfile($location);

// if $thermometer is not false some other device returned my temperature, and that temperature
// will now be in $thermometer.  Note use of ===, because 0 (casts to false) is a valid temperature.

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

