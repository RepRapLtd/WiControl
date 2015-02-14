<!DOCTYPE HTML>
<html>
<body style="background-color: rgb(225,225,225)">


<?php
$infile = file_get_contents('../workshop-profile.dat', FILE_USE_INCLUDE_PATH);
?>

West End Stables: Workshop Heating Settings
<br>

<form name="savefile" method="post" action=""><br/>
<textarea rows="16" cols="100" name="textdata">
<?php echo $infile; ?>
</textarea><br/>
<input type="submit" name="submitsave" value="Save heating settings">
</form>

<br/>

<hr>

<form method="post" action="set" name="settings">
Boiler device: <textarea cols="3" rows="1" name="boiler"></textarea>
<br>
<br>
Office - sensor <textarea cols="3" rows="1" name="office-sensor"></textarea>, switch <textarea cols="3" rows="1" name="office-switch"></textarea>
<table style="text-align: left; width: 684px; height: 62px;" border="1" cellpadding="2" cellspacing="2">
<tbody>
<tr align="right">
<td style="vertical-align: top;">Time (hh:mm)</td>
<td style="vertical-align: top; text-align: center;"><input type="time" name="office_time_1"></td>
<td style="vertical-align: top;"></td>
<td style="vertical-align: top;"></td>
<td style="vertical-align: top;"></td>
</tr>
<tr>
<td style="vertical-align: top; text-align: right;">Temperature(<sup>o</sup>C)</td>
<td style="vertical-align: top; text-align: center;"><textarea cols="3" rows="1" name="office_temp_1"></textarea></td>
<td style="vertical-align: top;"></td>
<td style="vertical-align: top;"></td>
<td style="vertical-align: top;"></td>
</tr>
</tbody>
</table>
<br>
</form>

<?php
    if (isset($_POST))
	 {
        if ($_POST['submitsave'] == "Save heating settings") 
		  {
            $text = $_POST["textdata"];
            file_put_contents("../workshop-profile.dat", $old . $text);
        }
    }
?>
</body>
</html>

