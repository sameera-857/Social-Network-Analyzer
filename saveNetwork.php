<?php
header('Content-Type: application/json');

// DB connection
$conn = new mysqli('localhost','root','','sim_network');

if ($conn->connect_error) {
    echo json_encode(["status"=>"error","message"=>"DB connection failed"]);
    exit();
}

// Get JSON data
$data = json_decode(file_get_contents("php://input"), true);

if (!$data) {
    echo json_encode(["status"=>"error","message"=>"No data received"]);
    exit();
}

// Clear old data
$conn->query("DELETE FROM nodes");
$conn->query("DELETE FROM edges");

// Insert nodes
foreach ($data['nodes'] as $node) {
    $id = intval($node['id']);
    $name = $conn->real_escape_string($node['name']);
    $interest = $conn->real_escape_string($node['interest']);

    $conn->query("INSERT INTO nodes (id, interest, name) VALUES ($id, '$interest', '$name')");
}

// Insert edges
foreach ($data['edges'] as $edge) {
    $src = intval($edge['src']);
    $dest = intval($edge['dest']);

    $conn->query("INSERT INTO edges (src, dest) VALUES ($src, $dest)");
}

echo json_encode(["status"=>"success"]);
?>