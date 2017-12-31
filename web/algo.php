<?php
function getMySQLConn(){
	$servername = "mysql8.000webhost.com";
	$username = "a9718642_root";
	$password = "Daag13";
	$database = "a9718642_drm";

	// Create connection
	$conn = mysqli_connect($servername, $username, $password, $database)
		or sendError('Error connecting to MySQL server.');
	return $conn;
}
?>
<!DOCTYPE html>
<html>
<head>
	<title>Power Controller</title>
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
	<script>
		function execute(){
			var lt = document.getElementById("limitTxt").value;
			var url = 'http://drm.netne.net/algo.php?limit=' + lt;
			document.location.href=url;
		}
	</script>
</head>
<body>
<div class="container" style="width:70%">
  <br/>
  <div class="header clearfix">
    <nav>
      <ul class="nav nav-pills pull-right">
        <li role="presentation"><a href="http://drm.netne.net/display.php">Data Log</a></li>
        <li role="presentation"><a href="http://drm.netne.net/control.php">Control Panel</a></li>
      </ul>
    </nav>
    <h1 class="text-muted">Power Controller</h1>
  </div>
  <br/>
<?php
$conn = getMySQLConn();
if ($_GET['limit'] > 0) {
	$lim = intval($_GET['limit']);
	$fanSql = "select ApplianceName, Value from DATA_LOG where ApplianceName='fan' ORDER BY InsertTime DESC limit 0,1";
	$fanResult = $conn->query($fanSql);
	$fanVal = $fanResult->fetch_assoc();
	$fanInt = intval($fanVal["Value"]);
	
	$dryerSql = "select ApplianceName, Value from DATA_LOG where ApplianceName='dryer' ORDER BY InsertTime DESC limit 0,1";
	$dryerResult = $conn->query($dryerSql);
	$dryerVal = $dryerResult->fetch_assoc();
	$dryerInt = intval($dryerVal["Value"]);

	if ($fanInt < $dryerInt) { 
		if ($lim <= $fanInt) {
			//off fan
			$updateSql = "UPDATE DeviceStatus SET Status=0 WHERE DeviceName='fan'";
			$conn->query($updateSql);
		} else if ($lim <= $dryerInt) {
			//off dryer
			$updateSql = "UPDATE DeviceStatus SET Status=0 WHERE DeviceName='dryer'";
			$conn->query($updateSql);
		} else {
			//off both
			$updateSql = "UPDATE DeviceStatus SET Status=0 WHERE DeviceName='fan'";
			$conn->query($updateSql);
			$updateSql = "UPDATE DeviceStatus SET Status=0 WHERE DeviceName='dryer'";
			$conn->query($updateSql);
		}
	} else {
		if ($lim <= $dryerInt) {
			//off dryer
			$updateSql = "UPDATE DeviceStatus SET Status=0 WHERE DeviceName='dryer'";
			$conn->query($updateSql);
		} else if ($lim <= $fanInt) {
			//off fan
			$updateSql = "UPDATE DeviceStatus SET Status=0 WHERE DeviceName='fan'";
			$conn->query($updateSql);
		} else {
			//off both
			$updateSql = "UPDATE DeviceStatus SET Status=0 WHERE DeviceName='dryer'";
			$conn->query($updateSql);
			$updateSql = "UPDATE DeviceStatus SET Status=0 WHERE DeviceName='fan'";
			$conn->query($updateSql);
		}
	}
	header("Location: http://drm.netne.net/control.php");
	die();
}
$conn->close();
?>
Limit: <input id="limitTxt" type="text" class="form-control" placeholder="0" aria-describedby="sizing-addon1"/>
<br/>
<button type="button" onclick="execute()" class="btn btn-primary">Execute</button>
</div>
</body>
</html>
<?php
die();
?>