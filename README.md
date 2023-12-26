# DesignProject
Smart solar powered streetlight control system

This project interfaces an ATMega328p on the Arduino platform with the following peripherals:
1) A real time clock - to track the passage of time.
2) A GSM controller - as a feedback mechanism to notify larger control systems of the state of the light.
3) Source switching relay - to switch from the solar powered battery back up to the national power grid as needed.
4) Light toggle relay - to switch the light ON and OFF as needed by the preprogrammed time schedule.
5) Day/Night switch using a Light dependent resistor - to ensure the streetlight is only ON when ambient lighting conditions require it.

These components integrate together to form a smart solar powered streetlight control system.
The programming was conducted in the Arduino IDE and the simulation was carried out in Proteus 8.
