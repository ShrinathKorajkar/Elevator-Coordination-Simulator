#include <iostream>
#include <limits>
#include "utility.hpp"
void printError(const char *message)
{
    if (message != nullptr)
    {
        std::cout << message << std::endl;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.clear();
}

void printHeading(const std::string heading)
{
    std::cout << std::endl;
    std::cout << DOUBLELINE;
    int desiredLength = 54;
    int headingLength = heading.length();
    if (headingLength <= desiredLength)
    {
        int fillLength1 = (desiredLength - headingLength) / 2 - 1;
        int fillLength2 = (desiredLength - headingLength) / 2;
        if (headingLength % 2 != 0)
        {
            fillLength1++;
        }
        std::string fill1(fillLength1, ' ');
        std::string fill2(fillLength2, ' ');
        std::cout << "---" << fill1 << heading << fill2 << "---" << std::endl;
    }
    else
    {
        std::cout << heading << std::endl;
    }
    std::cout << DOTTEDLINE;
}

void takeInput(int &noOfFloors, int &noOfLifts, int &noOfBasements)
{
    std::cout << "NOTE : \nEnter the No of floors greater than 1, \nElevators greater than 0, and \nBasements 0-2\n\n";
    while (true)
    {
        std::cout << "Enter the number of floors : ";
        std::cin >> noOfFloors;
        if (std::cin.fail() || noOfFloors < 2)
        {
            printError("Invalid Input !... Please Try Again\n");
            continue;
        }
        break;
    }
    while (true)
    {
        std::cout << "\nEnter the number of Elevators : ";
        std::cin >> noOfLifts;
        if (std::cin.fail() || noOfLifts < 1)
        {
            printError("Invalid Input !... Please Try Again\n");
            continue;
        }
        break;
    }
    while (true)
    {
        std::cout << "\nEnter the number of Basements : ";
        std::cin >> noOfBasements;
        if (std::cin.fail() || noOfBasements < 0 || noOfBasements > 2)
        {
            printError("Invalid Input !... Please Try Again\n");
            continue;
        }
        break;
    }
}
