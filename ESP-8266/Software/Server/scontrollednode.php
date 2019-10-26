<html>
<head><title>controllednode</title></head>
<body><?php

/*
 * Program to serve HTTP requests from a node on the WiFi Heating Control system.
 *  
 * Adrian Bowyer
 * RepRap Ltd
 * http://reprapltd.com
 * 
 * 29 December 2017
 *
 * Licence: GPL
 */

// HTTP requests to this look like:  http://currentServer/pageRoot/scontrollednode.php?unit=1&load=0&temperature=20[&debugOn=1]

// Note to self: don't forget "\n" means newline; '\n' means \n...

// Debugging

ini_set('display_errors', 'On');
error_reporting(E_ALL | E_STRICT);
$debug = false;

// Append debugging info to this string

$debugString = '<br><br>';

// Where the files are.

$fileRoot = '/Data/';
$profileText = '-profile-';
$fileExtension = '.dat';
$units = 'units';

// Special characters

$delimiter = '*';
$timeDelimiter = ':';

// Occasional side effect when called - create the file of current temperatures about once a minute.

$creatingTemperatureFile = false;
$temperatureFileContents = "";
$temperatureFileName = "";

// Now

date_default_timezone_set("Europe/London");

$summerTime = false;
$unixTime = 0 + date_timestamp_get(date_create());
if (date('I', time()))
{
	$unixTime += 3600;
	$summerTime = true;
}


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

// This removes any '*' ($delimiter) and ' ' from the start of a string.  Useful for parsing
// the heating profile files.

function EatDelimitersAndSpaces(&$text)
{
	global $delimiter;

	while(substr($text, 0, 1) == $delimiter || substr($text, 0, 1) == ' ' || substr($text, 0, 1) == "\n")
		$text = substr($text, 1);
}

// This puts the next line (up to \n) from the $text into $ln and removes
// it from $text. 
// If there are no more lines it returns false. It assumes that the last line ends with a "\n".

function NextLineNoCheck(&$text, &$ln)
{
   $lineEnd = strpos($text, "\n");
   if(!$lineEnd)
   {
	$text = 'X';
	return false;
   }

   $ln = substr($text, 0, $lineEnd);

   $text = substr($text, 1 + $lineEnd);

   return true;
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

   // If we are reading the header and we get to '---' we have got to the end of the header.

   if(substr($ln, 0, 3) == '---')
	return false;

   if(substr($ln, 0, 1) != $delimiter)
	exit('ERROR - NextLine(): line does not start with a '.$delimiter.': ' . $ln);
   $text = substr($text, 1 + $lineEnd);
   return true;
}

// This returns the next name (up to but not includeing $delimiter) from $text and
// removes that from $text (including any trailing spaces and $delimiters).  If there is
// no next name false is returned.

function NextNameWithSpaces(&$text, &$nam)
{
   global $delimiter;

   EatDelimitersAndSpaces($text);
   $endName = strpos($text, $delimiter);
   if(!$endName)
	return false;
   $nam = substr($text, 0, $endName);
   $text = substr($text, $endName);
   EatDelimitersAndSpaces($text);
   return true;
}

// This returns the next name (up to but not includeing $delimiter) from $text and
// removes that from $text (including any trailing spaces and $delimiters).  If there is
// no next name false is returned.  If the name contains ' ' these are removed.

