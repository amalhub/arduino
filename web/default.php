<?php
if(isMethod("post")){
	$contentType = $_SERVER["CONTENT_TYPE"];
	if(mb_strtolower($contentType) !== 'application/json') {
		setHttpStatus(400, 'Bad request');
        $response = json_encode(array(
            'status' => 'Only support application/json payload'
        ));
        echo $response;
        die();
	}
	$input = json_decode(file_get_contents('php://input'), true);
	$address = $input["address"];
	$applianceName = $input["applianceName"];
	$registerName = $input["registerName"];
	$value = $input["value"];

	// Create connection
	$conn = getMySQLConn();
	

	$sql = "INSERT INTO DATA_LOG (Address, ApplianceName, RegisterName, Value) VALUES ('" . $address . "', '" . $applianceName . "', '" . $registerName . "', " . $value . ")";
	$override = 0;
	if($address[0] === '1' ) {
		$override = 1;
	}
	$statusSql = "UPDATE DeviceStatus SET override=" . $override . " WHERE DeviceName='" . $applianceName . "'";

	if (($conn->query($sql) === TRUE) && ($conn->query($statusSql) === TRUE)) {
	    setHttpStatus(200, 'Success');
	    $response = json_encode(array(
            'status' => 'Record added successfully'
        ));
        echo $response;
	} else {
	    sendError("Error: " . $sql . " " . $conn->error);
	}
	$conn->close();
	die();
} else if (isMethod("get")) {
	$conn = getMySQLConn();
	$sql = "select Address, ApplianceName, RegisterName, Value, InsertTime from DATA_LOG";
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
		$prefix = "";
		$response = "[";
	    // Get data of each row
	    while($row = $result->fetch_assoc()) {
	        $response = $response . $prefix . "{\"address\": \"" . $row["Address"] . "\", \"applianceName\": \"" . $row["ApplianceName"] . "\", \"registerName\": \"" . $row["RegisterName"] . "\", \"value\": " . $row["Value"] . ", \"InsertTime\": \"" . $row["InsertTime"] . "\"}";
	        $prefix = ",";
	    }
	    $response = $response . "]";
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
    header('Content-Type: application/json');
}