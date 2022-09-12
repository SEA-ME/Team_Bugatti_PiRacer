# Raspberry Pi Setup

- Contents
    
    [Step 1: Flash Operating System](#step-1-flash-operating-system)
    
    [Step 2: Setup for the first boot](#step-2-setup-for-the-first-boot)
    
    [Step 3: Connecting to the Pi](#step-3-connecting-to-the-pi)
    
    [Step 4: Update and Upgrade](#step-4-update-and-upgrade)
    
    [Step 5: Raspi-config](#step-5-raspi-config)
    
    [Step 6: Install Dependencies](#step-6-install-dependencies)
    
    [Step 7: Install OpenCV Dependencies](#step-7-install-opencv-dependencies)
    
    [Step 8: Setup Virtual Env](#step-8-setup-virtual-env)
    
    [Step 9: Install Donkeycar Python Code](#step-9-install-donkeycar-python-code)
    
    [Step 10: Install OpenCV](#step-10-install-opencv)
    
---
<br/>

## Step 1: Flash Operating System
Advanced options

1. Download [Raspian Legacy](https://downloads.raspberrypi.org/raspios_oldstable_lite_armhf/images/raspios_oldstable_lite_armhf-2021-12-02/2021-12-02-raspios-buster-armhf-lite.zip) ( Buster ) 

> Raspbian Latest ( bullseye ) is not compatible with PiRacer Pro wiki
> 
2. Download [Raspberry Pi Imager](https://www.raspberrypi.com/software/)
3. Connect you SD card in your computer ( labtop )
4. Change Operating System to `Buster` above and Storage
5. Change advanced options to connect WIFI
6. Click Write to Download OS

<br/>

## Step 2: Setup for the first boot

1. Start a text editor ( VS code )
    
    ```bash
    cd /media/seame-three/boot # SD card boot
    touch ssh # create ssh file to enable SSH on boot
    code wpa_supplicant.conf # create wpa_supplicant.conf file
    ```
    
2. Paste and edit to match your wifi
    
    ```bash
    ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
    update_config=1
    country=GE
    network={
        ssid="your network name"
        psk="your password"
        scan_ssid=1
    }
    ```
    
3. Eject your SD card and put it in the Pi and power on

<br/>

## Step 3: Connecting to the Pi

Find car IP

1. Find my car’s IP
    
    ```bash
    pint <your hostname>.local
    ```
    
2. Connet to Pi
    
    ```bash
    ssh bugatti@<your pi ip address>
    ```
    
    - Username: `bugatti`
    - Password: `seame2022`
    - Hostname: `your pi IP address`

<br/>

## Step 4: Update and Upgrade

```bash
sudo apt-get update --allow-releaseinfo-change
sudo apt-get upgrade
```
<br/>

## Step 5: Raspi-config

```bash
sudo raspi-config
```

- change default password for pi
- change hostname
- enable `Interfacing Options` - `I2C`
- enable `Interfacing Options` - `Camera`
- select `Advanced Options` - `Expand Filesystem` so you can use your whole sd-card storage
- Choose `<Finish>` and Reboot

<br/>

## Step 6: Install Dependencies

```bash
ping bugatti.local
ssh bugatti@192.168.0.102
sudo apt-get install build-essential python3 python3-dev python3-pip python3-virtualenv python3-numpy python3-picamera python3-pandas python3-rpi.gpio i2c-tools avahi-utils joystick libopenjp2-7-dev libtiff5-dev gfortran libatlas-base-dev libopenblas-dev libhdf5-serial-dev libgeos-dev git ntp
```
<br/>

## Step 7: Install OpenCV Dependencies

```bash
sudo apt-get install libilmbase-dev libopenexr-dev libgstreamer1.0-dev libjasper-dev libwebp-dev libatlas-base-dev libavcodec-dev libavformat-dev libswscale-dev libqtgui4 libqt4-test
```

<br/>

## Step 8: Setup Virtual Env

```bash
python3 -m virtualenv -p python3 env --system-site-packages
echo "source ~/env/bin/activate" >> ~/.bashrc
source ~/.bashrc
```
<br/>

## Step 9: Install Donkeycar Python Code

1. Create Directory
    
    ```bash
    cd ~
    mkdir projects
    cd projects
    ```
    
2. Get the latest donkeycar from Github
    
    ```bash
    git clone https://github.com/autorope/donkeycar
    cd donkeycar
    git checkout main
    pip install -e .[pi]
    pip install https://github.com/lhelontra/tensorflow-on-arm/releases/download/v2.2.0/tensorflow-2.2.0-cp37-none-linux_armv7l.whl
    ```
    

1. Validate tensorflow install
    
    ```bash
    python -c "import tensorflow; print(tensorflow.__version__)"
    ```
    
<br/>

## Step 10: Install OpenCV

1. Install OpenCV
    
    ```bash
    sudo apt install python3-opencv
    # if failed
    # pip install opencv-python
    ```
    
2. Test to see if import succeeds
    
    ```bash
    python -c "import cv2"
    # if no errors, you have OpenCV installed
    ```

<br/>

# PiRacer Setup

- Contents
    
    [Create Donkeycar from Template](#create-donkeycar-from-template)
    
    [Web Control](#web-control)
    
    [Calibrate DonkeyCar](#calibrate-donkeycar)
    
    [Teleoperation ( GamePad )](#teleoperation--shanwan-gamepad)
    



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

    If Pi doesn’t display like below, exchange your **PiRacer expansion board**
    
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


1. Open Raspberry Pi terminal and run the follow commands
    
    ```bash
    source ~/env/bin/activate
    cd ~/mycar
    python manage.py drive
    ```
    
2. Go to `http://<raspberrypi_ip_address>:8887`
    
    <aside>
    💡 If you connected with physical game controller, it overrides web control.
    
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
💡 Nintendo Switch Pro Controller is compatible with  kernel version more than 5.16

</aside>

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