# Stinkpot
## Overview
### About the Project
Stinkpot is CSMA Technology's endeavor in building an affordable, easy-to-use, and fully smart-home integrated automatic pet feeder. We wanted to find a solution that not only costs less than the existing smart pet feeders out there, but that also works seamlessly with any Smart Home, whether you're in an Amazon, Google, Apple, or other household. 

### Built With
* Arduino ESP8266 Board
* Controller written in C++
* Coming Soon: Integration with Matter for smart home plugins

## Getting Started
### Prerequisites
* Have an ESP8266 Arduino Board or likewise, that you can flash the program onto (must be Wi-Fi enabled)
* Platform IO or VSCode to edit and upload the program to the board
    * You may have to install necessary drivers for the board onto your machine

### Install & Run
1. Clone the repo `git clone git@github.com:CSMA-Technology/stinkpot-controller.git`
2. Open `main.cpp` and change line 30 and update your own WiFi details.
3. Compile and Flash the program onto your board. 
    * If using VSCode, install the PlatformIO extension and use the built in tools to upload the code to the board. 
4. The server should be accepting requests at `http://esp8266.local/motor/spin`
    * If you are unable to hit this url, try hitting your local IP address instead.
5. To spin the attached servo motor (**should be a continuous 360 motor**), send a `GET` request to the server with the following params: 
    * `servoSpeed` - what speed you would like to turn the motor at. This value also determines the direction of the spin.
    * `spinTime` - how long you would like the motor to spin in milliseconds. 
    * The full request should look something like this: 
    `http://esp8266.local/motor/spin?servoSpeed=45&spinTime=3000`

## Contributing
If you would like to work with us on this project, or have suggestions on how to make any part of the game better, please fork our repo and create a pull request, and please reach out to us using the contact info below. Thank you!!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feat/super-cool-feature`)
3. Commit your Changes (`git commit -m 'Added something super cool'`)
4. Push to the Branch (`git push origin feat/super-cool-feature`)
5. Open a Pull Request

## Contact Us
This project is part of CSMA Technology. To reach out to us, feel free to use the contact form on our website here: https://csma.technology/contact

### Our Team
* [Stephanie Cruz](https://github.com/exscruzme) - stephanie@csma.technology
* [Adrian Moya](https://github.com/admoya) - adrian@csma.technology

## Acknowledgments
* This code is based on the first web server tutorial in this guide: https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html   