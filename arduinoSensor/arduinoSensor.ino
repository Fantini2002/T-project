#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <SoftwareSerial.h>

#define room  "Camera Alex"
#define roomNumber 01
#define server "192.168.2.61"

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
int displayScreen; //variable to choose display screen

/*------------------------
        Wi-Fi
------------------------*/
#define wifiTX 3
#define wifiRX 2
SoftwareSerial espSerial =  SoftwareSerial(wifiTX,wifiRX);
#include "private.h" //import wifi settings
char wifiReply[50];
char ip[16];

/*------------------------
        ERROR LED
------------------------*/
#define errorPin 5

/*------------------------
        CRON
------------------------*/
long previousMillis = 0;
long interval = 3600000;

void setup()
{
    Serial.begin(9600);

    /*------------------------
            ERROR LED
    ------------------------*/
    pinMode(errorPin, OUTPUT);

    /*------------------------
            Wi-Fi
    ------------------------*/
    espSerial.begin(9600);

    delay(2000);

    //reset
    espSerial.print(F("AT+RST\r\n"));
    getResponse(true, false);

    delay(2000);

    //change mode
    espSerial.print(F("AT+CWMODE=1\r\n"));
    getResponse(true, false);

    //connect to wifi
    espSerial.print(F("AT+CWJAP=\""));
    espSerial.print(F(wifiName));
    espSerial.print(F("\",\""));
    espSerial.print(F(wifiPassword));
    espSerial.print(F("\"\r\n"));
    getResponse(true, false);

    //ip
    espSerial.print(F("AT+CIFSR\r\n"));
    getResponse(true, true);

    bool memorize = false;
    int c = 0;
    for(int i=0; i<strlen(wifiReply); i++){
        if(wifiReply[i]=='S' && wifiReply[i+1]=='T' && wifiReply[i+2]=='A' && wifiReply[i+3]=='I' && wifiReply[i+4]=='P'){
            memorize = true;
        }

        if(memorize){
            if(wifiReply[i+8-1]!='"' && c<15){
                ip[c] = wifiReply[i+8-1];
                c++;
            }
            else
                memorize = false;
        }
    }

    //configure for single connections 
    espSerial.print(F("AT+CIPMUX=0\r\n"));
    getResponse(true, false);

    /*------------------------
            DISPLAY
    ------------------------*/
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        digitalWrite(errorPin, HIGH);
        for(;;); //if connection failed, stop
    }
    
    displayLayout0();
    displayScreen = 0;
    
    /*------------------------
        TEMPERATURE SENSOR
    ------------------------*/
    dht.begin();

    /*------------------------
            BUTTON
    ------------------------*/
    pinMode(buttonPin, INPUT);

    Serial.println(F("Ready!"));
}

void loop()
{
    //read from sensors
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    //button setting
    bool buttonPressed = digitalRead(buttonPin);
    if(buttonPressed){
        displayScreen += 1;
        if(displayScreen>1) //numbers of screens
            displayScreen=0;
        Serial.println(F("Button pressed"));
    }

    //display screens
    switch(displayScreen){
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
        case 1:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.setTextSize(1);
            display.print(F("IP Address"));
            display.setTextSize(2);
            display.setCursor(0, 20);
            display.print(ip);
            display.display();
            break;
    }
    display.display(); 

    //serial output
    Serial.print(temperature);
    Serial.print(F(" °C - "));
    Serial.print(humidity);
    Serial.println(F(" %"));

    //errors control
    if(isnan(humidity) || isnan(temperature)){
        //reading error
        Serial.println(F("Reading error"));

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(F("Reading error"));
        display.display();
    }
    
    //cron
    long currentMillis = millis();

    //Wi-Fi comunication
    
    if((currentMillis - previousMillis >= interval) && !isnan(humidity) && !isnan(temperature)){

        previousMillis = currentMillis;

        espSerial.print(F("AT+CIPSTART=\"TCP\",\""));
        espSerial.print(F(server));
        espSerial.print(F("\",80\r\n"));
        getResponse(true, false);
        
        espSerial.print(F("AT+CIPSEND="));
        espSerial.print(76); //length of GET request
        espSerial.print(F("\r\n"));
        getResponse(true, false);

        espSerial.print("GET /update.php?room=");
        espSerial.print(roomNumber);
        espSerial.print("&t=");
        espSerial.print(temperature);
        espSerial.print("&h=");
        espSerial.print(humidity);
        espSerial.print(" HTTP/1.0\r\n\r\n\r\nHost: ");
        espSerial.print(server);
        espSerial.print("\r\n\r\n\r\n");
        getResponse(false, false);

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
    display.print(F("T: "));
    display.setCursor(0, 54);
    display.print(F("H: "));

    display.setTextSize(1);
    display.setCursor(110, 23);
    display.cp437(true);
    display.write(167);
    display.print(F("C"));

    display.setTextSize(1);
    display.setCursor(80, 54);
    display.print(F("%"));
}

void getResponse(bool control, bool memorize){
    //bool control: true wait "OK"; false dont't wait
    //bool memorize: true store message in wifiReply; false not
    long int time = millis();
    bool success = false;
    short c = 0;
    short wait;
    if(control)
        wait = 10000;
    else
        wait = 5000;
    while((time + wait) > millis()){
        char text[4];
        while(espSerial.available()){
            text[0] = text[1];
            text[1] = text[2];
            text[2] = text[3];
            text[3] = espSerial.read();
            Serial.print(text[3]);

            if(memorize && c<50){
                wifiReply[c] = text[3];
                c++;
            }

            if(control && text[0]=='O' && text[1]=='K' && text[2]=='\r' && text[3]=='\n')
                success = true;
            
        }
        if(success)
            break;
    }
    if(control && !success){
        Serial.println(F("\n---ERROR---"));
        digitalWrite(errorPin, HIGH);
    }
    Serial.println(F("\n------------------------------"));
    delay(100);
}
