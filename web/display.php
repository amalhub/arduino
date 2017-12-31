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
	<title>Data Log</title>
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
		    document.body.innerHTML = document.body.innerHTML.replace(new RegExp(fan, 'g'), fanReplace);
		    document.body.innerHTML = document.body.innerHTML.replace(new RegExp(dryer, 'g'), dryerReplace);
		}
	</script>
</head>
<body onload="replaceScript();">
<script type="text/javascript">
	setTimeout(function(){ location.reload(); }, 15000);
</script>
<div class="container" style="width:75%">
  <br/>
  <div class="header clearfix">
    <nav>
      <ul class="nav nav-pills pull-right">
        <li role="presentation"><a href="http://drm.netne.net/control.php">Control Panel</a></li>
        <li role="presentation"><a href="http://drm.netne.net/algo.php">Power Controller</a></li>
      </ul>
    </nav>
    <h1 class="text-muted">Data Log</h1>
  </div>
  <br/>
<table class="table table-striped">
	<tr>
		<th>ID</th>
		<th>Address</th>
		<th>Appliance Name</th>
		<th>Register Name</th>
		<th>Value</th>
		<th>Time</th>
	</tr>
<?php
$conn = getMySQLConn();
$sql = "select ID, Address, ApplianceName, RegisterName, Value, InsertTime from DATA_LOG ORDER BY InsertTime DESC LIMIT 0,50";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    // Get data of each row
    while($row = $result->fetch_assoc()) {
?>
<tr>
<?php
		echo "<td>" . $row["ID"] . "</td>";
        echo "<td>" . $row["Address"] . "</td>";
        echo "<td>" . $row["ApplianceName"] . "</td>";
        echo "<td>AWATTHR</td>";
        echo "<td>" . $row["Value"] . "</td>";
        echo "<td>" . $row["InsertTime"] . "</td>";
?>
</tr>
<?php
    }
} 
$conn->close();
?>
</table>
<button onclick="document.location.href='http://drm.netne.net/push.php'" type="button" class="btn btn-primary">Data</button>
</div>
</body>
</html>
<?php
die();
?>