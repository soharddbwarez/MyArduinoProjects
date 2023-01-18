# This is for the LILYGO TTGO T-Display ESP32 WiFi bluetooth Module 1.14 Inch LCD Development Board - CH9102F 16MB
If you're interested in buying this board checkout BangGood here: https://www.banggood.com/custlink/vmGJ6yJrRi
## WeatherStation is a combination of code that I found here on GitHub from two people that I put together.

I'm working on adding a third screen option from a third guy that also suggested his code to the same project 
which I really like and gives some extra useful information about voltage and the battery, also on the main
screen there's an extra icon showed showing the current weather with a cloud, rain, the sun etc.
At this point I've only uploaded the icons in the data directory so you can leave that out if you 
want to try this WeatherStation you don't need it for it to work.

I want to give credits to the guy who made this project available which is:
VolosR and you can find him here on GitHub at: https://github.com/VolosR/TTGOWeatherStation
Check out his profile he has more really cool projects for this board, even games.

Because the code wasn't working for me I started looking at the issues and what other people comitted
and so I found the code for the main WeatherStation.ino file and he also added an env.h file to get the
network login and api key seperated which is easier to change.
So I took that part of the code that I got from Leon which you can find here: https://github.com/Leon2ev
He did a great job in optimizing the code, adding comments etc. but the code doesn't get merged so
I decided to take over this project, merged the code myself going through all the commits manually
and made it working, saved the code locally and uploaded it to here as it is right now this is the latest
iteration at the moment.

I'm still working on this project because I found a bug when I startup the board but I need Leon's input
to figure it out and getting it optimized so it works reliably every time you startup.
Now you'll have to push reset a couple times until it loads all the data and showing it on the display correctly.

I'll keep this up to date and as soon as it's fixed you'll know it right after that.

# Here's what I'm working on right now...
I found on the original project page that someone asked if it was possible to implement a way to change the
WiFi without hardcoding your own network credentials into the code, the answer is of course yes.
I haven't implemented a WiFi manager into any program before but that doesn't hold me back getting it done,
it happens to be something that I was looking for because the single hardcoded network that it can connect to
really isn't working out for me because I wanted to show it somewhere else which requires me to connect to
their network of course so that was underwhelming that it was working okay but without an internet connection
this gadget doesn't do anything.

So, I've been busy getting the necessary libraries installed into my Arduino IDE and I've been reading a lot
of documentation to get familiar with the code, turns out that it's not that easy to get it just right
because of the many different libraries forked from each other so I hope that I've got all the right parts
together now to make this work.

#How It Works
This information can be found on GitHub here: https://github.com/khoih-prog/ESP_WiFiManager_Lite#how-it-works
 - The ESP_WiFi example shows how it works and should be used as the basis for a sketch that uses this library.
 - The concept of ESP_WiFi is that a new ESP32/ESP8266 WiFi will start a WiFi configuration portal when powered up, 
   but has no valid stored Credentials or can't connect to WiFi APs after a pre-determined time.
 - There are 6 more custom parameters added in the sketch which you can use in your program later. In the example, 
   they are: 2 sets of Blynk Servers and Tokens, Blynk Port and MQTT Server.
 - Using any WiFi enabled device with a browser (computer, phone, tablet) connect to the newly created AP and type 
   in the configurable AP IP address (default 192.168.4.1). The Config Portal AP channel (default 10) is also 
   configurable to avoid conflict with other APs.
 - The Config Portal is auto-adjusted to fix the 4 static parameters (WiFi SSIDs/PWDs) as well as 6 more 
   dynamic custom parameters.
 - After the custom data entered, and Save button pressed, the configuration data will be saved in host's 
   non-volatile memory, then the board reboots.
 - If there is valid stored Credentials, it'll go directly to connect to one of the MultiWiFi APs without 
   starting / using the Config Portal.
 - ESP32/ESP8266 WiFi will try to connect. If successful, the dynamic DHCP and/or configured static IP address 
   will be displayed in the configuration portal.
 - The ESP32/ESP8266 WiFi Config Portal network and Web Server will shutdown to return control to the sketch code.
 - In the operation, if the current WiFi connection is lost because of any reason, the system will auto(Re)connect 
   to the remaining WiFi AP.
 - If system can't connect to any of the 2 WiFi APs, the Config Portal will start, after some pre-determined time, 
   to permit user to update the Credentials.
