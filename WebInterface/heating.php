<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<meta content="text/html; charset=ISO-8859-1" http-equiv="content-type">
<title>West End Stables Heating Control</title>
</head>

<body>

<?php
$file = file_get_contents('../workshop-profile.dat', FILE_USE_INCLUDE_PATH);
?>

<div style="text-align: center;"><big style="font-weight: bold;"><big>West End Stables Heating Control</big></big><small><small><small><span style="font-weight: bold;"><br>
</span></small></small></small>
<div style="text-align: left;"><small><br>
<textarea cols="80" rows="20" name="profile-file">
<?php echo $file; ?>
</textarea><br>
</div>
</div>
</body>
</html>
