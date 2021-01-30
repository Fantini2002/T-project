#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define room  "Camera Alex"

/*------------------------
        DISPLAY
------------------------*/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*------------------------
    TEMPERATURE SENSOR
------------------------*/
#define DHTPIN 8
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);

	/*------------------------
            DISPLAY
    ------------------------*/
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); //if connection failed, stop
    }
    displayLayout();

    /*------------------------
        TEMPERATURE SENSOR
    ------------------------*/
    dht.begin();
}

void loop()
{
    //read from sensors
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();


    //display output
    displayLayout();
    //temperature
    display.setTextSize(3);
    display.setCursor(16, 16);
    display.print(temperature);
    //humidity
    display.setTextSize(2);
    display.setCursor(16, 50);
    display.print(humidity);
    display.display(); 

    //serial output
    Serial.print(temperature);
    Serial.print(" °C - ");
    Serial.print(humidity);
    Serial.println(" %");

    //errors control
    if(isnan(humidity) || isnan(temperature)){
        //reading error
        
        Serial.println("Reading error");

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Reading error");
        display.display();
    }

    delay(2000);
}

void displayLayout(){
    //function to set basic display layout
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(room);

    display.setTextSize(1);
    display.setCursor(0, 23);
    display.print("T: ");
    display.setCursor(0, 54);
    display.print("H: ");

    display.setTextSize(1);
    display.setCursor(110, 23);
    display.cp437(true);
    display.write(167);
    display.print("C");

    display.setTextSize(1);
    display.setCursor(80, 54);
    display.print("%");
}