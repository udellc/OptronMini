# OptronMini
Repo for the Optron Mini

# Project Wiki:
https://github.com/udellc/OptronMini/wiki

# Folder Details
### BOM
Link to live spreadsheet of materials and links to products
### CAD
DXF: Laser Cutting File for Mini Frame
PCB: .sch and .brd files for Mini Circuit board
STL: 3D printer files
Archive: Old file dump for your enjoyment
### Code
Arduino code to download onto your Adafruit ItsyBitsy M4
### MaxMSP
Max8 apps tto use your Optron Mini!

# Getting Started
## Program Optron Mini
###Install Arduino IDE and configure to program M4 Itsy Bitsy:
Follow instructions at this link:
https://learn.adafruit.com/introducing-adafruit-itsybitsy-m4/setup

Then follow the instructions at this link:
https://learn.adafruit.com/introducing-adafruit-itsybitsy-m4/using-with-arduino-ide

You will now be able to push code from your computer IDE to the Adafruit M4 ItsyBitsy!

##Download Optron Mini Code
Click the green CODE download button on the top-right of the Optron Mini repo page.
https://github.com/udellc/OptronMini/
Unzip the folder
Find and open the following file with the Arduino IDE Application:
OptronMini > Code > M4_OptronMinixx.xx.xx > M4_OptronMinixx.xx.xx.ino

The opened window should look like:
-screenshot here-
There are three tabs that organize the code into major sections.
M4_....ino: Main program code that uses all support functions and files.
LEDPatterns.ino: All Serial LED commands and LED pattern functions are found here
Sensors.ino: All Sensor code is found here

##Compile and upload
First - connect a USB data cable (some USB cables are only intended for charging and will not work) from the M4 Itsy Bitsy micro USB port to the computer USB port.
If you havent already done so in the Arduino IDE go to: Tools > Board > Adafruit ItsyBitsy M4 (SAMD51)
Then go to: Tools > Port 
Choose the USB COM port (PC) or usbserialxxxx port (Mac) that corresponds with the Adafruit board
-screenshot here-
Click the Check circle button at the top left of the IDE window to compile. Wait for message on the console to indicate successful compilation.
Click the Arrow button beside the Check button to upload the code to the M4.
See troubleshooting guide if there are complications at this stage.

##Download Max:
Go to Cycling 74 and download the version for your operating system.
Open Max 8 Application so the support folders you need to place the support files generate in your Documents folder.
You may want to create an account for authorization

##Download Loom Tools for Max
Go to: github.com/udellc/loomformax
Click the green button on the right to download the .zip of the repo.
Unzip
Find MaxMSP8 folder and open it
Drag the two folders: Loom2 and MPG, into the following folder on your computer: 
(User) > Documents > Max 8 > Library 

##Download JVM
In a google search, type in: download oracle java
Click Java SE Downloads
Look for Oracle JDK and click the JDK download link
Choose the operating system that matches your computer and download.

##Download the FTDI VCP drivers (virtual com port)
ftdichip.com/drivers/vcp.htm

##Download Link Tools
Max 8 application > Package manager
Top right corner, type “link” in the search bar
Install Link by Cycling ‘74
reStart and reOpen Max 8

##Quit Max
You may need to restart your computer too.

##Reopen Max8
On your computer, Open (User) > Documents > Max 8 > Library > Optron > __OptronMiniApp.maxpat
