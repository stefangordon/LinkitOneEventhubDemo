#LinkIt One EventHub Demo
Example using the LinkIt One to communicate with Azure Websites and EventHub

###How To Run
1. Configure the web.config of the web solution with the correct Event Hub URL and Service bus connection string
2. Generate an SAS url (with reasonably long expiry) using the service bus SAS URL Generator https://github.com/sandrinodimattia/RedDog/releases/tag/0.2.0.1
3. Place the SAS Url and publisher/deviceid name in the Arduino code
4. Update the WIFI credentials in the Arduino code
5. Deploy website
6. Deploy LinkIt One
