# C-Calculator

A full-stack calculator application featuring a custom-built C backend web server and a vanilla HTML/CSS/JavaScript frontend. 

## Project Structure
- **Frontend:** HTML5, CSS3 (CSS Grid), JavaScript (Fetch API)
- **Backend:** C (Custom Socket HTTP Server)

## Prerequisites
To run this project, you must have the GCC compiler installed on your system.

## How to Run

1. **Start the Backend Server**
   Open your terminal, navigate to the project folder, and compile the C server:
   ```bash
   gcc backend/main.c -o backend/calc_server
   ./backend/calc_server