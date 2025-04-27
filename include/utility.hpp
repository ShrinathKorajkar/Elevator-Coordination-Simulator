#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <string>
#define DOTTEDLINE "-----------------------------------------------------------\n"
#define DOUBLELINE "===========================================================\n"

void printError(const char *message);

void printHeading(const std::string heading);

void takeInput(int &noOfFloors, int &noOfLifts, int &noOfBasements);

#endif // !UTILITY_HPP