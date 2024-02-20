# REV3-VCU

## Introduction
The Vehicle Control Unit is responsible for controlling and error-checking almost the entire bike. It acts as a bridge
between the powertrain CAN and the accessory CAN. It will have two STMs, a main one and one that exists just for error 
handling. It will communicate with pretty much every board on the bike in order to make sure they are in good working 
order and tell them what to do. It will also be responsible for running Simulink models for the TMU and the traction 
control system. It will primarily take input from the Handlebar Interface Board and communicate those inputs to the 
motor controller, and will control the LVSS in order to turn the bike on and off.
