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

## How to Run

### Prerequisites
- C++17 Compiler
- Qt 6 Library (including Core, Gui, Widgets, Network modules)

### Running the Server
1. Open the `PofferServer.pro` project in Qt Creator.
2. Build the project in **Release** mode.
3. Run the application. The server will start on port `8888` and display its local IP address in the console for clients to connect.

### Running the Client
1. Open the `PofferClient.pro` project (or use the parent `Poffer-Game-AP.pro` project).
2. Run the application.
3. On the connection screen, enter the IP address displayed by the server and port `8888`.
4. Register or log in to start playing.

---

## Creators
This project was developed by **Mohammad Mahdi Salimi** and **Amir Babak Babaei** as the final project for the Advanced Programming course.
3/july/2025