function NextName(&$text, &$nam)
{
   global $delimiter;

   $result = NextNameWithSpaces($text, $nam);
   $nam = str_replace(' ', '', $nam);
   return $result;
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
// that temperature from the little file recorded by the other device, overwriting $therm.  In addition it loads 
// the set temperature and whether the device is ON or OFF, which allows it also to be used for constructing
// the list-of-current-temperatures file.  If we are debugging it also checks that the little file has been
// updated recently (in other words, it checks that its client device called this .php script within the last
// three minutes to record a temperature).

function GetTemperatureFromElsewhere($house, &$therm, &$set, &$on)
{
include 'globals.php';

   $thermName = $house . $fileRoot . $therm . $fileExtension;

   if($debug)
   {
	    $t = filemtime($thermName);
	    if(!$t)
		exit('ERROR: GetTemperatureFromElsewhere() - temperature file not found: ' . $fileName);
	    $fileTouched = 0 + $t;
	    if($summerTime)
	    	$fileTouched += 3600;
	    $fileTouched = $unixTime - $fileTouched;
	    if($fileTouched > 180)
	    {
	    	$debugString = $debugString . 'The file ' . $thermName . ' has not been updated for ' . $fileTouched .' seconds.<br>';
	    }
   }

   $thermContents = file_get_contents($thermName);
   EatSpaces($thermContents);
   if(!NextNumber($thermContents, $therm))
	 exit('ERROR: GetTemperatureFromElsewhere() - current temperature not found:'.$thermContents);
   if(!NextNumber($thermContents, $set))
	 exit('ERROR: GetTemperatureFromElsewhere() - set temperature not found:'.$thermContents);
   if(substr($thermContents, 0, 3) == 'OFF')
	$on = false;
   else if(substr($thermContents, 0, 2) == 'ON')
	$on = true;
   else
	exit('ERROR: GetTemperatureFromElsewhere() - device is neither ON nor OFF:'.$thermContents);
}


// This reads the first part of the profile file until it finds the device,
// then gets the on and off delays in seconds for that device's switching.

function GetSwitchingDelays($device)
{
include 'globals.php';

   // The first time the name $device appears will be a line like
   // *Boiler* 5 0 30.0 0.0
   // The first two numbers were for the old Panstamp and can be
   // ignored. The third is the delay in seconds before switching
   // on, the fourth the delay for switching off.

   // The device may not exist in the header (if it all works by slaves).  Set some defaults.

   $onDelay = 0;
   $offDelay = 0;

   while(NextLine($profile, $line))
   {
        // Save the original line for the moment in case we need to report errors in it.

	$thisLine = $line;

	if(!NextName($line, $name))
		exit('Error: ParseProfile() - first name on line missing.' . $thisLine);

	// Is this the line for this device?

	if($name == $device)
	{
	   if(!NextNumber($line, $number))
		exit('ERROR: GetSwitchingDelays() - number 1 not found: '.$thisLine);
	   if(!NextNumber($line, $number))
		exit('ERROR: GetSwitchingDelays() - number 2 not found: '.$thisLine);
	   if(!NextNumber($line, $number))
		exit('ERROR: GetSwitchingDelays() - number 3 not found: '.$thisLine);
           $onDelay = $number;
	   if(!NextNumber($line, $number))
		exit('ERROR: GetSwitchingDelays() - number 4 not found: '.$thisLine);
	   $offDelay = $number;
           return;
	}
   }
}

// This adds a line to the temperature record file for device $name.
// This file is loaded by the control web interface to show the
// current state of the system.
//
// A line looks like:
//
//   *name* temperature set-temperature on/off
//   *Electronics Lab* 23 10 0
//                           ^ 0 for off, 1 for on.

function AddALineToTheTemperatureFile($house, $name)
{
include 'globals.php';

	$temp = str_replace(' ', '', $name);
	$set = 0;
	$on = false;

	GetTemperatureFromElsewhere($house, $temp, $set, $on);

	//$temperatureFileContents = $temperatureFileContents . $delimiter . $name . $delimiter . ' ' . $temp . ' ' . $set;
	$temperatureFileContents = $temperatureFileContents . $delimiter . $name . $delimiter . ' ' . round(0.0 + $temp) . ' ' . round(0.0 + $set);
	if($on)
		$temperatureFileContents = $temperatureFileContents . " 1\n";
	else
		$temperatureFileContents = $temperatureFileContents . " 0\n";
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

function ParseProfile($house, $device)
{
include 'globals.php';

   $profileName = $house . $fileRoot . strtolower($house) . $profileText . strtolower(date('l')) . $fileExtension;
   $profile = file_get_contents($profileName);

   // Do we need to update the list of room temperatures for the web interface?
   // Do this about once a minute.

   $temperatureFileName = $house . $fileRoot . strtolower($house) . "-temperatures" . $fileExtension;
   $t = filemtime($temperatureFileName);
   if(!$t)
	exit('ERROR: ParseProfile() - temperature list file not found: ' . $temperatureFileName);
   $fileTouched = 0 + $t;
   if($summerTime)
    	$fileTouched += 3600;
   if(($unixTime - $fileTouched) > 60)
   {
   	if($debug)
    		$debugString = $debugString . 'Updating the temperature list in ' . $temperatureFileName .'<br>';
        $creatingTemperatureFile = true;
	$temperatureFileContents = "";
   }
      

   // N.B. GetSwitchingDelays() will eat some of the start of $profile, but shouldn't get to the '---'

   GetSwitchingDelays($device);

   // Ignore the old Panstamp control information at the start of the file. There is a line
   // with --- on between that and the stuff we want.

   $listStart = strpos($profile, '---');
   if($listStart === false)
	exit('ERROR: ParseProfile() - string --- not found in ' . $profile);

   $profile = substr($profile, 4 + $listStart);

   // Flag that's set when we have found $device to prevent searching further. If we are not
   // creating the temperature list file, we can return as soon as $device's details are loaded.
   // If we are, we carry on in the loop for all the devices creating the file.

   $gotTheProfile = false;

   while(NextLine($profile, $line))
   {
	$thisLine = $line;
	
	// Get the next name with the spaces in as we may need it in that form for the temperature
        // list file.

	if(!NextNameWithSpaces($line, $name))
		exit('Error: ParseProfile() - first name on line missing.' . $thisLine);

	if($creatingTemperatureFile)
		AddALineToTheTemperatureFile($house, $name);

	// Now get rid of the spaces

        $name = str_replace(' ', '', $name);

	// If we haven't yet found $device, is this the line for it?

	if(!$gotTheProfile && $name == $device)
	{
		if(!NextName($line, $thermometer))
			exit('Error: ParseProfile() - thermometer name on line missing.' . $thisLine);

		if($thermometer == $device)
		{
			// We are our own thermometer

			$thermometer = false;
		} else
		{
			// Someone else is our thermometer

			$set = 0;
			$on = false;
			GetTemperatureFromElsewhere($house, $thermometer, $set, $on);
                        if($debug)
			{
				$debugString = $debugString . '<br>(Another gives my temperature. Its t is ' . $thermometer . ', its set t is '
 					. $set . ' and it is ';
				if($on)
					$debugString = $debugString . 'ON';
				else
					$debugString = $debugString . 'OFF';
				$debugString = $debugString . ')<br>';
			}
		}

		// Am I responsible for turning myself off and on, or does someone else tell me to?

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
			$debugString = $debugString . $device. ' in building ' . $house .' is ';
			if($iAmOn)
				$debugString = $debugString . 'active<br>';
			else
				$debugString = $debugString . 'inactive (10C)<br>';

			if(!($thermometer === false))
				$debugString = $debugString . 'Temperature from elsewhere: ' . $thermometer . '<br>';

			if($iAmMyOwnSlave)
				$debugString = $debugString . 'It is its own slave <br>';

			$debugString = $debugString . ' boost: '.$boostTime.' (Uxt: '.$unixTime.')<br> Ts&Ts:<br>';
			for($i = 0; $i < sizeof($times); $i++)
				$debugString = $debugString . ' '.$times[$i].' '.$temps[$i].'<br>';
			$debugString = $debugString . 'slaves:<br>';
			for($i = 0; $i < sizeof($mySlaves); $i++)
				$debugString = $debugString . ' '.$mySlaves[$i];
			$debugString = $debugString . '<br>';
		}

		// If we are not creating the list of temperatures, we can go home now.
		// If we are we carry on in the loop, but stop looking for $device.
		// At the end no more information is needed from $profile, but it has to contain something, hence 'X'.

		if(!$creatingTemperatureFile)
		{
			$profile = 'X'; 
			return;
		} else
			$gotTheProfile = true;
	}
   }
   $profile = 'X';
}

// What it says...

function SecondsSinceMidnight()
{
include 'globals.php';

	return $unixTime % 86400; // There are 86400 seconds in 24 hours
}


// Time as "hh:mm:ss"

function ServerTime()
{
	$secondsSinceMidnight = SecondsSinceMidnight();
        $h = intval($secondsSinceMidnight/3600);
        $m = intval(($secondsSinceMidnight - $h*3600)/60);
        $s = intval($secondsSinceMidnight - $h*3600 - $m*60);
        return '' . $h . ':' . $m . ':' . $s; 
}


// Return the set temperature for $device at the current time.

function SetTemperature($device) 
{
include 'globals.php';

    if(empty($profile)) // That's why $profile has to contain something...
	exit('ERROR: SetTemperature() - profile not loaded');

    $secondsSinceMidnight = SecondsSinceMidnight();
    
    if($debug)
    {
        $debugString = $debugString . 'Server time: ' . ServerTime() .'<br>'; 
    }

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

function IAmAnOnSlave($house, $device)
{
include 'globals.php';

    if(empty($profile))
	exit('ERROR: IAmAnOnSlave() - profile not loaded');

    $fileName = $house . $fileRoot . $device . $fileExtension;
    $t = filemtime($fileName);
    if(!$t)
	exit('ERROR: IAmAnOnSlave() - slave file not found: ' . $fileName);
    $fileTouched = 0 + $t;
    if($summerTime)
    	$fileTouched += 3600;
    if($debug)
    	$debugString = $debugString . 'File touched time: ' . $fileTouched . ', Unix time: ' . $unixTime .'<br>';
    return (($unixTime - $fileTouched) < 120);
}

// Touch the .dat files of slave devices that also need to be turned on if 
// master $device is on.  

function TurnOnDependentList($house, $device)
{
include 'globals.php';

    if(empty($profile))
	exit('ERROR: TurnOnDependentList() - profile not loaded');

    if(empty($mySlaves))
	return;

    for ($i = 0; $i < sizeof($mySlaves); $i++)
    {
	if($mySlaves[$i] != $device) // Don't touch my own file as it will be written to anyway
	{
		$fileName = $house . $fileRoot . $mySlaves[$i] . $fileExtension;
		touch($fileName);
	}
    }
}

// If we are logging data, maybe add to the log file

function MaybeUpdateLog($house, $device, $act, $temp, $s)
{
include 'globals.php';

	// If there is no log file, we are not logging

	$logFileName = $house . $fileRoot . 'Log' . $fileExtension;
	if(!file_exists($logFileName))
		return;

	$fileName = $house . $fileRoot . $device . $fileExtension;
	$lastState = file_get_contents($fileName);
        if(!$lastState)
		exit('ERROR: MaybeUpdateLog() - can not get file contents: '.$fileName);
	
	// If the state hasn't changed, do nothing.
 
        if($act == 'OFF')
        {
		if(strpos($lastState, 'OFF'))
			return;
        }
        if($act == 'ON')
        {
		if(strpos($lastState, 'ON'))
			return;
        }


	file_put_contents($logFileName, $house.','.$device.','.$act.','.$temp.','.$unixTime."\n" , FILE_APPEND);
}

// Save the status of a master device in its .dat file.

function SaveTemperature($house, $device, $act, $temp, $s)
{
include 'globals.php';

    MaybeUpdateLog($house, $device, $act, $temp, $s);

    $fileName = $house . $fileRoot . $device . $fileExtension;
    $fileHandle = fopen($fileName, 'w');
    if(!$fileHandle)
	exit('ERROR: SaveTemperature() - can not open file to write: '.$fileName); 
    fwrite($fileHandle, $temp . ' ' . $s . ' ' . $act . ' Server time: ' . ServerTime() . "\n");
    fclose($fileHandle);
    if($debug)
	$debugString = $debugString . $device . ' state: temp = ' . $temp . ', set = ' . $s;
}


// This reads the file that lists all the unit and load numbers and associates them with a
// building and location in it, and finds the one corresponding to the device that has
// send the HTTP request.  It also sets the temperature offset for that unit.

function ReadUnit($unit, $load)
{
include 'globals.php';

    $fileName = $units . $fileExtension;
    $allUnits = file_get_contents($fileName);
    if(empty($allUnits))
	exit('ERROR: ReadUnits() - can not open file to read: '.$fileName);
    while(NextLineNoCheck($allUnits, $line))
    {
	if(!NextNumber($line, $n1))
		exit('ERROR: ReadUnit() - line does not start with a number: '.$line1);
	if(!NextNumber($line, $n2))
		exit('ERROR: ReadUnit() - line does not start with two numbers: '.$line1);
        if($n1 == $unit && $n2 == $load)
	{
		if(!NextName($line, $building))
			exit('ERROR: ReadUnit() - line does not have a delimited building name: '.$line1);
		if(!NextName($line, $location))
			exit('ERROR: ReadUnit() - line does not have a delimited room name: '.$line1);
		if(!NextNumber($line, $temperatureOffset))
			exit('ERROR: ReadUnit() - line does not have a temperature offset: '.$line1);
		$temperatureOffset = 0 + $temperatureOffset;
		return;
	}
    }
    exit('ERROR: ReadUnit() - unit and load not found: '.$unit.', '.$load);
}

//***************************************************************

// Gather data...

// Get the query sring from the HTTP request.
// This should be: "html://...../controllednode.php?building=house&location=where/what-the-device-is&temperature=the-device's-temperature[&debugOn=1]"
// If the device does not have a temperature it can send -300.0 (impossible, as below abs zero)

parse_str($_SERVER['QUERY_STRING']);


// Check we have a $device/$location and $temperature from the HTTP query...

//if(empty($location) || empty($temperature) || empty($building))
//	exit('ERROR - $location, $temperature or $building undefined');

if(!strlen($unit) || !strlen($temperature) || !strlen($load))
	exit('ERROR - $unit, $temperature or $load undefined');

$debug = !empty($debugOn);

ReadUnit($unit, $load);

$temperature = $temperature+$temperatureOffset;

ParseProfile($building, $location);

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

	if(IAmAnOnSlave($building, $location))
	{
	     $action = 'ON';
	}

	if($debug)
	     $debugString = $debugString . ' (I am a slave and I am ' . $action . '.)';
} else
{
	// $location is a master device

	if($set > $t)
	{
	    TurnOnDependentList($building, $location);
	    if($iAmMyOwnSlave)
	    	$action = 'ON';
	    else if($debug)
		$debugString = $debugString . ' (I do not control myself.)';
	    
	}
	SaveTemperature($building, $location, $action, $t, $set);
}

if($action == 'ON')
 $action = $action . ' ' . $onDelay;
else
 $action = $action . ' ' . $offDelay; 	

// Tell the device what to do as the first line of the HTML <body> returned

echo $action;

// Write the list of temperatures file if we are creating it.

if($creatingTemperatureFile)
{
    $fileHandle = fopen($temperatureFileName, 'w');
    if(!$fileHandle)
	exit('ERROR: Creating list of temperatures - can not open file to write: '.$temperatureFileName); 
    fwrite($fileHandle, $temperatureFileContents);
    fclose($fileHandle);
    if($debug)
	$debugString = $debugString . '<br>Temperature list written.<br>';
}

// Send helpful info in the HTML <body> as well if we are debugging

if($debug)
{
  echo $debugString;
}


?></body>
</html>

