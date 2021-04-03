<?php

require_once("private.php");

//connect to db
$conn = new mysqli("localhost", $dbUser, $dbPsw, "temperature", 3306);
if ($conn->connect_errno) {
    error_log("SQL Error:Failed to connect to MySQL: (" . $conn->connect_errno . ") " . $conn->connect_error, 0);
    die;
}

function existTable($conn, $tabella){
    $query = "SHOW TABLES LIKE '".$tabella."'";
    if(mysqli_num_rows(mysqli_query($conn, $query)) > 0) return true;
    else return false;
}

?>