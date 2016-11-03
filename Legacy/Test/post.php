<?php

if (isset($HTTP_RAW_POST_DATA))
{
   if($_GET["file"] == "")
       file_put_contents("" . htmlspecialchars($_GET["building"]) . "-profile-" . htmlspecialchars($_GET["day"]) . ".dat", $HTTP_RAW_POST_DATA);
   else
	file_put_contents("" . htmlspecialchars($_GET["building"]) . "-Profiles/" . htmlspecialchars($_GET["file"]) . ".dat",$HTTP_RAW_POST_DATA);
   echo('Saved');
}
?>
