## Summary

This repository contains everything you need to build the arduino controlled electronics of my [3d printable Portal Gun](https://www.thingiverse.com/thing:6191828).

In the `main` branch here you will find my own circuit which features a USB-C rechargable LiPo battery and simply two buttons to switch between orange and blue lights. 

If you want to share an altered design then don't hesitate to send a pull request so I can see about incorporating it into a new branch and link your remix over at [Thingiverse](thingiverse.com).

**Disclaimer:** *These instructions include soldering things to a 5V powersupply (e.g. computer, USB charger) and a Lithium Polymer battery. Especially the latter one can be easily damaged and if mistreated start burning or explode. I did all of this to the best of my knowledge but I'm a hobbyist at best. So make sure you double check everything, research this yourself and ask professionals or someone who actually knows what they are doing for help if you are unsure. These batteries aren't plug-and-play. <ins>They will get damaged</ins> if you supply the wrong voltage or current. Do this at your own risk.*

## Part list
I have linked some sellers as an example so you know what to look for but you can probably find similar parts at a vendor closer to you.

| Amount | Part Number | Part | Info |
|---|---|---|---|
| 1 | 201 | [USB-C Breakout Box ](https://www.adafruit.com/product/4090) |  |
| 1 | 202 | [PowerBoost 500 Charger](https://www.adafruit.com/product/1944) | Look for adafruit distributors on the site if it's out of stock there. *Use a PowerBoost 1000 Charger if you want faster charging. Works identical and allowes higher current. I might change to it here in the future too.* |
| 1 | 203 | [3.7V LiPo Battery](https://www.adafruit.com/product/353) | Must be rated for at least 1500mA discharge rate! |
| 1 | 204 | [Arduino Nano](https://store.arduino.cc/products/arduino-nano) | Other 5V boards work too, you may have to adjust the pins in the code though. |
| 4 | 205 | [WS2812B RGB LED](https://www.amazon.de/dp/B088K8DVMQ) |  |
| 1 | 206 | [AZDelivery 5V RGB LED Ring (WS2812B)](https://www.amazon.de/dp/B07SPL2YC1) |  |
| 1 | 207 | [Male Mini-USB connector](https://www.amazon.de/dp/B00O9YN06E) |  |
| 2 | 209 | [16mm Momentary Push Button](https://amzn.eu/d/92y2J6f) |  |
| 1 | 210 | [DPDT ON/ON Toggle Switch](https://amzn.eu/d/6uNPagw) |  |
| 1 | 211 | [Colored Wires](https://amzn.eu/d/iaCihai) | Must be rated for at least 1A. Anything similar works here obviously. I use the colors in the instructions later. |
| 1 | 212.1 | [150 Ohm Resistor](https://amzn.eu/d/dEHxbVM) | Resistance needs to fit the red LED below with ``U_in = 5V`` |
| 1 | 212.2 | [120 Ohm Resistor](https://amzn.eu/d/1QdeTb7) | Resistance needs to fit the green LED below with ``U_in = 5V`` |
| 1 | 213 | [5mm RGB LED](https://amzn.eu/d/5IawEnR) | You can alternatively use a single ([red](https://amzn.eu/d/4peTtBC)) led. |
| 1 | 214 | [PCB Solder Board](https://amzn.eu/d/1guBOHR) |  |

## Assembly
Here is a schematic for the entire circuit. The colors here are mostly the same as in the photos (although I mistakenly mixed them up in some places). I recommend using the lengths given for each wire. They already include 10-15mm of slack.

**Important: You can solder the entire circuit before assembly <ins>except for the LED Ring to the right and the two pushbuttons</ins>**. Only solder those in after screwing the buttons into the *003_Hatch* and laying the wires through the *011_Middle-Channel*.

![Diagram](https://github.com/EVARATE/Portal-Gun-OS/blob/main/images/Circuit%20Diagram.svg)

The following pictures are outdated and will be updated in the future. The diagram always shows the correct wiring. Note: the two connections that run through the DPDT switch must be open/closed inversely to eachother. Meaning that if one is open then the other must be closed.

### Top view

![Top view](https://github.com/EVARATE/Portal-Gun-OS/blob/main/images/PGV2_Circuit-1.jpg)
*Note: This image shows an outdated version of the circuit and is not accurate. Refer to the diagram above for the correct wiring!*

### Bottom view

![Bottom view](https://github.com/EVARATE/Portal-Gun-OS/blob/main/images/PGV2_Circuit-2.jpg)
*Note: This image shows an outdated version of the circuit and is not accurate. Refer to the diagram above for the correct wiring!*

Solder the remaining wires according to the schematic (**except for the LED_Ring and the two pushbuttons!**). The picture below shows my circuit again (ignore the missing WS2812B LEDs and the connectors at the pushbutton, those were just for testing). Also solder the wires as perpendicular to the WS2812B LEDs as possible or they won't fit into the printed parts later.

![Full circuit](https://github.com/EVARATE/Portal-Gun-OS/blob/main/images/PGV2_Circuit-3.jpg)
*Note: This image shows an outdated version of the circuit and is not accurate. Refer to the diagram above for the correct wiring!*

![LED Solder](https://github.com/EVARATE/Portal-Gun-OS/blob/main/images/PGV2_Circuit-4.jpg)


## See if it works
Before connecting a powersupply check all connections with a multimeter just to be safe. Also check if the wires of your battery have the same polarity as the powerboost. Some manufacturers flip them!!! Cut the battery wires individually and solder the connector on in reverse if this is the case.
Then plug in the LiPo battery and see if the board powers up. (**The PowerBoost 500 Charger requires the battery to be plugged in even if you connect the USB-C to power.**).

With the DPDT switch off all other leds should also be off. With the toggle switch on there should be the ``PWR`` or ``Chrg`` LED of the PowerBoost and some red LEDs on the arduino burning.

### Uploading the code
If you have never uploaded code to an arduino then the simplest way is probably the [Arduino Web Editor](https://create.arduino.cc/editor). Go to the link, create an account and follow the instructions. There is also a downloadable IDE and tons of guides and tutorials on youtube to get you started.

If you are using the desktop IDE you also need to include the FastLED library. You can get it via the Library Manager (``Sketch -> Include Library -> Manage Libraries``) and searching for ``fastled``. Install the package by *Daniel Garcia*.

Connect the USB-C connector to your computer. If everything is soldered correctly, the arduino should show up in the editor. If you are using an arduino nano like me you probably also need to choose ``Tools -> Processor -> ATmega328P (Old Bootloader)`` in the IDE for it to work.

![Bootloader](https://github.com/EVARATE/Portal-Gun-OS/blob/main/images/PGV2_IDE_OldBootloader.png)

Upload the code.

**Congratulations, you are done!** The two pushbuttons should switch between orange and blue. If you want to change the colors, simply change the RGB values for ``color1`` and ``color2`` in (lines 14 and 15) in the code and reupload it.

Maybe leave the circuit on for a while and keep an eye on it just to be sure that it wont explode.
