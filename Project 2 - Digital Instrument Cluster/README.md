# Arduino Setup
# Project 2 - Digital Instrument Cluster

- Arduino Nano ( humidity, temperature )
- transmit humidity, temperature to CAN
- show on LCD connected to Arduino Uno
- show the work ( room temperature )
- RPM, Speed
- battery level
- Implement PRND indicators

<br/>

### Arduino Setting

1. Donwload Arduino IDE
    
    ```bash
    sudo snap install arduino
    arduino
    ```
    
    <details>
    <summary>error</summary>
    
    ![Untitled](https://user-images.githubusercontent.com/111988634/189864114-eeb157e5-42cf-46af-9b59-f848882e8b40.png)

    ```bash
    sudo usermod -a -G dialout seame-three
    sudo reboot
    ```
    </details>
    <br/>
2. Connect Arduino Uno and MH-Sensor-Series
    
    ```arduino
    const int IN_A0 = A1; // analog input
    const int IN_D0 = 8; // digital input
    void setup() {
      Serial.begin(9600);
      pinMode (IN_A0, INPUT);
      pinMode (IN_D0, INPUT);
     
    }
    int value_A0;
    bool value_D0;
    void loop() {
      value_A0 = analogRead(IN_A0); // reads the analog input from the IR distance sensor
      value_D0 = digitalRead(IN_D0);// reads the digital input from the IR distance sensor
      Serial.print(" Analogue = "); 
      Serial.print(value_A0);
      Serial.print("\t Digital ="); 
      Serial.println(value_D0);
      delay(100);
    }
    ```
<br/>

### Arduino Uno and MH-Sensor-Series

![nse-8050088173945518711-6769](https://user-images.githubusercontent.com/111988634/190005206-37e48b0b-eba8-4a11-9f84-69b67825110b.jpg)


- Connect Arduino Uno and MH-Sensor-Series

| MH-Sensor-Series | Uno |
| --- | --- |
| VCC | 5V |
| GND | GND |
| D0 | 8 |
| A0  | A0 |

```arduino
const int IN_A0 = A1; // analog input
const int IN_D0 = 8; // digital input
void setup() {
  Serial.begin(9600);
  pinMode (IN_A0, INPUT);
  pinMode (IN_D0, INPUT);
 
}
int value_A0;
bool value_D0;
void loop() {
  value_A0 = analogRead(IN_A0); // reads the analog input from the IR distance sensor
  value_D0 = digitalRead(IN_D0);// reads the digital input from the IR distance sensor
  Serial.print(" Analogue = "); 
  Serial.print(value_A0);
  Serial.print("\t Digital ="); 
  Serial.println(value_D0);
  delay(100);
}
```

---
<br/>


### Arduino Uno, Nano, DHT11 Sensor ( Temperature and Humidity )

- Circuit

![unonano](https://user-images.githubusercontent.com/111988634/190004955-ea62c788-894f-4662-ad9f-57140d833e26.png)

![nse-5415125685950099462-6771](https://user-images.githubusercontent.com/111988634/190005137-51d631fb-e49a-4db0-86e8-eb0733afeda0.jpg)


| Arduino Uno | CAN Module MCP2515 |
| --- | --- |
| VCC | 5V |
| GND | GND |
| CS | 9 |
| SO | 11 |
| SI | 10 |
| SCK | 13 |


| CAN Module MCP2515 | CAN Module MCP2515 |
| --- | --- |
| CAN H | CAN H |
| CAN L | CAN L |


| Arduino Nano | CAN Module MCP2515 |
| --- | --- |
| VCC | 5V |
| GND | GND |
| CS | 9 |
| SO | 11 |
| SI | 10 |
| SCK | 13 |


| DHT11 | Arduino Nano  |
| --- | --- |
| GND | GND |
| DATA | A0 |
| VCC | 5V |

- receiver ( Uno )
    
    ```arduino
    #include <SPI.h> //Library for using SPI Communication 
    #include <mcp2515.h> //Library for using CAN Communication
    
    struct can_frame canMsg; 
    MCP2515 mcp2515(10); // SPI CS Pin 10 
    
    void setup() {
      SPI.begin();   //Begins SPI communication
      Serial.begin(9600); //Begins Serial Communication at 9600 baud rate 
      mcp2515.reset();                          
      mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
      mcp2515.setNormalMode();  //Sets CAN at normal mode
    }
    
    void loop(){
     if ((mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) && (canMsg.can_id == 0x036)){
         int x = canMsg.data[0];         
         int y = canMsg.data[1];  
         Serial.print("canMsg.data[0]: ");
         Serial.print(x);
         Serial.print(" canMsg.data[1]: ");
         Serial.println(y);      
       }
    }
    ```
    

- transmitter ( Nano )
    
    ```arduino
    #include <SPI.h> //Library for using SPI Communication
    #include <mcp2515.h> //Library for using CAN Communication
    #include <DHT.h> //Library for using DHT sensor
    #define DHTPIN A0
    #define DHTTYPE DHT11
    
    struct can_frame canMsg;
    MCP2515 mcp2515(10);
    DHT dht(DHTPIN, DHTTYPE); //initialize object dht for class DHT with DHT pin with STM32 and DHT type as DHT11
    
    void setup(){
      while (!Serial);
      Serial.begin(9600);
      SPI.begin(); //Begins SPI communication
      dht.begin(); //Begins to read temperature & humidity sensor value
      mcp2515.reset();
      mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
      mcp2515.setNormalMode();
    }
    
    void loop(){
      int h = dht.readHumidity(); //Gets Humidity value
      int t = dht.readTemperature(); //Gets Temperature value
      canMsg.can_id = 0x036; //CAN id as 0x036
      canMsg.can_dlc = 8; //CAN data length as 8
      canMsg.data[0] = h; //Update humidity value in [0]
      canMsg.data[1] = t; //Update temperature value in [1]
      canMsg.data[2] = 0x00; //Rest all with 0
      canMsg.data[3] = 0x00;
      canMsg.data[4] = 0x00;
      canMsg.data[5] = 0x00;
      canMsg.data[6] = 0x00;
      canMsg.data[7] = 0x00;
      mcp2515.sendMessage(&canMsg); //Sends the CAN message
      Serial.print("humidiy: ");
      Serial.print(h);
      Serial.print(" temperature: ");
      Serial.println(t);
      delay(1000);
    }
    ```