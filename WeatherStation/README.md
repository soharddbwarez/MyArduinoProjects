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
