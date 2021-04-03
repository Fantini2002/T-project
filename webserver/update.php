<?php
require_once("db.php");

$room = $_GET['room'];
$t = $_GET['t'];
$h = $_GET['h'];
$date = date("Y-m-d");
$time = date("H:i:s");

if(existTable($conn, "room$room")){
    //update table rooms
    mysqli_query($conn, "UPDATE rooms SET lastTemperature = $t, lastHumidity = $h, lastTime = '$date $time' WHERE number = $room");

    $db = mysqli_fetch_array(mysqli_query($conn, "SELECT minTemperature, maxTemperature FROM rooms WHERE number = $room"), MYSQLI_ASSOC);

    if($t < $db['minTemperature'])
        mysqli_query($conn, "UPDATE rooms SET minTemperature = $t, minHumidity = $h, minTime = '$time' WHERE number = $room");
    
    if($t > $db['maxTemperature'])
        mysqli_query($conn, "UPDATE rooms SET maxTemperature = $t, maxHumidity = $h, maxTime = '$time' WHERE number = $room");
    
    //add in room's table
    mysqli_query($conn, "INSERT INTO room$room(temperature, humidity, date) VALUES($t, $h, '$date $time')");   
}
else{
    //TODO
}


?>