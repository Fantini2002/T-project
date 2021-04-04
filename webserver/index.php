<?php include_once("server/db.php");

$result = mysqli_query($conn, "SELECT * FROM rooms");
?>

<!DOCTYPE html>
<html lang="it">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="CSS/style.css" rel="stylesheet" type="text/css"/>
    <title>Temperature system</title>
</head>
<body>
    <div class="cover">
        <div class="head">
            <h1>Temperature system</h1>
        </div>
        <div class="cards">
            <?php
                while($row = mysqli_fetch_array($result, MYSQLI_ASSOC)){
            ?>
            <div class="card">
                <div class="row1">
                    <h2 class="title"><?php echo $row['name']; ?></h2>
                    <p class="time"><?php echo date("H:i:s",strtotime($row['lastTime'])); ?></p>
                    <p class="temperature">T: <?php echo $row['lastTemperature']; ?> °C</p>
                    <p class="humidity">H: <?php echo $row['lastHumidity']; ?> %</p>
                </div>
                <div class="row2">
                    <div class="min">
                        <h4>Min</h4>
                        <p class="time"><?php echo date("H:i:s",strtotime($row['minTime'])); ?></p>
                        <p class="temperature">T: <?php echo $row['minTemperature']; ?> °C</p>
                        <p class="humidity">H: <?php echo $row['minHumidity']; ?> %</p>
                    </div>
                    <div class="max">
                        <h4>Max</h4>
                        <p class="time"><?php echo date("H:i:s",strtotime($row['maxTime'])); ?></p>
                        <p class="temperature">T: <?php echo $row['maxTemperature']; ?> °C</p>
                        <p class="humidity">H: <?php echo $row['maxHumidity']; ?> %</p>
                    </div>
                </div>
            </div>
            <?php
                }
            ?>
        </div>
    </div>
</body>
</html>