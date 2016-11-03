<?php

      $dir = $_GET["dir"];
$results_array = array();

if (is_dir($dir))
{
        if ($handle = opendir($dir))
        {
                //Notice the parentheses I added:
                while(($file = readdir($handle)) !== FALSE)
                {
                        $results_array[] = $file;
                }
                closedir($handle);
        }
}

//Output findings
foreach($results_array as $value)
{
    if($value != '.' && $value != '..')
    echo $value . "\n";
}
?>
