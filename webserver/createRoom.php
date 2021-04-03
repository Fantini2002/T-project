<?php
$roomName = $argv[1];
$roomNumber = $argv[2];

require_once("db.php");

if(existTable($conn, "rooms")){
    //add in table rooms
    $addInRooms = mysqli_query($conn, "INSERT INTO rooms(name, number) VALUES('$roomName', '$roomNumber')");
    //create room's table
    $createRoomTable = mysqli_query($conn, "CREATE TABLE room$roomNumber(
        id INT AUTO_INCREMENT,
        date DATETIME,
        temperature FLOAT(4,2),
        humidity FLOAT(4,2),
        min BOOL,
        max BOOL,
        PRIMARY KEY(id)
    )");
    //control
    if($addInRooms && $createRoomTable)
        echo "Done!";
    else
        echo mysqli_error($conn);
        //TODO
}
else{
    echo "First you have creare table 'rooms'";
    //TODO
}

echo "\n";
?>