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
	<title>Control Panel</title>
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
	<script>
		function httpGet(theUrl)
		{
		    var xmlHttp = new XMLHttpRequest();
		    xmlHttp.open( "GET", theUrl, false ); // false for synchronous request
		    xmlHttp.send( null );
		    return xmlHttp.responseText.split("\n")[0];
		}
		function replaceScript() {
			var ss = httpGet('http://drm.netne.net/device.php');
			var fanReplace = ss.split(',')[1];
			var dryerReplace = ss.split(',')[0];
		    var fan = 'fan';
		    var dryer = 'dryer';
		    document.body.innerHTML = document.body.innerHTML.replace(fan, fanReplace);
		    document.body.innerHTML = document.body.innerHTML.replace(dryer, dryerReplace);
		}
	</script>
</head>
<body onload="replaceScript();">
<script type="text/javascript">
	setTimeout(function(){ document.location.href='http://drm.netne.net/control.php'; }, 15000);
</script>
<div class="container" style="width:70%">
  <br/>
  <div class="header clearfix">
    <nav>
      <ul class="nav nav-pills pull-right">
      	<li role="presentation"><a href="http://drm.netne.net/display.php">Data Log</a></li>
        <li role="presentation"><a href="http://drm.netne.net/algo.php">Power Controller</a></li>
      </ul>
    </nav>
    <h1 class="text-muted">Control Panel</h1>
  </div>
  <br/>
<table class="table table-striped">
	<tr>
		<th>Device Name</th>
		<th>Status</th>
		<th>Overridden</th>
		<th>Action</th>
	</tr>
<?php
$conn = getMySQLConn();
if ($_GET['switch']) {
	$switchDevice = $_GET['switch'];
	$switchAction = $_GET['action'];
	$updateSql = "UPDATE DeviceStatus SET Status=" . $switchAction . " WHERE DeviceName='" . $switchDevice . "'";
	$conn->query($updateSql);
	header("Location: http://drm.netne.net/control.php");
	die();
}
$sql = "select DeviceName, Status, override from DeviceStatus";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    // Get data of each row
    while($row = $result->fetch_assoc()) {
?>
<tr>
<?php
	$switchAction = 1;
	echo "<td>" . $row["DeviceName"] . "</td>";
	if (intval($row["Status"]) === 1) {
		echo "<td><font color='green'>ON</font></td>";
		$switchAction = 0;
	} else {
		echo "<td><font color='red'>OFF</font></td>";
	}
	if (intval($row["override"]) === 1) {
		echo "<td><font color='red'>Yes</font></td>";
	} else {
		echo "<td><font color='orange'>No</font></td>";
	}
    echo "<td><button onclick='document.location.href=\"http://drm.netne.net/control.php?switch=" . 
    		$row["DeviceName"] . "&action=" . $switchAction . "\"' type='button' class='btn btn-primary'>Switch</button></td>";
?>
</tr>
<?php
    }
} 
$conn->close();
?>
</table>
</div>
</body>
</html>
<?php
die();
?>