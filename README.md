# Drone
Drone monitoring and flight control software

This repository contains several projects to control a custom built drone. The software is split between 
multiple connected systems:
 * The on-drone software translates control inputs to actuate the propellers and manages various sensors.
 * The transmitter software allows the drone and controller to communicate with each other via radio.
 * The PC software reads a connected XBox controller and sends commands to the drone via the transmitter 
   and also provides a web-based dashboard for monitoring the statuses of all connected systems (controller,
   PC, transmitter, and drone)

The Radio folder contains radio communication examples.
