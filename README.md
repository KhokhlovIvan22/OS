This repository contains laboratory work on the subject "Operating Systems"

Lab 1

Two console utilities for working with binary files:
- Creator — requests information about employees from the console and generates a binary file with structured records.
- Reporter — reads the binary file and produces a formatted report with aggregated statistics.  
Both utilities are designed to demonstrate concurrent processing.


Build system: CMake  
Language standard: C++20  
Unit testing framework: GoogleTest


Key files:

- creator.cpp - implements the utility that takes a binary file name and a number of records from the command line as arguments, reads employee records from standard input, and writes them to the specified binary file.

- reporter.cpp - implements the utility that reads employee records from a binary file, sorts them, calculates wages using the given hourly rate, and writes a formatted report to the specified output file.

- main.cpp - launches the Creator and Reporter utilities as separate processes, waits for them to complete, displays the generated binary data and the final report, handles errors opening files and starting threads.

- employee.h & employee.cpp - provide the core functionality for employee data handling — define input/output operators for employee, implement comparison logic, and supply helper functions to read employee records
from a binary file and to generate a formatted salary report.

- UnitTest.cpp - implements unit tests that check core project functionality: check binary read/write of employee records, validate that generated reports contain correct salary calculations, ensure that invalid inputs are properly rejected

- helpers.h/helpers.cpp - provide utility functions for unit tests —  implement file existence and deletion checks, reading text lines into wide strings and wrapper around WinAPI process creation to launch and monitor external executables.



Lab 2

A multithreaded program for processing integer arrays:
- First thread finds the minimum and maximum values in the array.
- Second thread computes the average value.
After both threads finish, the array is modified so that all min/max values are replaced with the average.


Build system: CMake
Language standards: C++98 (WinAPI threads usage) and C++20 (std::thread)
Unit testing framework: GoogleTest


Key files:

- min_max.h / min_max.cpp — define the MinMax structure and implement the thread function that scans the int array to find minimum and maximum values.
- average.h / average.cpp — define the AverageEl structure and implement the thread function that computes the average value of the array.
- main.cpp — entry point that initializes the array, launches the MinMax and Average threads, waits for their completion, modifies the array and outputs the results.
- array_functions.h / array_functions.cpp — provide helper functions for array input/output and modification.
- tests.cpp — implements end-to-end test and unit tests that verify array input/output, correct replacement of min/max values, and correctness of mimMaxThread and averageThread computations.


Lab 3

A multithreaded console program that simulates the work of "markers" on a shared integer array:
- Each marker thread randomly selects positions in the array and marks them with its own ID.
- If a marker encounters a conflict (tries to mark an already occupied cell), it becomes blocked and waits for user input.
- The user can terminate a specific marker by ID. Upon termination, the marker cleans up all its marks from the array and finishes.
- The process continues until all markers are terminated, after which the final state of the array is displayed.

Build system: CMake  
Language standard: C++23  
Unit testing framework: GoogleTest

Key files:
- Array.h / Array.cpp — implement a thread-safe integer array with methods for marking elements, resetting marks by ID, and printing the current state.
- Marker.h / Marker.cpp — define the Marker class that runs in its own thread, performs random marking, handles blocking/unblocking, and cleans up its marks upon termination.
- main.cpp — entry point that initializes the array, creates and manages multiple markers, coordinates their blocking/unblocking, handles user input for terminating markers, and prints intermediate and final states of the array.
- end2end.cpp — provides end-to-end tests that launch multiple markers, simulate their work, terminate them, and verify that the array is correctly reset and that expected output messages are produced.
- marker_tests.cpp — unit tests for the Marker class, verifying correct state transitions (isFinished, isBlocked), proper cleanup of array elements, and correct output messages.
- array_tests.cpp** — unit tests for the`Array class, checking constructor validation, correct behavior of tryMark and reset, and ensuring that print produces the expected output.
