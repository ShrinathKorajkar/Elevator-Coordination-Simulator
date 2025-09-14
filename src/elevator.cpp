#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <regex>
#include "elevator.hpp"

void Elevator::moveUp(const int &totalFloors, const int &totalBasements)
{
    if (currentFloor == 0 && upDirectionRequests.size() > 0 && *upDirectionRequests.begin() == 0)
    {
        upDirectionRequests.erase(currentFloor);
        if (upDirectionRequests.size() == 0)
        {
            return;
        }
    }
    currentDirection = 1;
    auto endRequest = *(upDirectionRequests.rbegin());
    while (currentFloor < totalFloors && currentFloor < endRequest && !stopElevator)
    {
        std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC));
        currentFloor++;
        if (upDirectionRequests.count(currentFloor) == 1)
        {
            upDirectionRequests.erase(currentFloor);
            if (currentFloor != 0)
            {
                elevatorMoving = false;
                std::this_thread::sleep_for(std::chrono::seconds(WAITING_TIME_IN_SEC));
                elevatorMoving = true;
            }
        }
        if (currentFloor == totalFloors - 1 || (upDirectionRequests.size() == 0 && currentFloor == 0))
        {
            break;
        }
        if (upDirectionRequests.size() != 0)
        {
            endRequest = *(upDirectionRequests.rbegin());
        }
    }
    moveDown(totalFloors, totalBasements);
}

void Elevator::moveDown(const int &totalFloors, const int &totalBasements)
{
    if (currentFloor == 0 && downDirectionRequests.size() > 0 && *downDirectionRequests.begin() == 0)
    {
        upDirectionRequests.erase(currentFloor);
    }

    if (downDirectionRequests.size() > 0 && *downDirectionRequests.begin() > 0)
    {
        auto endRequest = *(downDirectionRequests.rbegin());
        if (currentFloor < endRequest)
        {
            addRequest(1, endRequest);
            downDirectionRequests.erase(endRequest);
            moveUp(totalFloors, totalBasements);
        }
    }

    if (downDirectionRequests.size() == 0 && upDirectionRequests.size() == 0 && currentFloor != 0)
    {
        if (currentFloor < 0)
        {
            addRequest(1, 0);
        }
        else
        {
            addRequest(0, 0);
        }
    }

    currentDirection = 0;

    while (currentFloor > 0 - totalBasements && !stopElevator)
    {
        if ((downDirectionRequests.size() > 0 && *(downDirectionRequests.begin()) > currentFloor) || (downDirectionRequests.size() == 0 && currentFloor == 0))
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC));
        currentFloor--;
        if (downDirectionRequests.count(currentFloor) == 1)
        {
            elevatorMoving = false;
            downDirectionRequests.erase(currentFloor);
            std::this_thread::sleep_for(std::chrono::seconds(WAITING_TIME_IN_SEC));
            elevatorMoving = true;
        }
        if ((downDirectionRequests.size() == 0 && currentFloor < 0) || (currentFloor == (0 - totalBasements)))
        {
            addRequest(1, 0);
            break;
        }
    }
}

void Elevator::run(const int &totalFloors, const int &totalBasements)
{
    while (!stopElevator)
    {
        while (getTotalRequestsServing() == 0 && !stopElevator)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        isIdle = false;
        elevatorMoving = true;
        while (getTotalRequestsServing() > 0 && !stopElevator)
        {
            if (upDirectionRequests.size() > 0)
            {
                moveUp(totalFloors, totalBasements);
            }
            if (downDirectionRequests.size() > 0)
            {
                moveDown(totalFloors, totalBasements);
            }
        }
        isIdle = true;
        currentDirection = 2;
    }
}

void Elevator::stop()
{
    stopElevator = true;
}

void Elevator::addRequest(int direction, int floor)
{
    if (direction == 0 || (floor < 0 && currentFloor > floor))
    {
        downDirectionRequests.insert(floor);
    }
    else
    {
        upDirectionRequests.insert(floor);
    }
}

bool Elevator::checkIfRequestPresent(const int &floorNumber)
{
    return (upDirectionRequests.count(floorNumber) == 1 || downDirectionRequests.count(floorNumber) == 1);
}

bool Building::elevatorCanBeAssigned(const Elevator &elevator, const int &direction, const int &floorNumber)
{
    bool canBeAssigned = false;
    if (elevator.getCurrentDirection() == direction)
    {
        if (direction == 0 && (elevator.getCurrentFloor() > floorNumber || elevator.getUpRequestSize() > 0))
        {
            canBeAssigned = true;
        }
        else if (direction == 1 && (elevator.getCurrentFloor() < floorNumber || elevator.getDownRequestSize() > 0))
        {
            canBeAssigned = true;
        }
    }
    return canBeAssigned;
}

int Building::findElevatorWithLeastJobs()
{
    int leastJobsElevatorIndex = 0;
    for (auto elevatorIndex = 1; elevatorIndex < noOfElevators; ++elevatorIndex)
    {
        if (elevators[elevatorIndex].getTotalRequestsServing() < elevators[leastJobsElevatorIndex].getTotalRequestsServing())
        {
            leastJobsElevatorIndex = elevatorIndex;
        }
    }
    return leastJobsElevatorIndex;
}

