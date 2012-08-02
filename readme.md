Leeroy: A networked CI Lamp using Arduino
=========================================

Leeroy is a quite simple Arduino ci-lamp. 

 - Fetches ip from DHCP server
 - polls RSS feed every 30 seconds
 - looks for the string (stable) in the title of the first entry in the
   feed to determine build status

Why name it Leeroy? Because we use Jenkins at work. Get it? [Leeeeeroooy Jenkins](https://en.wikipedia.org/wiki/Leeroy_Jenkins)

Hardware
--------
 - [Ethernet Pro] (https://www.sparkfun.com/products/10536) You could just use a Ethernet shield and another Arduino.
 - [Super Bright LED Red] (https://www.sparkfun.com/products/8862)
 - [Super Bright LED Green] (https://www.sparkfun.com/products/8861)
 - A breadboard, jumper wires and resistors

Schematic
---------
![Fritzing schematic](https://raw.github.com/bjartek/leeroy/master/Leeroy_bb.png)

TODO
-----
 - Move configuration of feed to fetch and duration to wait to the NET.  Query upon connection.
 - ATM i use 330 OHM resistors because it is what i have, but according to Adafruit Playground I can use 22-27 OHM for the green LED and 36-40 OHM for the red. Have ordered resistors and will play with it. 


