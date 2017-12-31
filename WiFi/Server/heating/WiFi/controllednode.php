<html>
<head><title>controllednode</title></head>
<body><?php
parse_str($_SERVER['QUERY_STRING']);
//echo $location . "\n";
echo "Temp: " . $temperature . ", ";

$fileRoot = "Data/";
$fileName = $fileRoot . $location . ".dat";
$fileContents = file_get_contents($fileName);
echo "file: " . $fileContents . "\n";

$fileHandle = fopen($fileName, "w");
fwrite($fileHandle, $temperature);
fclose($fileHandle);
?>
</body>
</html>
