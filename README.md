# Linked List Library Management System

This project is a C++ application developed for the UCCD1024 Data Structures and Algorithmic Problem Solving course. It focuses on implementing and managing data using linked lists, specifically for handling student information and library book borrowings.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Error Handling](#error-handling)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Features

This application provides the following functionalities for managing student and book information:

* **Read Student Data**: Reads student information from a file into a linked list, ensuring no duplicate records. 
* **Delete Student Record**: Deletes a student record from the linked list based on their ID. 
* **Search Student**: Searches for a student by ID and displays their information if found. 
* **Insert Book**: Inserts borrowed book information for a student. This includes calculating overdue fines based on a given current date (29/3/2020) and updating the student's total fine. 
* **Display Data**: Displays student and/or book information to either the console or a file, with options for detailed or student-only views. 
* **Compute and Display Statistics**: Calculates and presents statistics on the number of students, total books borrowed, total overdue books, and total overdue fines, categorized by course. 
* **Find Students with Same Book**: Identifies and lists all students who have borrowed a specific book (based on call number), including their borrow and due dates for that book. 
* **Display Warned Students**: Identifies and categorizes students into two "warned" lists based on overdue books and total fines. 
* **Interactive Menu**: Provides a menu-driven interface for users to select and perform tasks. 

## Getting Started

### Prerequisites

* Microsoft Visual Studio 2022 

### Installation

1.  Clone the repository:
    ```bash
    git clone <repository_url>
    ```
2.  Open the project in Microsoft Visual Studio 2022. 
3.  Build the solution.

## Usage

Run the executable. The program will present a menu with options to perform various operations on the student and book data. 

Sample input files `student.txt` and `book.txt` are provided for testing.  You are encouraged to create your own test data following the specified formats to ensure robustness. 

## Project Structure

The project consists of several classes and files:

* `app.cpp`: This is the main application file where the core logic and function calls are implemented. This is the only file that can be modified. 
* `LibStudent.h`: Defines the `LibStudent` class (struct), used to store student information and an array of books they borrow. 
    * **Attributes**: `id`, `name`, `course`, `phone_no`, `total_fine`, `totalbook`, `book` (an array of `LibBook` objects). 
    * **Member Functions**: `LibStudent()`, `compareName1()`, `compareName2()`, `print()`, `calculateTotalFine()`. 
* `LibBook.h`: Defines the `LibBook` class (struct), used to store information for a library book. 
    * **Attributes**: `title`, `author[]`, `publisher`, `ISBN`, `yearPublished`, `callNum`, `fine`, `borrow` (Date object), `due` (Date object). 
    * **Member Functions**: `LibBook()`, `compareTitle()`, `compareCallNum()`, `print()`. 
* `Date.h`: Defines the `Date` struct, used to store date information. 
    * **Attributes**: `day`, `month`, `year`. 
    * **Member Functions**: `Date()`, `print()`. 
* `README.TXT`: Contains group member information (MUST be filled in). 

## Contributing

This project was developed as a university assignment. While direct contributions are not expected, any feedback or suggestions are welcome.

## License

This project is not publicly licensed as it is a university assignment.

## Acknowledgments

* Universiti Tunku Abdul Rahman Data Structure and Algorithmic Problem Solving Course Material 
* Microsoft Visual Studio 

---
**Group Members:**

* Stanley Chew Jun Xian
* Chong Jian Yi
* Fun Yong Qi
* Aiden Ting Sing Soon