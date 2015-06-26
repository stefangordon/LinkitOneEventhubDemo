#LinkIt One EventHub Demo
Example using the LinkIt One to communicate with Azure Websites and EventHub

###Creating an EventHub
>There is a quick walk through video at the bottom of this readme which will provide more detail on setup.

1. Create a new Service Bus instance in the non-preview Azure Portal.
2. Add an EventHub to it.  1 Day retention and 8 partitions is a good default.
3. Add two Security Access Policies to the EventHub, one for your LinkIt One and one for the monitoring website.  The LinkIt One policy should have "send" permissions and be named after your device ("l1device" in the sample code).  The website policy should have "listen" permissions.

###Overview on Running Code
1. Configure the web.config of the web solution with the correct Event Hub URL and Service bus connection string
2. Generate an SAS url (with reasonably long expiry) using the service bus SAS URL Generator and the security access policy created for your device https://github.com/sandrinodimattia/RedDog/releases/tag/0.2.0.1
3. Place the SAS Url and publisher/deviceid name in the Arduino code
4. Update the WIFI credentials in the Arduino code
5. Deploy website.  The UI will be present at \www\index.htm
6. Deploy LinkIt One

###Notes on first use of LinkIt One 
1. Install Arduino IDE 1.5.7 Beta https://www.arduino.cc/en/Main/OldSoftwareReleases#previous
2. Install LinkitOne SDK http://download.labs.mediatek.com/MediaTek_LinkIt_SDK_for_Arduino_Windows_1_1_09.zip
3. Plug in LinkIt One via USB
3. From device manager update the driver on the two unknown COM port devices.  Tell Windows to search for the driver in \Arduino\Drivers\mtk
4. Use the DEBUG com port for programming

###Notes on Debugging
The LinkIt One will expose a Debug and Modem COM port.  You set the Arduino IDE to the Debug COM port for deploying code, but you will see Serial output on the Modem COM port.  If you do not have an alternative tool for connecting to the Modem COM port you may launch a second instance of the Arduino IDE, choose that COM port, and launch the Arduino terminal (control+shift+M).

You will have to close and relaunch this monitoring terminal each time you deploy the code to see data, as it loses its connection when the LinkIt One power cycles.

###Quick Video Walkthrough
http://youtu.be/qwjrrthRbsU
