# AgriFlow – Smart Irrigation Advisory System

## About the Project

AgriFlow is a C-based Smart Irrigation Advisory System designed to help sugarcane farmers make irrigation decisions using sensor readings.

The application takes soil moisture, temperature, humidity, and rainfall values as input. It processes the readings and provides basic irrigation recommendations.

The project demonstrates the practical implementation of Queue and Singly Linked List data structures.

## Data Structures Used

### 1. Queue (Array Implementation)
- Stores incoming sensor readings.
- Follows FIFO (First In, First Out).
- Processes readings in the order they are received.

### 2. Singly Linked List
- Stores processed sensor records.
- Allows insertion of records at the end.
- Supports searching, displaying, and deleting records.

## Functionalities

1. Add New Sensor Reading
2. Process Incoming Sensor Data
3. Display Pending Sensor Readings
4. Display All Stored Sensor Records
5. Search Sensor Record
6. Delete Sensor Record
7. Generate Irrigation Advisory
8. Display Total Stored Records
9. Exit Application

## Technologies Used

- Programming Language: C
- Data Structures: Queue and Singly Linked List
- Compiler: GCC

## How to Run

### Step 1: Clone the Repository
git clone https://github.com/Azhan-Memon/Self-Developed-Application.git

### Step 2: Open the Project Folder
cd Self-Developed-Application

### Step 3: Compile the Program
gcc DSA_IA_Code.c -o agriflow

## Irrigation Advisory

The application provides recommendations based on sensor readings.

- Low soil moisture: Irrigation required.
- High soil moisture: Irrigation not required.
- Sufficient rainfall: Irrigation not required.
- Hot temperature and low humidity: Irrigation required.
- Other conditions: Monitor soil conditions.

Note: The thresholds used in this project are demonstration values defined in the source code. They are not validated agricultural standards.

## Future Scope

- Integration with actual agricultural sensors.
- Real-time monitoring of soil conditions.
- Improved irrigation recommendations using agricultural data.
- Development of a graphical user interface.
