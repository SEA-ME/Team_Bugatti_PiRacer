# CAN Communication
<br/>

- Contents  
    [Arduino setting](#arduino-setting)  
    [Arduino Uno, MH-Sensor-Series](#arduino-uno-mh-sensor-series)  
    [Arduino Uno, Nano, DHT11 Sensor ( Temperature and Humidity )](#arduino-uno-nano-dht11-sensor--temperature-and-humidity)  
    [Arduino Uno, Raspberry PI, CAN module ( MCP 2515 )](#arduino-uno-raspberry-pi-can-module--mcp-2515)  
    [Arduino Uno, Raspberry PI, CAN Shields](#arduino-uno-raspberry-pi-can-shields)

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

<br/>

## Arduino Uno, Raspberry PI, CAN Shields
![nse-3031638833028511961-6963](https://user-images.githubusercontent.com/111988634/192337689-a812475f-3fe9-4a5a-9b55-a79576cfffff.jpg)
| PiRacer Pro expansion board | Raspberry PI  CAN shield |
| --- | --- |
| SCL | SCL ( Don't connect with GND ) |
| SDA | SDA ( Don't connect with GND ) |
| 3V3 | 3V3 ( Soldering )  |
| GND | GND |
| 5V | 5V |
| 5V | 5V |


1. **Soldering 3V3 to Raspberry PI CAN Shield**

1. Open config.txt file
    
    ```bash
    sudo nano /boot/config.txt
    ```
    
2. Add the following line at the end of the file
    
    ```bash
    dtoverlay=seeed-can-fd-hat-v2
    ```
    
3. Reboot RPI
    
    ```bash
    sudo reboot
    ```
    
4. Check the kernel log to see if CAN-BUS HAT was initialized successfully. You will also see can0
and can1 appear in the list of ifconfig results
    
    ```bash
    (env) bugatti@bugatti:~ $ dmesg | grep spi
    [    5.682114] spi_master spi0: will run message pump with realtime priority
    [    5.690279] mcp251xfd spi0.1 can0: MCP2518FD rev0.0 (-RX_INT -MAB_NO_WARN +CRC_REG +CRC_RX +CRC_TX +ECC -HD c:40.00MHz m:20.00MHz r:17.00MHz e:16.66MHz) successfully initialized.
    [    5.702068] mcp251xfd spi0.0 can1: MCP2518FD rev0.0 (-RX_INT -MAB_NO_WARN +CRC_REG +CRC_RX +CRC_TX +ECC -HD c:40.00MHz m:20.00MHz r:17.00MHz e:16.66MHz) successfully initialized.
    ```
    
    ```bash
    (env) bugatti@bugatti:~ $ ifconfig -a
    can0: flags=128<NOARP>  mtu 16
            unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 10  (UNSPEC)
            RX packets 0  bytes 0 (0.0 B)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 0  bytes 0 (0.0 B)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
            device interrupt 67  
    
    can1: flags=128<NOARP>  mtu 16
            unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 10  (UNSPEC)
            RX packets 0  bytes 0 (0.0 B)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 0  bytes 0 (0.0 B)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
            device interrupt 68  
    
    eth0: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
            ether e4:5f:01:64:c0:ca  txqueuelen 1000  (Ethernet)
            RX packets 0  bytes 0 (0.0 B)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 0  bytes 0 (0.0 B)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
    
    lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
            inet 127.0.0.1  netmask 255.0.0.0
            inet6 ::1  prefixlen 128  scopeid 0x10<host>
            loop  txqueuelen 1000  (Local Loopback)
            RX packets 0  bytes 0 (0.0 B)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 0  bytes 0 (0.0 B)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
    
    wlan0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
            inet 192.168.0.102  netmask 255.255.255.0  broadcast 192.168.0.255
            inet6 fe80::63b1:930e:fdf6:6fa7  prefixlen 64  scopeid 0x20<link>
            ether e4:5f:01:64:c0:cb  txqueuelen 1000  (Ethernet)
            RX packets 608  bytes 49980 (48.8 KiB)
            RX errors 0  dropped 0  overruns 0  frame 0
            TX packets 476  bytes 70356 (68.7 KiB)
            TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
    ```
    
5. Raspberry pi setting and and you can use can-util to receive
    
    ```bash
    sudo ip link set can1 up type can bitrate 500000
    ```
    
    ```python
    # add bashrc
    alias cl='sudo ip link set can1 up type can bitrate 500000'
    ```
    
    ```bash
    # receive
    (env) bugatti@bugatti:~ $ candump can1
      can1  000   [8]  00 00 00 00 00 00 09 1C
      can1  000   [8]  00 00 00 00 00 00 09 1D
      can1  000   [8]  00 00 00 00 00 00 09 1E
      can1  000   [8]  00 00 00 00 00 00 09 1F
      can1  000   [8]  00 00 00 00 00 00 09 20
      can1  000   [8]  00 00 00 00 00 00 09 21
      can1  000   [8]  00 00 00 00 00 00 09 22
      can1  000   [8]  00 00 00 00 00 00 09 23
      can1  000   [8]  00 00 00 00 00 00 09 24
    ```
    
6. You can use python code to get the CAN data
    
    ```bash
    # install python-can
    sudo pip3 install python-can
    ```
    
    ```bash
    cd ~/mycar
    sudo nano can_recv.py
    ```
    
7. Paste this code and execute
    - Arduino
        
        ```arduino
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
        float rpm = 0;
        
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
        
            CAN.sendMsgBuf(0x00, 0, 8, stmp);
            delay(100);                       // send data per 100ms
        }
        
        void count()
        {
            cnt++;
        }
        // END FILE
        ```
        
    - python (RPI)
        
        ```python
        import can
         
        can_interface = 'can1'
        bus = can.interface.Bus(can_interface, bustype='socketcan')
        while True:
            message = bus.recv(1.0) # Timeout in seconds.
            if message is None:
                    print('Timeout occurred, no message.')
            print(message)
        ```
        
    
    ```bash
    (env) bugatti@bugatti:~/mycar $ python3 can_recv.py
    Timestamp: 1663683669.282204        ID: 0000    S Rx                DL:  8    00 00 00 00 00 00 23 47     Channel: can1
    Timestamp: 1663683669.382681        ID: 0000    S Rx                DL:  8    00 00 00 00 00 00 23 48     Channel: can1
    Timestamp: 1663683669.482546        ID: 0000    S Rx                DL:  8    00 00 00 00 00 00 23 49     Channel: can1
    Timestamp: 1663683669.582747        ID: 0000    S Rx                DL:  8    00 00 00 00 00 00 23 4a     Channel: can1
    Timestamp: 1663683669.682920        ID: 0000    S Rx                DL:  8    00 00 00 00 00 00 23 4b     Channel: can1
    Timestamp: 1663683669.783183        ID: 0000    S Rx                DL:  8    00 00 00 00 00 00 23 4c     Channel: can1
    Timestamp: 1663683669.883471        ID: 0000    S Rx                DL:  8    00 00 00 00 00 00 23 4d     Channel: can1
    Timestamp: 1663683669.983672        ID: 0000    S Rx                DL:  8    00 00 00 00 00 00 23 4e     Channel: can1
    ```