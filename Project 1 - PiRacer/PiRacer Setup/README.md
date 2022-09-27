# PiRacer Setup

- Contents  
    [Create Donkeycar from Template](#create-donkeycar-from-template)  
    [Web Control](#web-control)  
    [Calibrate DonkeyCar](#calibrate-donkeycar)  
    [Teleoperation ( Shanwan gamepad )](#teleoperation--shanwan-gamepad)
    



---

<br/>

## Create Donkeycar from Template

1. Connect PiRacer by SSH

   ```bash
   ping <your hostname>.local
   ssh bugatti@<your pi ip address>
   donkey createcar --path ~/mycar
   ```

2. Configure I2C PCA9685 ( servo driver board )

    ```bash
    sudo apt-get install -y i2c-tools
    sudo i2cdetect -y 1
    ```

    If Pi doesn't display like below, exchange your **PiRacer expansion board**
    
    ```bash
         0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
    00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
    10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    30: -- -- -- -- -- -- -- -- -- -- -- -- 3c -- -- -- 
    40: 40 -- 42 -- -- -- -- -- -- -- -- -- -- -- -- -- 
    50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    70: 70 -- -- -- -- -- -- --
    ```
<br/>  

## Web Control

![Untitled (4)](https://user-images.githubusercontent.com/111988634/189865372-29e46eb9-9c7a-4abf-a8f5-9ac2da8a535f.png)
1. Open Raspberry Pi terminal and run the follow commands
    
    ```bash
    source ~/env/bin/activate
    cd ~/mycar
    python manage.py drive
    ```
    
2. Go to `http://<raspberrypi_ip_address>:8887`
    
    <aside>
    :warning: If you connected with physical game controller, it overrides web control.
    
    </aside>
    
<br/>

## Calibrate DonkeyCar

1. Adjust the value to make the servo turns to the center place and remember the value ( In our case, 380 was center )
    
    ```bash
    cd ~/mycar
    donkey calibrate --channel 0 --bus=1
    ```
    

1. Test if 280, and 480 can let the servo turns to the far left and the far right

1. Modify the config.py file and change variables
    
    ```bash
    sudo nano config.py
    # Change values
    # STEERING_LEFT_PWM = 480
    # STEERING_RIGHT_PWM = 280
    # THROTTLE_FORWARD_PWM = 500
    # THROTTLE_STOPPED_PWM = 370
    # THROTTLE_REVERSE_PWM = 220
    ```
    
<br/>

## Teleoperation ( Shanwan gamepad )

<aside>
:warning: Nintendo Switch Pro Controller is compatible with  kernel version more than 5.16

</aside>

![Untitled (5)](https://user-images.githubusercontent.com/111988634/189865479-05225856-e480-4b85-89c0-9c7b93fc58e7.png)

1. Connect game pad with PiRacer
2. Create joystick creator wizard
    
    ```bash
    cd ~/mycar
    donkey createjs
    ```
    
3. Map button, axis, control
In mapping steps of steering and throttle, you should set in opposite direction 
( For example, If you want to throttle Left Stick: Vertical, you need to register Right Stick: Horizontal in the throttle step )
4. Change configuration
    
    ```bash
    cd ~/mycar
    sudo nano config.py
    # JOYSTICK_DEVICE_FIL = "/dev/input/js0"
    # CONTROLLER_TYPE = 'custom'
    ```
