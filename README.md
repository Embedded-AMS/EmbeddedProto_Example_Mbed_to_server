
![alt text](https://embeddedproto.com/wp-content/uploads/2022/04/Embedded_Proto.png  "Embedded Proto Logo")


Copyrights 2020-2024 Embedded AMS B.V. Hoorn, [www.EmbeddedAMS.nl](https://www.EmbeddedAMS.nl), [info@EmbeddedAMS.nl](mailto:info@EmbeddedAMS.nl)


Looking for a more elaborate description of this example? Please visit: Mbed OS ethernet example link


# Introduction

This repository hosts example code for Embedded Proto, the embedded implementation of Google Protocol Buffers. It is a simple example showing how a microcontroller and desktop pc can communicate over ethernet using Mbed OS as operating system for the microcontroller. 


This example makes use of a NUCLEO-F767ZI board made by ST Microelectronics. The microcontroller is based on an ARM Cortex-M4 processor. To build the source code and program the hardware Mbed Studio has been used. 
When the blue user button is pressed on the NUCLEO-F767ZI, a protobuf message is send to the PC.


# Installation

This example has been tested with both Arm Compiler version 6.14 and GNU Arm Embedded Toolchain version 9-2019-q4-major compilers.
You can use Mbed Studio or Mbed CLI to build this example. In this example we describe how to build the software using Mbed Studio.

1. Install Mbed Studio if you have not already.
2. Install the dependencies required by Embedded Proto. They are listed [here](https://github.com/Embedded-AMS/EmbeddedProto).
3. Checkout this example repository including the submodule of Embedded Proto: `git clone --recursive https://github.com/Embedded-AMS/EmbeddedProto_Example_STM32_ETH.git`.
4. Setup the environment required for Embedded Proto and the desktop script by running the setup script: `python setup.py`. This script also copies the Embedded Proto source code into the Mbed Studio project.
5. Open Mbed Studio and select the nucleo-f767zi folder as workspace (file > open workspace).
6. Checkout the mbed os library, you can do this in the libraries tab in Mbed Studio.

The setup script already does it for you but you can regenerate the source code using the `python setup.py --generate` parameter. This is required when you have changed the \*.proto file.


# Running the code

First connect an ethernet cable to the PC and NUCLEO. Then connect the NUCLEO via the usb programmer and use Mbed Studio to build and program the microcontroller on it. Next go to the desktop folder, activate the virtual environment and run the script. 
Make sure that the python server and NUCLEO-F767ZI are on the same network, such that they can talk to each other. 

```bash
cd desktop
source venv/bin/activate
python3 main.py
```
Now you can reset the NUCLEO-F767ZI, and you should see that the server lists a new connected client.
When you press the blue user button on the NUCLEO-F767ZI board, a message is sended from to board to the server. 

Have fun!
