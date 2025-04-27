#include <iostream>
#include "elevator.hpp"
#include "utility.hpp"

int main()
{
    int noOfFloors = 0;
    int noOfLifts = 0;
    int noOfBasements = 0;

    takeInput(noOfFloors, noOfLifts, noOfBasements);

    Building itt(noOfFloors, noOfLifts, noOfBasements);

    std::cout << "\033[2J\033[H";
    printHeading("ELEVATOR SIMULATION");
    itt.startLifts();

    std::cout << DOUBLELINE;
    return 0;
}
