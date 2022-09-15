# Arduino Setup

- Arduino Nano ( humidity, temperature )
- transmit humidity, temperature to CAN
- show on LCD connected to Arduino Uno
- show the work ( room temperature )
- RPM, Speed
- battery level
- Implement PRND indicators

<br/>

- Contents

    [Arduino setting](#arduino-setting)

    [Arduino Uno, MH-Sensor-Series](#arduino-uno-mh-sensor-series)

    [Arduino Uno, Nano, DHT11 Sensor ( Temperature and Humidity )](https://github.com/SEA-ME/Team_Bugatti_PiRacer/tree/main/Project%202%20-%20Digital%20Instrument%20Cluster#arduino-uno-nano-dht11-sensor--temperature-and-humidity-)

    [Arduino Uno, Raspberry PI, CAN module ( MCP 2515 )](#arduino-uno-raspberry-pi-can-module--mcp-2515)

<br/>

## Arduino Setting

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

## Arduino Uno, MH-Sensor-Series

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


## Arduino Uno, Nano, DHT11 Sensor ( Temperature and Humidity )

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

- Receiver ( Uno )
    
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
    

- Transmitter ( Nano )
    
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

<br/>

## Arduino Uno, Raspberry PI, CAN module ( MCP 2515 )

<img width="737" alt="image" src="https://user-images.githubusercontent.com/111988634/190488739-d6cc93b2-57a1-4e00-8630-2188dca2b012.png">


| PiRacer Pro expansion board | Raspberry PI |
| --- | --- |
| SCL | GPIO3 |
| SDA | GPIO2 |
| 3V3 | 3V3 |
| GND | GND |
| 5V | 5V |
| 5V | None |

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

1. Open config file and add these lines
    
    ```bash
    sudo nano /boot/config.txt
    ```
    
    ```bash
    dtparam=spi=on
    dtoverlay=mcp2515-can0,oscillator=8000000,interrupt=25
    ```
    
2. Download can utils and reboot
    
    ```bash
    sudo apt-get install can-utils
    sudo reboot
    ```
    
3. You can check by these commands
    
    ```bash
    (env) bugatti@bugatti:~ $ ls /sys/bus/spi/devices/spi0.0
    driver  driver_override  modalias  net  of_node  power  statistics  subsystem  uevent
    ```
    
    ```bash
    (env) bugatti@bugatti:~ $ ls /sys/bus/spi/devices/spi0.0/net
    can0
    ```
    
    ```bash
    (env) bugatti@bugatti:~ $ ls /sys/bus/spi/devices/spi0.0/net/can0/
    addr_assign_type  broadcast        carrier_down_count  dev_port  duplex             ifalias  link_mode         napi_defer_hard_irqs  phys_port_id    power       speed       testing       uevent
    addr_len          carrier          carrier_up_count    device    flags              ifindex  mtu               netdev_group          phys_port_name  proto_down  statistics  tx_queue_len
    address           carrier_changes  dev_id              dormant   gro_flush_timeout  iflink   name_assign_type  operstate             phys_switch_id  queues      subsystem   type
    ```
    
4. Set up the CAN Interface
    
    ```bash
    sudo ip link set can0 up type can bitrate 125000
    ```
    
    ```bash
    (env) bugatti@bugatti:~ $ sudo ifconfig
    can0: flags=193<UP,RUNNING,NOARP>  mtu 16
            unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 10  (UNSPEC)
            RX packets 0  bytes 0 (0.0 B)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 0  bytes 0 (0.0 B)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
    ```
    
5. Install Raspberry PI Python CAN
    
    ```bash
    pip3 install python-can
    ```
