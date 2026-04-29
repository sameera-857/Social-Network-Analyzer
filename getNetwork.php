<?php
header('Content-Type: application/json');

$conn = new mysqli("localhost", "root", "", "sim_network");

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$nodes = [];
$edges = [];

$result = $conn->query("SELECT * FROM nodes");
while ($row = $result->fetch_assoc()) {
    $nodes[] = $row;
}

$result = $conn->query("SELECT * FROM edges");
while ($row = $result->fetch_assoc()) {
    $edges[] = $row;
}

echo json_encode([
    "nodes" => $nodes,
    "edges" => $edges
]);
?>