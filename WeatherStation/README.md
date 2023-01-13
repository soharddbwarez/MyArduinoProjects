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

## Here's How It Works
The Async_ConfigOnSwitch (https://github.com/khoih-prog/ESPAsync_WiFiManager/blob/master/examples/Async_ConfigOnSwitch) 
example shows how it works and should be used as the basis for a sketch that uses this library.

The concept of Async_ConfigOnSwitch is that a new ESP32 / ESP8266 will start a WiFi ConfigPortal when powered up and 
save the configuration data in non volatile memory. Thereafter, the ConfigPortal will only be started again if a 
button is pushed on the ESP32 / ESP8266 module.

Using any WiFi enabled device with a browser (computer, phone, tablet) connect to the newly created Access Point (AP) 
using configurable SSID and Password (specified in sketch)

// SSID and PW for Config Portal
String ssid = "ESP_" + String(ESP_getChipId(), HEX);
const char* password = "your_password";

then connect WebBrowser to configurable ConfigPortal IP address, default is 192.168.4.1

Choose one of the access points scanned, enter password, click Save. ESP will restart, then try to connect to the 
WiFi netwotk using STA-only mode, without running the ConfigPortal WebServer and WiFi AP. 
See Accessing manager after connection (https://github.com/khoih-prog/ESP_WiFiManager/issues/15).

Because I want to keep the part that's already working on the TTGO board separated I'll add this part in a new file 
"ConfigPortal.h" that needs to be included in the main program file using the quotes "" instead of the hooks <>
since the file is in the same directory as the main program.

My new file will be replacing the env.h file when I have it working but I'm using Git to update my online
repositories so it will all be updated for you automatically, hopefully to a better working program.

I hope that I can get this working so I can share it with you here.
