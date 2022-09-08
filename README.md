# Team_Bugatti_PiRacer



<details>
<summary>~2022.09.07</summary>

### wiki
https://www.waveshare.com/wiki/PiRacer_Pro_AI_Kit

### Assemble PiRacer
https://www.waveshare.com/w/upload/a/a2/Piracer_pro_ai_kit-en2.pdf

### Install Image
https://www.raspberrypi.com/software/

![Untitled (1)](https://user-images.githubusercontent.com/111988634/188967883-b973bd4d-04a3-4c35-8b94-13f593164421.png)
![Untitled](https://user-images.githubusercontent.com/111988634/188967633-cdf18f14-a9fd-4264-a4a3-2d365bbb6a68.png)Write Raspberry pi OS in TF card

### Connect Linux labtop to display Raspberry Pi ( WIFI )
- https://www.youtube.com/watch?v=T2hmCf1XZmg
- Download PuTTY SSH Client on ubuntu 22.04
https://www.linuxcapable.com/how-to-install-connect-putty-ssh-client-on-ubuntu-22-04-lts/
![Untitled (3)](https://user-images.githubusercontent.com/111988634/188968808-3ed45c12-fe8e-4491-b44c-c7c157cc9a66.png)
Since you don't know the PiRacer's internet address, you need to have your own router.
https://askubuntu.com/- 2022.09.08
    - Change Raspberry Pi OS
    - [https://docs.donkeycar.com/guide/robot_sbc/setup_raspberry_pi/](https://docs.donkeycar.com/guide/robot_sbc/setup_raspberry_pi/)
    - download this file [https://downloads.raspberrypi.org/raspios_oldstable_lite_armhf/images/raspios_oldstable_lite_armhf-2021-12-02/2021-12-02-raspios-buster-armhf-lite.zip](https://downloads.raspberrypi.org/raspios_oldstable_lite_armhf/images/raspios_oldstable_lite_armhf-2021-12-02/2021-12-02-raspios-buster-armhf-lite.zip)
    - insert your sd card in your computer, download image through this
        
        [Raspberry Pi OS - Raspberry Pi](https://www.raspberrypi.com/software/)
        
        ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/04945f1a-b97b-4d24-9120-98f4c556ad6f/Untitled.png)
        
        ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/0f9b858c-f8bc-416b-8802-38c3eb398bf5/Untitled.png)
        
        ```bash
        cd /media/seame-three/boot
        touch ssh
        code wpa_supplicant.conf
        ```
        
        - paste it
        
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
        
        - eject sd card and put it in the Pi and power on
        - find my car’s ip
        
        ```bash
        ping raspberrypi.local
        ```
        
        ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/235e3f94-53a0-4141-9d39-1ee8b520688b/Untitled.png)
        
        - connect to pi
        
        ```bash
        ssh bugatti@192.168.0.102
        ```
        
        - rasberry pi bash
        
        ```bash
        sudo apt-get update --allow-releaseinfo-change
        sudo apt-get upgrade
        ```
        
        ```bash
        sudo raspi-config
        ```
        
        - change default password for pi
        - change hostname
        - enable `Interfacing Options` - `I2C`
        - enable `Interfacing Options` - `Camera`
        - select `Advanced Options` - `Expand Filesystem` so you can use your whole sd-card storage
        
        Choose `<Finish>` and hit enter.
        
        Reboot Piracer
        
        ```bash
        ping bugatti.local
        ssh bugatti@192.168.0.102
        sudo apt-get install build-essential python3 python3-dev python3-pip python3-virtualenv python3-numpy python3-picamera python3-pandas python3-rpi.gpio i2c-tools avahi-utils joystick libopenjp2-7-dev libtiff5-dev gfortran libatlas-base-dev libopenblas-dev libhdf5-serial-dev libgeos-dev git ntp
        sudo apt-get install libilmbase-dev libopenexr-dev libgstreamer1.0-dev libjasper-dev libwebp-dev libatlas-base-dev libavcodec-dev libavformat-dev libswscale-dev libqtgui4 libqt4-test
        ```
        
        ```bash
        python3 -m virtualenv -p python3 env --system-site-packages
        echo "source ~/env/bin/activate" >> ~/.bashrc
        source ~/.bashrc
        ```
        
        ```bash
        mkdir projects
        cd projects
        ```
        
        ```bash
        git clone https://github.com/autorope/donkeycar
        cd donkeycar
        git checkout main
        pip install -e .[pi]
        pip install https://github.com/lhelontra/tensorflow-on-arm/releases/download/v2.2.0/tensorflow-2.2.0-cp37-none-linux_armv7l.whl
        ```
        
        validate tensorflow
        
        ```bash
        python -c "import tensorflow; print(tensorflow.__version__)"
        ```
        
        ```bash
        sudo apt install python3-opencv
        # if failed
        # pip install opencv-python
        python -c "import cv2"
        ```
        
        ```bash
        cd ~/mycar
        nano myconfig.py
        ```
        
- failes
    
    ```bash
    sudo apt-get update
    sudo apt-get upgrade
    ```
    
    ```bash
    sudo apt-get install build-essential python3 python3-dev python3-pip python3-virtualenv python3-numpy python3-picamera 
    sudo apt-get install python3-pandas python3-rpi.gpio i2c-tools avahi-utils joystick libopenjp2-7-dev libtiff5-dev gfortran 
    sudo apt-get install libatlas-base-dev libopenblas-dev libhdf5-serial-dev git ntp
    ```
    
    ```bash
    sudo apt-get install libilmbase-dev libopenexr-dev libgstreamer1.0-dev libjasper-dev libwebp-dev 
    sudo apt-get install libatlas-base-dev libavcodec-dev libavformat-dev libswscale-dev
    ```
    
    ```bash
    git clone https://github.com/autorope/donkeycar
    cd donkeycar
    git checkout master
    pip install -e .[pi]
    ```
    
    - To install tensorflow==1.13.1, you should download bazel in raspberry pi.
    - [https://github.com/koenvervloesem/bazel-on-arm/blob/master/README.md](https://github.com/koenvervloesem/bazel-on-arm/blob/master/README.md)
    
    ```bash
    git clone https://github.com/koenvervloesem/bazel-on-arm.git
    cd bazel-on-arm
    sudo make requirements
    make bazel
    
    pip install tensorflow==1.13.1
    pip install numpy --upgrade
    ```
    
    ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/e659b73e-ad07-4db0-8713-c660b46672ca/Untitled.png)
    
    - Download balenaEtcher
    - [https://www.balena.io/etcher/](https://www.balena.io/etcher/)
    - 
    
    ```bash
    cd Downloads
    chmod u+x balena-etcher-electron-1.7.9-linux-ia32
    
    ```
    
    - extract the file
    - properties → permissions → Group → Access : Change to Read and write
        
        ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/b95711aa-5348-4cb2-9adb-3e02e855fade/Untitled.png)
        
    -questions/1410644/putty-0-76-does-not-work-on-ubuntu-22-04-unless-with-sudo
you should change font

### ====== trouble shooting ======
![Untitled (4)](https://user-images.githubusercontent.com/111988634/188969563-6d3fdb40-568f-4f2c-96e7-f0e265d90b98.png)
- cannot download opencv libraries libqtgui4, libqt4-test
![Untitled (5)](https://user-images.githubusercontent.com/111988634/188969579-e223d013-d570-419f-9722-ca41937d1510.png)
- cannot download tensorflow 1.13.1 (maybe python version problem??)
</details>
<details>
<summary>2022.09.08</summary>
- 2022.09.08
    - Change Raspberry Pi OS
    - [https://docs.donkeycar.com/guide/robot_sbc/setup_raspberry_pi/](https://docs.donkeycar.com/guide/robot_sbc/setup_raspberry_pi/)
    - download this file [https://downloads.raspberrypi.org/raspios_oldstable_lite_armhf/images/raspios_oldstable_lite_armhf-2021-12-02/2021-12-02-raspios-buster-armhf-lite.zip](https://downloads.raspberrypi.org/raspios_oldstable_lite_armhf/images/raspios_oldstable_lite_armhf-2021-12-02/2021-12-02-raspios-buster-armhf-lite.zip)
    - insert your sd card in your computer, download image through this
        
        [Raspberry Pi OS - Raspberry Pi](https://www.raspberrypi.com/software/)
        
        ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/04945f1a-b97b-4d24-9120-98f4c556ad6f/Untitled.png)
        
        ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/0f9b858c-f8bc-416b-8802-38c3eb398bf5/Untitled.png)
        
        ```bash
        cd /media/seame-three/boot
        touch ssh
        code wpa_supplicant.conf
        ```
        
        - paste it
        
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
        
        - eject sd card and put it in the Pi and power on
        - find my car’s ip
        
        ```bash
        ping raspberrypi.local
        ```
        
        ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/235e3f94-53a0-4141-9d39-1ee8b520688b/Untitled.png)
        
        - connect to pi
        
        ```bash
        ssh bugatti@192.168.0.102
        ```
        
        - rasberry pi bash
        
        ```bash
        sudo apt-get update --allow-releaseinfo-change
        sudo apt-get upgrade
        ```
        
        ```bash
        sudo raspi-config
        ```
        
        - change default password for pi
        - change hostname
        - enable `Interfacing Options` - `I2C`
        - enable `Interfacing Options` - `Camera`
        - select `Advanced Options` - `Expand Filesystem` so you can use your whole sd-card storage
        
        Choose `<Finish>` and hit enter.
        
        Reboot Piracer
        
        ```bash
        ping bugatti.local
        ssh bugatti@192.168.0.102
        sudo apt-get install build-essential python3 python3-dev python3-pip python3-virtualenv python3-numpy python3-picamera python3-pandas python3-rpi.gpio i2c-tools avahi-utils joystick libopenjp2-7-dev libtiff5-dev gfortran libatlas-base-dev libopenblas-dev libhdf5-serial-dev libgeos-dev git ntp
        sudo apt-get install libilmbase-dev libopenexr-dev libgstreamer1.0-dev libjasper-dev libwebp-dev libatlas-base-dev libavcodec-dev libavformat-dev libswscale-dev libqtgui4 libqt4-test
        ```
        
        ```bash
        python3 -m virtualenv -p python3 env --system-site-packages
        echo "source ~/env/bin/activate" >> ~/.bashrc
        source ~/.bashrc
        ```
        
        ```bash
        mkdir projects
        cd projects
        ```
        
        ```bash
        git clone https://github.com/autorope/donkeycar
        cd donkeycar
        git checkout main
        pip install -e .[pi]
        pip install https://github.com/lhelontra/tensorflow-on-arm/releases/download/v2.2.0/tensorflow-2.2.0-cp37-none-linux_armv7l.whl
        ```
        
        validate tensorflow
        
        ```bash
        python -c "import tensorflow; print(tensorflow.__version__)"
        ```
        
        ```bash
        sudo apt install python3-opencv
        # if failed
        # pip install opencv-python
        python -c "import cv2"
        ```
        
        ```bash
        cd ~/mycar
        nano myconfig.py
        ```
        
- failes
    
    ```bash
    sudo apt-get update
    sudo apt-get upgrade
    ```
    
    ```bash
    sudo apt-get install build-essential python3 python3-dev python3-pip python3-virtualenv python3-numpy python3-picamera 
    sudo apt-get install python3-pandas python3-rpi.gpio i2c-tools avahi-utils joystick libopenjp2-7-dev libtiff5-dev gfortran 
    sudo apt-get install libatlas-base-dev libopenblas-dev libhdf5-serial-dev git ntp
    ```
    
    ```bash
    sudo apt-get install libilmbase-dev libopenexr-dev libgstreamer1.0-dev libjasper-dev libwebp-dev 
    sudo apt-get install libatlas-base-dev libavcodec-dev libavformat-dev libswscale-dev
    ```
    
    ```bash
    git clone https://github.com/autorope/donkeycar
    cd donkeycar
    git checkout master
    pip install -e .[pi]
    ```
    
    - To install tensorflow==1.13.1, you should download bazel in raspberry pi.
    - [https://github.com/koenvervloesem/bazel-on-arm/blob/master/README.md](https://github.com/koenvervloesem/bazel-on-arm/blob/master/README.md)
    
    ```bash
    git clone https://github.com/koenvervloesem/bazel-on-arm.git
    cd bazel-on-arm
    sudo make requirements
    make bazel
    
    pip install tensorflow==1.13.1
    pip install numpy --upgrade
    ```
    
    ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/e659b73e-ad07-4db0-8713-c660b46672ca/Untitled.png)
    
    - Download balenaEtcher
    - [https://www.balena.io/etcher/](https://www.balena.io/etcher/)
    - 
    
    ```bash
    cd Downloads
    chmod u+x balena-etcher-electron-1.7.9-linux-ia32
    
    ```
    
    - extract the file
    - properties → permissions → Group → Access : Change to Read and write
        
        ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/b95711aa-5348-4cb2-9adb-3e02e855fade/Untitled.png)
        
    -
</details>
