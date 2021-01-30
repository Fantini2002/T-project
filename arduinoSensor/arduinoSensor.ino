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

/*------------------------
        BUTTON
------------------------*/
#define buttonPin 12
int displayScreen = 0; //variable to choose display screen

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
    displayLayout0();

    /*------------------------
        TEMPERATURE SENSOR
    ------------------------*/
    dht.begin();

    /*------------------------
            BUTTON
    ------------------------*/
    pinMode(buttonPin, INPUT);
}

void loop()
{
    //read from sensors
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    //button  setting
    bool buttonPressed = digitalRead(buttonPin);
    if(buttonPressed){
        displayScreen += 1;
        if(displayScreen>1) //numbers of screens
            displayScreen=0;
        Serial.println("Button pressed");
    }

    //display screens
    switch (displayScreen){
        case 0:
            displayLayout0();
            //display temperature
            display.setTextSize(3);
            display.setCursor(16, 16);
            display.print(temperature);
            //display humidity
            display.setTextSize(2);
            display.setCursor(16, 50);
            display.print(humidity);
            break;
        case 1: //just for example
            display.clearDisplay();
            display.setCursor(0, 0);
            display.setTextSize(1);
            display.print("Execution time");
            display.setTextSize(2);
            display.setCursor(0, 20);
            display.print(millis());
            display.display();
            break;
    }
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

void displayLayout0(){
    //function to set layout of screen 0
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