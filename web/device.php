<?php
$servername = "mysql8.000webhost.com";
$username = "a9718642_root";
$password = "Daag13";
$database = "a9718642_drm";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $database)
	or sendError('Error connecting to MySQL server.');

$sql = "select fan, dryer from DeviceName";
$result = $conn->query($sql);
$row = $result->fetch_assoc();

echo $row["dryer"] . "," . $row["fan"];
die();
?>