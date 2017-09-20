# Smart-Fan-Controller
Smart Fan Controller C++ Code for the Smart Fan project proposed by Fisher &amp; Paykel Appliances.
This project was designed for use on the ATtiny841 Atmel Microcontroller.

## Dependencies
This Project depends on the following Libraries:
- [TinyJSONpp](https://github.com/hydroflax/tinyjsonpp)
- [Atmel PID C Code](http://www.atmel.com/Images/Atmel-2558-Discrete-PID-Controller-on-tinyAVR-and-megaAVR_ApplicationNote_AVR221.pdf)

TinyJSONpp was chosen as it is a memory friendly JSON parsing library. The Atmel PID Controller code was chosen for similar reasons (the main one being that this implementation of a PID controller uses Unsigned Integers rather than floats, which many other PID Controller Libraries use. This data type choice makes the library memory footprint much lower than the other libraries that use floating point variables).

## Build
You need to use Atmel Studio 7 in order to build and program the ATtiny. 
  1. Set the compiler optimisation to -Os (Project -> Toolchain -> Optimisation Level: -OS)
  2. Clear the CLKDIV8 fuse bit of the ATtiny 841 in programming settings
  3. Build the project and then program the ATtiny using the Atmel ISP programmer
