# ESP_Rainmaker-Smart-Controller

ESP Rainmaker is an open-source platform that allows developers to easily connect, control, and manage Internet of Things (IoT) devices using the ESP32 and ESP8266 microcontrollers.

The platform includes a cloud-based management system, a mobile application, and a set of libraries and examples for the ESP microcontrollers. The ESP Rainmaker is similar to other IoT services like the Blynk application, Ubidots, Thingspeak, IFTTT, Arduino IoT cloud, etc. But this platform offers more features such as self-adapting mobile apps, free integration of Alexa and Google Assistant, Node sharing, Wi-Fi provisioning through QR code, and much more.

ESP Rainmaker is an open-source platform that allows developers to easily connect, control, and manage Internet of Things (IoT) devices using the ESP32 and ESP8266 microcontrollers.

The platform includes a cloud-based management system, a mobile application, and a set of libraries and examples for the ESP microcontrollers. The ESP Rainmaker is similar to other IoT services like the Blynk application, Ubidots, Thingspeak, IFTTT, Arduino IoT cloud, etc. But this platform offers more features such as self-adapting mobile apps, free integration of Alexa and Google Assistant, Node sharing, Wi-Fi provisioning through QR code, and much more.

## Installation

ESP  RainMaker Workflow
The IDE in use is Arduino and requires ESP32 and USB to UART  driver support in some OSs.  This [tutorial](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) has all the info and installation steps.  
The ESP32 is programmed with ESP RainMaker firmware which in turn generates and displays a QR code in the serial monitor which can be scanned using the ESP rainmaker app.
Once the QR code is scanned the app will initiate BLE communication with the ESP32 SoC.
When connection is established, the SoC is provisioned by providing the WiFi credentials. 
The ESP32 will establish connection to the ESP RainMaker Cloud through the WiFi connection and will exchange the relevant data we have programmed it to do.
Once everything is setup, we can communicate to the ESP32 through either the Cloud Service or the mobile app.



## Usage

The code we'll employ is extremely straightforward. We will establish a node and add new devices with specified parameters to it. This example will involve the creation of three devices: my switch, temperature, and humidity.
The two other devices will be used to monitor and show temperature and humidity from the DHT11 sensor while the my switch device will be used to control the LED. Install the essential libraries first, of course. The SimpleTimer library by Alexander Kiryanenko is the required library for this example.
By looking for their names in the library manager, you can install them right away. 

1. We began by including all of the required libraries, which included the WiFi and RainMaker, Simple timer libraries. Then we created a few variables to keep track of the LED and Locks.
We later specified the BLE credentials by using the pointers service name and pop, which will be used during provisioning.
2. The write callback function will be in charge of determining which device sent the data and then changing the device's parameters accordingly. 
3. We start the serial communication at 115200 baud rate inside the setup() function. The reset pin is then set as an input pin, and the LED and Locks pin are set as output pins.
This is accomplished by calling the pinMode() function and passing the GPIO pin as the first parameter and the mode as the second. Furthermore, we will initially set the LED and Locks pins to the default state that we defined at the start.
4. he ESP RainMaker node was later declared. Then, connect all three devices to the node. 
Additionally, we activated the OTA, Timezone, and Schedule services and declared the callback method for the my switch device. Following that, we developed a timer to transmit sensor data to the ESP RainMaker Cloud every two seconds. The RainMaker band was later started using the Rmaker.start function, and provisioning was then handled by calling the sysProEvent function.
5. We will check the status of GPIO0 during the loop and update the parameter led state appropriately.
The WiFi password may be reset with a long push for three seconds, and the ESP32 RainMaker firmware can be reset to factory defaults with a long press for ten seconds. While the device is being reconfigured, these features will be fully utilized.

## Google Voice Assistant Integration
1. We need to install the Google Home App in order to use the RainMaker firmware with Google Assistant. After downloading and signing in to the Google Main app, select Setup a device from the + symbol on the app's home screen.
On the following screen, choose the works with Google option.
2. You will now see a list of all devices that support Google Assistant. Choose the ESP RainMaker option from the list.
To find it quickly, you may also use the search option. This time, you'll be taken to the ESP RainMaker login page. Once you've logged in to your smartphone, a shortcut will be put to the home screen and the Google Assistant function will be available to you.
To operate the LED and locks, either use the shortcut or your voice. 




## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
