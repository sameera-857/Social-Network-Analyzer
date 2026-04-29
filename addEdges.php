<?php
$conn = new mysqli("localhost", "root", "", "sim_network");

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$userId   = $_POST['userId'];
$friendId = $_POST['friendId'];

$check = $conn->query("SELECT * FROM edges WHERE 
    (src='$userId' AND dest='$friendId') 
    OR 
    (src='$friendId' AND dest='$userId')");

if ($check->num_rows == 0) {
    $conn->query("INSERT INTO edges (src, dest) VALUES ('$userId', '$friendId')");
}

echo "OK";
?>