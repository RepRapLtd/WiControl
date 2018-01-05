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
   global $profile, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

   $parsed = date_parse($timeOfDay);
   $seconds = $parsed['hour'] * 3600 + $parsed['minute'] * 60 + $parsed['second'];
   return 0 + $seconds;
}

// This removes ' ' from the start of a string.  Useful for parsing
// the heating profile files.

function EatSpaces()
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

	while(substr($line, 0, 1) == ' ' || substr($line, 0, 1) == "\n")
		$line = substr($line, 1);
}

// This removes any '*' and ' ' from the start of a string.  Useful for parsing
// the heating profile files.

function EatDelimitersAndSpaces()
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

	while(substr($line, 0, 1) == $delimiter || substr($line, 0, 1) == ' ' || substr($line, 0, 1) == "\n")
		$line = substr($line, 1);
}

// This puts the next line (up to \n) from the $profile into $line and removes
// it from $profile.  It also checks that the line starts with a string delimiter.
// If there are no more lines it returns false.

function NextLine()
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

   $lineEnd = strpos($profile, "\n");
   if(!$lineEnd)
   {
	$profile = 'X';
	return false;
   }

   $line = substr($profile, 0, $lineEnd);
   if(substr($line, 0, 1) != $delimiter)
	exit('ERROR - NextLine(): line does not start with a '.$delimiter.': ' . $line);
   $profile = substr($profile, 1 + $lineEnd);
   return true;
}

// This returns the next name (up to but not includeing $delimiter) from $line and
// removes that from $line (including any trailing spaces and $delimiters).  If there is
// no next name false is returned.

function NextName()
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

   EatDelimitersAndSpaces();
   $endName = strpos($line, $delimiter);
   if(!$endName)
	return false;
   $name = str_replace(' ', '', substr($line, 0, $endName));
   $line = substr($line, $endName);
   EatDelimitersAndSpaces();
   return $name;
}

// This returns the next number (up to but not includeing ' ') from $line and
// removes that from $line (including any trailing ' ').  If there is no next number false
// is returned.  Note times (hh:mm:ss) are considered numbers and returned.

function NextNumber()
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

   if($line == '')
	return false;

   EatSpaces();
   $endNumber = strpos($line, ' ');
   if(!$endNumber)
   {
      $number = str_replace('"', '', $line);
      $line = '';
   } else
   {
      $number = str_replace('"', '', substr($line, 0, $endNumber));
      $line = substr($line, $endNumber);
   }
   EatSpaces();
   return $number;
}


// This parses the profile file which is written by the web interface.

// If $device is a master:
// The result is an array of dependent slaves for $device, $mySlaves, the boolean $iAmOn set,
// The Unix time $boostTime set, and the day's list of pairs of times and temperatures, $times and $temps.

// If $device is a slave:
// $times is empty().


function ParseProfile($device)
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

   $profileName = $fileRoot . $profileRoot . strtolower(date('l')) . $fileExtension;
   $profile = file_get_contents($profileName);

   $listStart = strpos($profile, '---');
   if(!$listStart)
	exit('ERROR: ParseProfile() - string --- not found');

   $profile = substr($profile, 4 + $listStart);
   while(NextLine())
   {
	$thisLine = $line;
	$name = NextName();
	if($name == $device)
	{
		$slaveCount = 0;
		$name = NextName(); // Thermometer device in the old system - ignore
		if(!$name)
			exit('Error: ParseProfile() - thermometer name missing.' . $thisLine);
		$name = NextName();
		while($name)
		{
			if($name != $device)
			{
				$mySlaves[$slaveCount] = $name;
				$slaveCount++;
			}
			$name = NextName();
		}

		$number = NextNumber();
		if($number === false) // NB 0 is a valid return
			exit('ERROR: ParseProfile() - on/off digit not found: '.$thisLine);
		$iAmOn = (0+$number == '1');
		$number = NextNumber();
		if($number === false)
			exit('ERROR: ParseProfile() - boost time not found: '.$thisLine);
		$boostTime = 0 + $number;

		$timeCount = 0;
		$time = NextNumber();
		while(!($time === false))
		{
			$times[$timeCount] = HMSToSeconds($time);
			$temp = NextNumber();
			if($temp === false)
				exit('ERROR: ParseProfile() - time without temp: '.$thisLine);
			$temps[$timeCount] = 0 + $temp;
			$timeCount++;
			$time = NextNumber();
		}

/* Debug parsing: */

		echo $device.':<br> is ';
		if($iAmOn)
			echo 'ON<br>';
		else
			echo 'OFF<br>';

		echo ' boost: '.$boostTime.' (Uxt: '.$unixTime.')<br> Ts&Ts:<br>';
		for($i = 0; $i < sizeof($times); $i++)
			echo ' '.$times[$i].' '.$temps[$i].'<br>';
		echo ' slaves:';
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
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

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
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

    if(empty($profile))
	exit('ERROR: IAmAnOnSlave() - profile not loaded');

    $fileName = $fileRoot . $device . $fileExtension;
    $t = filemtime($fileName);
    if(!$t)
	exit('ERROR: IAmAnOnSlave() - slave file not found: ' . $fileName);
    $fileTouched = 0 + $t;
    return ($unixTime - $fileTouched < 120);
}

// Touch the .dat files of slave devices that also need to be turned on if 
// master $device is on.  

function TurnOnDependentList($device)
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

    if(empty($profile))
	exit('ERROR: TurnOnDependentList() - profile not loaded');

    if(empty($mySlaves))
	return;

    for ($i = 0; $i < sizeof($mySlaves); $i++)
    {
	$fileName = $fileRoot . $mySlaves[$i] . $fileExtension;
	touch($fileName);
    }
}

// Save the status of a master device in its .dat file.

function SaveTemperature($device, $act, $temp, $s)
{
   global $profile, $line, $debug, $debugString, $fileRoot, $fileExtension, $mySlaves, $iAmOn, 
	$boostTime, $times, $temps, $profileRoot, $unixTime, $delimiter, $timeDelimiter;

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
