# DT Balanka - Table Soccer Observability

## Overview
Welcome to the DT Balanka repository, a project developed during the Dynatrace Innovation Day in January 2024 in order to take the spirit of playing table soccer at Dynatrace to a new level. Our project combines hardware and software elements to track and display game scores and player statistics in real-time.

## Project Components

### Hardware
- **ESP32 Device**: A compact, USB-powered, and WiFi-enabled microcontroller that forms the core of our hardware setup.
- **RFID Sensors**: Two sensors connected to the ESP32, allowing players to identify themselves using employee tags or custom RFID tags.
- **eInk Display**: A 2.7" WaveShare eInk display equipped with four buttons, providing an interactive interface for players.
- **Connectivity**: The ESP32's WiFi capabilities are utilized to connect the hardware with our web application.

### Software
- **Supabase Backend**: Our backend infrastructure is built on Supabase, ensuring robust data management and seamless integration with our frontend.
- **React Frontend**: A user-friendly web application developed using React, showcasing game results and detailed player statistics.

## Features

- **Player Identification**: Utilizes RFID technology for player authentication and identification.
- **Real-Time Score Tracking**: Players can input scores directly through the hardware interface, which are then reflected in real-time on the web application.
- **Game Management**: The eInk display and buttons on the device enable players to start games, enter scores, and submit them to our backend.
- **Statistical Insights**: The web application provides comprehensive statistics and insights about game performances, scores, and player rankings.

# User Interface

- 2.7" eInk display with 4 Buttons
- Screen shows initial welcome message
- Button 1: Reset the game. Players can identify and register themselves for teams A and B and names will be shown on the display accordingly. Missing identities will be anonymous players.
- Button 2: Increase score of team A, 0 to 10, then go back to 0
- Button 3: Increase score of team B, 0 to 10, then go back to 0
- Button 4: Pressed once questions to submit the current score, press again to submit the score to Supabase

## Getting Started

To get started with the DT Balanka project:

1. **Clone the Repository**: Clone this repository to your local machine.
2. **Hardware Setup**: Assemble the ESP32, RFID sensors, and eInk display as per the provided schematics.
3. **Software Installation**: Install the necessary software dependencies for both the backend and frontend components.
4. **Configuration**: Configure the ESP32 with your WiFi credentials and connect it to the Supabase backend.
5. **Running the App**: Launch the React web application to view real-time game data and statistics.

## Contributions

We welcome contributions to the DT Balanka project! If you have ideas for improvements, bug fixes, or new features, please feel free to submit a pull request or open an issue.

## Support

For support, please open an issue in the repository, and we will assist you as soon as possible.

Thank you for your interest in the DT Balanka project. Let's elevate the table soccer experience together! 🚀
