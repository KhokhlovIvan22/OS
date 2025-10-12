This repository contains laboratory work on the subject "Operating Systems"

Lab 1
Two console utilities for working with binary files:
Creator — requests information about employees from the console and generates a binary file with structured records.
Reporter — reads the binary file and produces a formatted report with aggregated statistics.  

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

- UnitTest.cpp
