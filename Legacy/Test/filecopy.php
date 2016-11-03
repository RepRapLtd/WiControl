<?php
   copy(htmlspecialchars($_GET["from"]), htmlspecialchars($_GET["to"]));
   echo('File loaded');
?>
