# Hotel Management System

## Introduction
This is a simple hotel management system that allows users to add rooms, search for rooms, and display detailed information about the rooms in the hotel. The project is developed in C++ and uses text files to store room and hotel information.

## Key Features
- **Add Room**: Allows the addition of new rooms to the system.
- **Display Room List**: Displays all rooms in the hotel with their details.
- **Search Room**: Search for a room by its room number.
- **Save and Load Data**: Save room information to a file and load it when needed.

## Data Structures
### `Date`:
- Stores information about the day, month, and year.
- Validates the date and calculates the number of days from a given date.

### `Room`:
- Stores information about the room number, status (K: Not in use, T: In use, G: Discounted), price, and start and end dates.
- Calculates the number of days and the total room price.

### `Hotel`:
- Stores hotel information including name, address, and the list of rooms in the hotel.

## Setup and Usage

1. **Installation**:
   - Download or clone the source code.
   - Compile the code using a C++ compiler.
   - Ensure that the `data.txt` file exists in the same directory as the program.

2. **Running the Program**:
   - Run the program from the command line or through your development environment.
   - The main menu will display options for managing rooms and the hotel.

## Features
- **Menu Options**:
  - Add a new room to the hotel.
  - Display all rooms in the hotel.
  - Search for a specific room.
  - Update room status (e.g., change from available to occupied).
  - Save and load room data to/from a file.

## Example Workflow
1. Launch the program.
2. Choose an option from the main menu:
   - Add a new room.
   - View all rooms.
   - Search for a room by its number.
3. The system will automatically save any changes to the room information in the `data.txt` file.
4. Upon restarting the program, room data will be loaded from the file.

## License
This project is open-source and available under the MIT License.

