# DT Balanka - Table Soccer Observability

## Overview
Welcome to the DT Balanka repository, a proof of concept project developed by Team Luminux during the Dynatrace Innovation Day in January 2024 in order to take the spirit of playing table soccer at Dynatrace to a new level. Our project combines hardware and software elements to track and display game scores and player statistics for Balanka (table soccer).

![DT Balanka Hardware](./dt-balanka-hardware.jpg?raw=true "DT Balanka Hardware")

## Project Components

### Hardware
- **ESP32 Device**: A compact, USB-powered, and WiFi-enabled microcontroller that forms the core of our hardware setup.
- **RFID Sensors**: Two sensors connected to the ESP32, allowing players to identify themselves using employee tags or custom RFID tags.
- **OLED Display**: 128x32px mini OLED display.
- **Connectivity**: The ESP32's WiFi capabilities are utilized to connect the hardware with our web application.
- **Buttons**: Four simple buttons for user input.

### Software
- **ESP32 Arduino App**: Arduino C++ application for the ESP32 and the sensors using the VSCode PlatformIO plugin.
- **Supabase Backend**: Our backend infrastructure is built on Supabase, ensuring robust data management and seamless integration with our frontend.
- **React Frontend**: A user-friendly web application developed using React, showcasing game results and detailed player statistics.
- **PHP Supabase Proxy**: As we did not know if the used HTTP library on the ESP32 will support HTTPS, we build a simple PHP API proxy (which wouldn't have been needed).

## Features

- **Player Identification**: Utilizes RFID technology for player authentication and identification.
- **Score Tracking**: Players can input scores directly through the hardware interface, which are then reflected on the web application.
- **Game Management**: The display and buttons on the device enable players to start games, enter scores, and submit them to the backend.
- **Statistical Insights**: The web application provides statistics and insights about game performances, scores, and player rankings.

# User Interface and process

- Mini display with 4 Buttons
- 5 Screens
  - **Welcome** (fetching player list over REST call)
  - **Register Players** (switch between players 1-4, hold tag near RFID scanner to identify, proceed to team overview when ready)
  - **Team Overview** (confirm team configuration or go back)
  - **Scores** (show and set team scores)
  - **Submit Scores?** (confirm or go back)
  - **Submission result** (success or error screen, any key goes back to welcome)

## Getting Started

To get started with the DT Balanka project:

1. **Clone the Repository**: Clone this repository to your local machine.
2. **Hardware Setup**: Assemble the ESP32, RFID sensors, buttons and display.
3. **Software Installation**: Install the necessary software dependencies for both the backend and frontend components.
4. **Configuration**: Configure the ESP32 with your WiFi credentials and the API URLs.
5. **Running the App**: Launch the React web application to view game data and statistics.

## Contributions

We welcome contributions to the DT Balanka project! If you have ideas for improvements, bug fixes, or new features, please feel free to submit a pull request or open an issue.

## Support

For support, please open an issue in the repository, and we will assist you as soon as possible.

Thank you for your interest in the DT Balanka project. Let's elevate the table soccer experience together! 🚀

