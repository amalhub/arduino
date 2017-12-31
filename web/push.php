<?php
$servername = "mysql8.000webhost.com";
$username = "a9718642_root";
$password = "Daag13";
$database = "a9718642_drm";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $database)
	or sendError('Error connecting to MySQL server.');

$sql = "select Address, Value from DATA_LOG where ApplianceName='fan' and Value > 0 ORDER BY InsertTime DESC limit 0,1";
$result = $conn->query($sql);
$row = $result->fetch_assoc();
$fanSql = "insert into DATA_LOG (Address, ApplianceName, RegisterName, Value) values ('" . $row['Address'] . "', 'fan', 'AWATTHR', " . $row['Value'] . ")";

$sql = "select Address, Value from DATA_LOG where ApplianceName='dryer' and Value > 0 ORDER BY InsertTime DESC limit 0,1";
$result = $conn->query($sql);
$row = $result->fetch_assoc();
$dryerSql = "insert into DATA_LOG (Address, ApplianceName, RegisterName, Value) values ('" . $row['Address'] . "', 'dryer', 'AWATTHR', " . $row['Value'] . ")";
$result = $conn->query($fanSql);
$result = $conn->query($dryerSql);

header("Location: http://drm.netne.net/display.php");
die();
?>