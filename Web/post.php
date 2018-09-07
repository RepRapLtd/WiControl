<?php

$name = "";
$postData = file_get_contents('php://input');
if ($postData != '')
{
   if($_GET["file"] == "")
   {
       $name = $name . htmlspecialchars($_GET["building"]) . "-profile-" . htmlspecialchars($_GET["day"]) . ".dat";
       file_put_contents("" . $name, $postData);
   } else
   {
       $name = $name . htmlspecialchars($_GET["building"]) . "-Profiles/" . htmlspecialchars($_GET["file"]) . ".dat";
       file_put_contents("" . $name ,$postData);
   }
   echo('Saved to ' . $name);
} else
  echo('No post data - not saved');
?>

