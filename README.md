# Poffer-Game-AP
Advanced Programming course project: a card game named Poffer Game

A complete implementation of the two-player online card game "Poffer," developed using C++ and the Qt Framework. This project was created as the final project for the Advanced Programming course and features a full client-server architecture.

---

## Key Features

### Server-Side
- **Multi-threaded Architecture:** Capable of managing multiple players simultaneously, with each client handled in a separate thread.
- **Thread-Safe Communication:** Utilizes Qt's Signal & Slot mechanism and `QMetaObject` to ensure server stability.
- **Full User Management:** Includes user registration, login, profile editing, and a "forgot password" feature with persistent data storage in a JSON file.
- **Lobby and Matchmaking:** Places players in a waiting queue and automatically starts a game as soon as an opponent is found.
- **Complete Game Logic:**
    - Implementation of all game rules, including starter determination, the card drafting process, and scoring.
    - A `HandEvaluator` engine for all 10 hand ranks in the Poffer game.
    - Full tie-breaking logic for all possible scenarios.
- **Special Game State Management:**
    - Handles player disconnections and reconnections with a 60-second timer.
    - Manages player inactivity during their turn with a two-strike penalty system.
    - Stop/Resume functionality with a 20-second timer.
- **Game History:** Saves and retrieves the last 3 game results for each user.

### Client-Side
- **Graphical User Interface (GUI):** All necessary game screens and components designed and implemented with Qt Widgets.
- **Asynchronous Communication:** Stable communication with the server via `QTcpSocket` and a signal-slot-based architecture.
- **Game State Management:** Live display of the game state, cards, turns, and results received from the server.
- **User Interaction:** Graphical capabilities for card selection, game requests, pausing, and other in-game actions.

---

## Architecture & Technologies
- **Language:** C++ (using the C++17 standard)
- **Framework:** Qt 6
- **Communication Protocol:** A custom JSON-based protocol for message exchange between the client and server.
- **Data Management:** Local data storage using JSON files.
- **Design Patterns:** Utilizes the Singleton pattern for manager classes (`UserManager`, `GameManager`) and the Signal & Slot pattern for safe inter-component communication.

---

## How to Play (Using the Installers)

Instructions for setting up and playing a two-player game on a local network.

### Prerequisites
- A 64-bit Windows operating system.
- All players must be connected to the **same local network** (e.g., the same Wi-Fi router).

### Step-by-Step Guide

**Step 1: Install the Applications**
- On the computer that will act as the server, run `PofferServer_Setup.exe` and complete the installation.
- On each of the client computers, run `PofferClient_Setup.exe` and complete the installation.

**Step 2: Start the Server**
1.  The player hosting the game runs the installed **"Poffer Game Server"** application from their Start Menu.
2.  A console window will appear and display the server's local IP address. The output will look something like this:
    ```
    Server started successfully on IP: 192.168.1.5 Port: 8888
    ```
3.  **Important:** The server host must share this IP address (e.g., `192.168.1.5`) with all other players.

**Step 3: Start and Connect the Clients**
1.  Each client player runs the installed **"Poffer Game"** application from their Start Menu.
2.  The application will ask for the server's IP address. Players must enter the **IP address** provided by the server host.
3.  The port should be kept as `8888`.
4.  Click "Connect".

**Step 4: Play the Game!**
1.  Once connected, players can register for a new account or log in with an existing one.
2.  From the main menu, players can request a new game.
3.  When two players have requested a game, the server will automatically match them and the game will begin.
---

## Creators
This project was developed by **Mohammad Mahdi Salimi** and **Amir Babak Babaei** as the final project for the Advanced Programming course.
3/july/2025

