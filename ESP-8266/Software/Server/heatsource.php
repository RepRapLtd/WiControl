<html>
<head><title>heatsource</title></head>
<body><?php

/*
 * Program to turn on a slave load from a remote request directly by touching
 * its .dat file.
 *  
 * Adrian Bowyer
 * RepRap Ltd
 * http://reprapltd.com
 * 
 * 15 August 2019
 *
 * Licence: GPL
 *
 * The general form of a request is (N.B. "/" characters should be in the strings):
 * 
 * GET http://currentServer/pageRoot/page/?messageString
 * 
 * where messageString is something like
 * 
 * unit=1&load=0[&debugOn=1]
 */

// Note to self: don't forget "\n" means newline; '\n' means \n...

// Debugging

ini_set('display_errors', 'On');
error_reporting(E_ALL | E_STRICT);
$debug = false;

// Append debugging info to this string

$debugString = '';

// Where the files are.

$fileRoot = '/Data/';
$profileText = '-profile-';
$fileExtension = '.dat';
$units = 'units';

// Special characters

$delimiter = '*';
$timeDelimiter = ':';


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


// Touch the .dat file requested.  

function TouchFile($house, $device)
{
include 'globals.php';

	$fileName = $house . $fileRoot . $device . $fileExtension;
	touch($fileName);
	echo "Touched";
	if($debug)
		$debugString = $debugString . ' ' . $fileName . "<br>";
	else
		echo "<br>";
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
// This should be: GET http://currentServer/pageRoot/page/?unit=xx&load=yy[&debugOn=1]

parse_str($_SERVER['QUERY_STRING']);

if(!strlen($unit) || !strlen($load))
	exit('ERROR - $unit or $load undefined');

$debug = !empty($debugOn);

ReadUnit($unit, $load);

TouchFile($building, $location);

// Send helpful info in the HTML <body> as well if we are debugging

if($debug)
{
  echo $debugString;
}


?></body>
</html>

