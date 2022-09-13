# Arduino Setup
# Project 2 - Digital Instrument Cluster

- Arduino Nano ( humidity, temperature )
- transmit humidity, temperature to CAN
- show on LCD connected to Arduino Uno
- show the work ( room temperature )
- RPM, Speed
- battery level
- Implement PRND indicators

[Arduino Simulator](https://www.tinkercad.com/dashboard)

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