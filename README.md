# FOTA Project For IMT Wazeefa Tech Scholarship

## Overview

This project demonstrates a comprehensive Firmware Over-The-Air (FOTA) update system, combining a server-side application, NodeMCU, and STM32 microcontrollers. FOTA is essential for updating embedded systems remotely, ensuring that devices in the field can receive firmware updates without requiring physical access.

## Features

- **Web Dashboard**: A user-friendly web interface to manage firmware uploads and device updates.
- **Server-Side Application**: Handles file uploads, manages firmware versions, and communicates with the embedded devices.
- **NodeMCU Integration**: Acts as a bridge between the server and the STM32 microcontroller.
- **STM32 Firmware**: Receives and applies firmware updates from the NodeMCU.
- **Security**: Ensures secure transmission of firmware updates to prevent unauthorized access.

## Getting Started

### Prerequisites

- NodeMCU
- STM32 Microcontroller
- Web Server (e.g., Apache, Nginx)
- PHP for server-side scripting

### Installation

1. **Clone the repository**:
    ```bash
    git clone https://github.com/ahmedsamy207/IMT_WT_FOTA_PROJECT.git
    cd IMT_WT_FOTA_PROJECT
    ```

2. **Set up the web server**:
    - Configure your web server to serve the files in the `Web Server` directory.
    - Ensure PHP is installed and configured.

3. **Upload the web dashboard**:
    - Copy the contents of the `web Server` directory to your web server's root directory.

4. **Configure NodeMCU**:
    - Flash the NodeMCU with the provided firmware in the `nodemcu` directory.
    - Configure the NodeMCU to connect to your server.

5. **Set up STM32**:
    - Flash the STM32 with the initial firmware provided in the `stm32` directory.

### Usage

1. Access the web dashboard via your web browser.
2. Use the dashboard to upload new firmware files.
3. Select the desired firmware version and update your devices over-the-air.

## Project Structure

- **web**: Contains the HTML, CSS, and JavaScript files for the web dashboard.
- **server**: Includes server-side scripts (PHP) for handling file uploads and device communication.
- **nodemcu**: Source code and firmware for the NodeMCU.
- **stm32**: Source code and firmware for the STM32 microcontroller.
  
## Demo Video

[Google Drive](https://drive.google.com/file/d/1VjCncpm-L4wH0LUv51nTvP535gYxhjUJ/view?usp=drivesdk)

## Team

- **Ahmed Samy Kamal** - [LinkedIn](https://www.linkedin.com/in/ahmedsamy207)
- **Philopateer Gayed** - [LinkedIn](https://www.linkedin.com/in/philopateer-gayed-284873242/)
- **Ahmed Megahed** - [LinkedIn](https://linkedin.com/in/ahmed-megahed-351142196/)

