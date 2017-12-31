<?php
if (isMethod("get")) {
	$conn = getMySQLConn();
	$sql = "select DeviceName, Status from DeviceStatus";
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
		$prefix = "";
		$response = "";
	    // Get data of each row
	    while($row = $result->fetch_assoc()) {
	        $response = $response . $prefix . $row["DeviceName"] . ":" . $row["Status"];
	        $prefix = ",";
	    }
	    setHttpStatus(200, 'Success');
	    echo $response;
	} else {
	    setHttpStatus(200, 'Success');
	    $response = json_encode(array(
            'status' => '0 records'
        ));
        echo $response;
	}
	$conn->close();
	die();
} else {
	setHttpStatus(200, 'Success');
    $response = json_encode(array(
        'status' => 'It\'s working!!!'
    ));
    echo $response;
	die();
}

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

function sendError($error) {
	setHttpStatus(500, 'Internal server error');
    $response = json_encode(array(
        'reason' => $error
    ));
    echo $response;
    die();
}

function isMethod($check_method){
    $method = $_SERVER['REQUEST_METHOD'];
    if (mb_strtolower($method) != $check_method) {
        return false;
    }
    return true;
}

function setHttpStatus($code, $message) {
	$protocol = (isset($_SERVER['SERVER_PROTOCOL']) ? $_SERVER['SERVER_PROTOCOL'] : 'HTTP/1.0');
    header($protocol . ' ' . $code . ' ' . $message);
    header('Content-Type: text/plain');
}