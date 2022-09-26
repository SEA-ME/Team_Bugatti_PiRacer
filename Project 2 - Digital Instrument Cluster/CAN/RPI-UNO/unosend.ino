// demo: CAN-BUS Shield, send data
// loovee@seeed.cc
 
#include <mcp_can.h>
#include <SPI.h>
#include <DHT.h> //Library for using DHT sensor
#define DHTPIN A0
#define DHTTYPE DHT11

const int HOLE = 2; // digital input
unsigned long millisBefore;
volatile int cnt; // count holes
float rpm = 2;
float spd = 3;

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;
 
MCP_CAN CAN(SPI_CS_PIN);  // Set CS pin
DHT dht(DHTPIN, DHTTYPE); //initialize object dht for class DHT with DHT pin with STM32 and DHT type as DHT11 

void setup()
{
    Serial.begin(115200);
    SPI.begin(); //Begins SPI communication
    dht.begin(); //Begins to read temperature & humidity sensor value
 
    pinMode (HOLE, INPUT);
    attachInterrupt(digitalPinToInterrupt(HOLE), count, FALLING);
    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}
 
unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void loop()
{
    int h = dht.readHumidity(); //Gets Humidity value
    int t = dht.readTemperature(); //Gets Temperature value

    // Caculate RPM
    if (millis() - millisBefore > 1000){
      rpm = (cnt/12.0) * 60;
      cnt = 0;
      millisBefore = millis();
    }
   
    stmp[0] = h;  // Humidity
    stmp[1] = t;  // Temperature
    stmp[2] = rpm; // RPM
    stmp[3] = spd; // Speed

    CAN.sendMsgBuf(0x00, 0, 8, stmp);
    delay(100);                       // send data per 100ms
}

void count()
{
    cnt++;
}
// END FILE
