#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <thread>
#include <vector>
#include <set>
#include "utility.hpp"
#define WAITING_TIME_IN_SEC 10
#define SPEED_OF_LIFT_IN_SEC 4
#define INPUT_RESPONSE_TIME 2000

class Elevator
{
    int elevatorId;
    std::set<int> upDirectionRequests;
    std::set<int> downDirectionRequests;
    bool isIdle = true;
    int currentDirection = 2;
    int currentFloor = 0;
    bool stopElevator = false;
    bool elevatorMoving = false;

protected:
    void moveUp(const int &totalFloors, const int &totalBasements);
    void moveDown(const int &totalFloors, const int &totalBasements);

public:
    Elevator(const int &id) : elevatorId(id) {}
    void run(const int &totalFloors, const int &noOfBasements);
    void stop();
    void addRequest(int direction, int floor);
    bool checkIfRequestPresent(const int &floorNumber);
    int getCurrentDirection() const { return currentDirection; }
    int getCurrentFloor() const { return currentFloor; }
    int getTotalRequestsServing() const { return upDirectionRequests.size() + downDirectionRequests.size(); }
    int inIdleState() const { return isIdle; }
    bool isMoving() const { return elevatorMoving; }
    int getUpRequestSize() const { return upDirectionRequests.size(); }
    int getDownRequestSize() const { return downDirectionRequests.size(); }
};

class Building
{
    int noOfFloors, noOfElevators, noOfBasements;
    std::vector<Elevator> elevators;
    std::vector<std::thread> elevatorThreads;
    bool stopPrinting = false;

protected:
    virtual bool validRequest(const int &floorNumber, const int &direction)
    {
        return (floorNumber >= (0 - noOfBasements) && floorNumber < noOfFloors) && (direction == 0 || direction == 1);
    }

    virtual bool elevatorCanBeAssigned(const Elevator &elevator, const int &direction, const int &floorNumber);
    virtual bool notInExistingRequests(const int &floorNumber);
    virtual int findElevatorWithLeastJobs();
    virtual void printGuidelines();
    virtual void addRequestToServe(const int &floorNumber, const int &direction);
    virtual void takeRealTimeInput();
    virtual void printCurrentState();

public:
    Building(const int &noOfFloors, const int &noOfElevators, const int &noOfBasements) : noOfFloors(noOfFloors), noOfElevators(noOfElevators), noOfBasements(noOfBasements)
    {
        for (int liftNumber = 0; liftNumber < noOfElevators; liftNumber++)
        {
            elevators.emplace_back(Elevator(liftNumber));
        }
    }

    void startLifts();
    void stopLifts();
};

#endif // !ELEVATOR_HPP