CREATE TABLE rooms(
    id INT AUTO_INCREMENT,
    name VARCHAR(20),
    number INT(2),
    minTemperature FLOAT(4,2) DEFAULT 60.00,
    minHumidity FLOAT(4,2),
    minTime TIME,
    maxTemperature FLOAT(4,2) DEFAULT -60.00,
    maxHumidity FLOAT(4,2),
    maxTime TIME,
    lastTemperature FLOAT(4,2),
    lastHumidity FLOAT(4,2),
    lastTime DATETIME,
    PRIMARY KEY(id),
    UNIQUE KEY number (number)
)