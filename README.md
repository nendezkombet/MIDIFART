# MIDIFART
Pocket midi controller

MIDIFART is a pocket midi controller designed for Arduino Promini that supports five analog potentiometer, 10 buttons, 1 bank button, some modules and midi output capability via 3.5mm TRS cable for sending data out to hardware synthesizer or midi device, and some modules supported. For programing code can be done with arduinoIDE. The PCB board also support Arduino Promicro as midi device for software controller but sadly you don't have full features like some optional module but it is more than enough for backing up your task. 

Designer and maintainer: [sandipratama/nendezkombet](https://github.com/nendezkombet) 


## Table of contents

- [Features](#features)
- [Midifart](#midifart)
- [PCB](#pcb)
- [BOM](#bom)
- [Case](#case)
- [Assembly Guide](#assembly-guide)


## Features

- Low cost build.
- Multi layout support.
- Easy to source components.
- Easy to build.
- Arduino Promini and Promicro compatible.
- 10 buttons with RGB support.
- 6 analog potentiometer support.
- Midi out support.
- Oled lcd support.
- Battery powered support ( as portable controller ).
- And more.


## Midifart




![IMG_E0225](https://user-images.githubusercontent.com/82454371/181727217-5c7a5eb6-eed3-4aac-b19c-c6cd6090f2f0.JPG)

![IMG_0154](https://user-images.githubusercontent.com/82454371/181727372-c23f0387-d0a2-4c00-81cf-646f6761c3d1.JPG)



## PCB


Top view

![IMG_E0231](https://user-images.githubusercontent.com/82454371/181727261-aae71024-72e2-41a2-b004-442f107a5531.JPG)

Bottom view

![IMG_E0233](https://user-images.githubusercontent.com/82454371/181727273-33d6a100-eff5-4c76-a92e-6eb743b40c56.JPG)


## BOM

|Parts|Footprint|Quantity|
|:---|:---|:---|
|WS2812B RGB LED |5050|10|
|100nF capacitor|0805|10|
|12mm tactile switch |4 pin|10|
|12mm tactile switch caps ||10|
|6x10mm tactile switch|4 pin|1|
|Arduino Promini or Promicro |3.3v or 5v|1|
|10K pullup resistor (optional)|axial or 0805|11|     
|220ohm resistor (depend on mcu voltage)|axial or 0805|4| 
|10ohm resistor (depend on mcu voltage)|axial or 0805|1|   
|22ohm resistor (depend on mcu voltage)|axial or 0805|1|   
|B10K Analog potentiometer | RV09 or RK09|5|
|3.5mm TRS female socket |PJ313|1|
|Micro slide switch (optional)|MSS22D18 |1|
|3.7v-9V battery pack (depend on mcu voltage)||1|
|8mm M3 "MALE TO FEMALE" brass standoff||4|
|12mm M3 "FEMALE TO FEMALE" brass standoff||4|
|6mm M3 screw|-|8|
|SSD1306 oled lcd (optional)|0.96 inch|1|   
|HC-SR04 (optional)|distance module |1|     
|3mm led (optional)|mono color|2|     
|2pin molex socket|2.54 pitch|1|
|3.5mm TRS jack to 5-pin midi din male socket|see detail below|1|


![51kXivjBTuL _AC_SX425_](https://user-images.githubusercontent.com/82454371/150636116-4ee8e17d-2fe3-4c75-84c9-792c8be12903.jpg)

you can build by your self, wiring instruction can by found out there.

Male TRS jack to female TRS jack socket wiring  :

|Male|Female|
|:---|:---|
|Tip | VCC |
|Ring | TX |
|Sleeve  | GROUND |


Because there are a few Promini variant, so for this project i recomended using compatible Promini variant and pinout configuration like in the picture below :

![ha-7006-027-2](https://user-images.githubusercontent.com/82454371/181730032-678429c9-06e7-40b8-9d09-194ea29de846.jpg)

If you decide use Arduino Promini 3.3v or 5v you will need USB to TTL converter like in picture below :

"DO NOT FORGET CHECKING THE VOLTAGE SWITCH MUST BE MATCHING WITH YOUR MCU OPERATING VOLTAGE !!!"

![ft232rl-ftdi-usb-zu-ttl-serial-adapter-modul-33v-5v](https://user-images.githubusercontent.com/82454371/181732610-d8e49c6d-bbf7-47dc-a0cb-466c89548d70.jpg)


## Case

Use 3mm acrylic plate like pictures above.

## Assembly Guide

See inside "case" folder !!!


