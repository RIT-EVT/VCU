# REV3-VCU

## Introduction
The Vehicle Control Unit is responsible for controlling and error-checking almost the entire bike. It acts as a bridge
between the powertrain CAN and the accessory CAN. It will have two STMs, a main one (MCUC) and one that exists just for 
error handling (Hardmon). It will communicate with pretty much every board on the bike in order to make sure they are in
good working order and tell them what to do. It will run a Simulink model to handle all the application-level logic for 
controlling the bike. It will primarily take input from the Handlebar Interface Board and use them to decide a torque to
send to the motor controller. It will also command the LVSS in order to the bike through its boot-up and shutdown procedures.
