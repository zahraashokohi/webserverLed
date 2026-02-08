# STM32 HTTP Web Server Project

This project implements a basic **HTTP Web Server** using an STM32 microcontroller. It features dynamic IP display on an **LCD** and serves custom web pages stored in the internal flash memory.

## 🛠 Prerequisites & Tools
* **STM32CubeMX:** Version 6.2.1
* **Toolchain:** Arm Keil MDK, STM32CubeIDE, or Makefile (as configured)
* **Middlewares:** LwIP stack for HTTP protocol

## 📂 Project Structure
* `webserver/`: Contains the source HTML, CSS, and Image files.
* `Core/Inc/`: Project header files, including the generated web data.
* `makefsdata.exe`: Utility to convert web files into C code.

## 🚀 Setup & Deployment

### 1. Web Content Preparation
All your frontend files (e.g., `index.html`, `style.css`) must be placed inside the **webserver** directory.

### 2. Generating the Header File
Since the microcontroller cannot read a file system directly, you need to convert the files into a C-header format:
1. Run the `makefsdata.exe` file.
2. A file named `fsdata.c` will be generated in the directory.

### 3. Integration
To make the data accessible to the project:
1. Move the generated `fsdata.c` to the `Core/Inc` folder.
2. **Rename** `fsdata.c` to **`MyWeb.h`**.

## 🖥 Hardware Interface
* **LCD Display:** Once the system boots and connects to the network, the assigned **IP Address** will be displayed on the LCD screen.
* **Accessing the Server:** Open a web browser on any device in the same network and type the displayed IP address.

---
