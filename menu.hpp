#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <string>

// Removed inline, added std:: qualifications
std::string start() {
    std::cout << "Enter the name of input file" << std::endl;
    std::string file_name;
    std::cin >> file_name;
    return file_name;
}

// Removed inline, added std:: qualifications
void afterReadingFile() {
    std::cout << "Choose the option :" << std::endl;
    std::cout << "1) Add new process to the CPU." << std::endl;
    std::cout << "2) Delete a process from the CPU." << std::endl;
    std::cout << "3) Run the processes according to scheduler for N Quantum time(user input) and show the remaining time of processes." << std::endl;
    std::cout << "4) Change priority of a process" << std::endl;
    std::cout << "5) Exit." << std::endl;
}

#endif