void Building::printGuidelines()
{
    std::cout << "USER GUIDELINES" << std::endl;
    std::cout << DOTTEDLINE;
    std::cout << "1. Press Q to exit" << std::endl;
    std::cout << "2. Enter Elevator Request in Below Format" << std::endl;
    std::cout << "\n\tF[number]D[number]\n\n";
    std::cout << "Where" << std::endl;
    std::cout << "F[number]  ->  Floor Number" << std::endl;
    std::cout << "D[number]  ->  Direction Number (0 for Down, 1 for Up)" << std::endl;
    std::cout << "\nEg: F4D0  ->  Floor Number 4 Direction Down" << std::endl;
    std::cout << DOUBLELINE << std::endl;
}

bool Building::notInExistingRequests(const int &floorNumber)
{
    bool isNotPresent = true;
    for (auto &elevator : elevators)
    {
        if (elevator.checkIfRequestPresent(floorNumber))
        {
            isNotPresent = false;
            break;
        }
    }
    return isNotPresent;
}

void Building::addRequestToServe(const int &floorNumber, const int &direction)
{
    bool assigned = false;
    for (auto &elevator : elevators)
    {
        if (elevator.inIdleState())
        {
            elevator.addRequest(direction, floorNumber);
            assigned = true;
            break;
        }
    }
    if (!assigned)
    {
        for (auto &elevator : elevators)
        {
            if (elevatorCanBeAssigned(elevator, direction, floorNumber))
            {
                elevator.addRequest(direction, floorNumber);
                assigned = true;
                break;
            }
        }
    }
    if (!assigned)
    {
        int leastJobsElevatorIndex = findElevatorWithLeastJobs();
        elevators[leastJobsElevatorIndex].addRequest(direction, floorNumber);
    }
}

void Building::takeRealTimeInput()
{
    std::ifstream input("./input.txt");
    if (!input.is_open())
    {
        std::cout << "Error Opening input file" << std::endl;
        return;
    }
    std::string line;
    std::regex pattern("^\\s*(f)(-?\\d+)(d)(\\d+)\\s*$");
    std::regex quitePattern("^\\s*q\\s*$");
    std::smatch matches;
    int floorNumber = 0;
    int direction = 0;
    while (true)
    {
        if (std::getline(input, line))
        {
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            if (std::regex_search(line, matches, quitePattern))
            {
                stopPrinting = true;
                break;
            }
            else if (std::regex_search(line, matches, pattern))
            {
                floorNumber = std::stoi(matches[2].str().c_str());
                direction = std::stoi(matches[4].str().c_str());
                if (validRequest(floorNumber, direction) && notInExistingRequests(floorNumber))
                {
                    addRequestToServe(floorNumber, direction);
                }
            }
        }
        else
        {
            input.clear();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(INPUT_RESPONSE_TIME));
    }
}

void Building::printCurrentState()
{
    std::cout << std::endl;
    std::cout << DOTTEDLINE;
    std::cout << "CURRENT POSITION OF ELEVATORS" << std::endl;
    std::cout << DOTTEDLINE;
    std::cout << "Elevator\tPosition\t      Current State" << std::endl;
    std::cout << "\033[s";
    while (!stopPrinting)
    {
        std::cout << "\033[u";
        for (auto i = 0; i < noOfElevators; i++)
        {
            std::string message;
            if (elevators[i].inIdleState())
            {
                message = " Idle at Ground floor ";
            }
            else if (elevators[i].isMoving())
            {
                std::string currentDir = elevators[i].getCurrentDirection() == 0 ? "Down ▼" : "Up ▲";
                message = "   Moving " + currentDir;
            }
            else
            {
                message = "Waiting for Passengers";
            }
            std::cout << std::left << std::setw(3) << "" << std::setw(14) << i + 1 << std::right << std::setw(4) << elevators[i].getCurrentFloor() << std::setw(14) << "" << std::left << std::setw(50) << message << std::endl;
        }
        int remainingReqToServe = 0;
        for (auto elevator : elevators)
        {
            remainingReqToServe += elevator.getTotalRequestsServing();
        }
        std::cout << "\nRemaining Requests To Serve : " << std::setw(2) << remainingReqToServe << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << std::endl;
}

void Building::startLifts()
{
    printGuidelines();
    for (Elevator &elevator : elevators)
    {
        elevatorThreads.emplace_back(&Elevator::run, &elevator, noOfFloors, noOfBasements);
    }
    std::thread continousInput([this]()
                               { this->takeRealTimeInput(); });
    std::thread printState([this]()
                           { this->printCurrentState(); });
    continousInput.join();
    printState.join();
    stopLifts();
}

void Building::stopLifts()
{
    std::cout << "Stopping Elevators ..." << std::endl;
    for (auto &elevator : elevators)
    {
        elevator.stop();
    }
    for (auto &thread : elevatorThreads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    std::cout << "Elevators Stopped" << std::endl;
}
