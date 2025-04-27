#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include "elevator.hpp"
#include "utility.hpp"

class TestSuitForPrint : public ::testing::Test
{
public:
    std::stringstream mockcin;
    std::stringstream mockcout;
    std::streambuf *realcin;
    std::streambuf *realcout;
    void SetUp() override
    {
        realcin = std::cin.rdbuf(mockcin.rdbuf());
        realcout = std::cout.rdbuf(mockcout.rdbuf());
    }

    void TearDown() override
    {
        std::cin.rdbuf(realcin);
        std::cout.rdbuf(realcout);
    }
};

TEST_F(TestSuitForPrint, PrintError_PrintMessage)
{
    printError("Error Message");
    ASSERT_TRUE(mockcout.str().find("Error Message") != std::string::npos);
}

TEST_F(TestSuitForPrint, PrintError_ClearErrorFlags)
{
    printError("Error Message");
    ASSERT_TRUE(mockcin.good());
    ASSERT_FALSE(mockcin.bad());
}

TEST_F(TestSuitForPrint, PrintError_NullPtr)
{
    ASSERT_NO_THROW(printError(nullptr));
}

TEST_F(TestSuitForPrint, PrintError_SpecialCharacters)
{
    ASSERT_NO_THROW(printError("!@#$%^&*()_+{}:\"<>? éèêë \\n\\t\\ \n"));
    ASSERT_TRUE(mockcout.str().find("!@#$%^&*()_+{}:\"<>? éèêë \\n\\t\\ \n") != std::string::npos);
}

TEST_F(TestSuitForPrint, PrintError_EmptyMessage)
{
    printError("");
    EXPECT_EQ(mockcout.str(), "\n");
}

TEST_F(TestSuitForPrint, PrintHeader_HeadingOddLenght)
{
    printHeading("TEST HEADINGS");
    std::string expectedOutput = "\n";
    expectedOutput += DOUBLELINE;
    expectedOutput += "---                    TEST HEADINGS                    ---\n";
    expectedOutput += DOTTEDLINE;
    EXPECT_EQ(mockcout.str(), expectedOutput);
}

TEST_F(TestSuitForPrint, PrintHeader_HeadingEvenLenght)
{
    printHeading("TEST HEADING");
    std::string expectedOutput = "\n";
    expectedOutput += DOUBLELINE;
    expectedOutput += "---                    TEST HEADING                     ---\n";
    expectedOutput += DOTTEDLINE;
    EXPECT_EQ(mockcout.str(), expectedOutput);
}

TEST_F(TestSuitForPrint, PrintHeader_LongHeading)
{
    printHeading("A TEST HEADING SO LONG THAT IT CANNOT FIT IN THE GIVEN BLOCK");
    std::string expectedOutput = "\n";
    expectedOutput += DOUBLELINE;
    expectedOutput += "A TEST HEADING SO LONG THAT IT CANNOT FIT IN THE GIVEN BLOCK\n";
    expectedOutput += DOTTEDLINE;
    EXPECT_EQ(mockcout.str(), expectedOutput);
}

TEST_F(TestSuitForPrint, PrintHeader_ExactLength)
{
    printHeading("A EXACT TEST HEADING WHICH MATCHES THE MAXIMUM LENGTH");
    std::string expectedOutput = "\n";
    expectedOutput += DOUBLELINE;
    expectedOutput += "---A EXACT TEST HEADING WHICH MATCHES THE MAXIMUM LENGTH---\n";
    expectedOutput += DOTTEDLINE;
    EXPECT_EQ(mockcout.str(), expectedOutput);
}

TEST_F(TestSuitForPrint, PrintHeader_EmptyString)
{
    printHeading("");
    std::string expectedOutput = "\n";
    expectedOutput += DOUBLELINE;
    expectedOutput += "---                                                     ---\n";
    expectedOutput += DOTTEDLINE;
    EXPECT_EQ(mockcout.str(), expectedOutput);
}

class TestSuitForInput : public ::testing::Test
{
public:
    std::stringstream mockcin;
    std::stringstream mockcout;
    std::streambuf *realcin;
    std::streambuf *realcout;
    int noOfFloors, noOfElevators, noOfBasements;
    void SetUp() override
    {
        realcin = std::cin.rdbuf(mockcin.rdbuf());
        realcout = std::cout.rdbuf(mockcout.rdbuf());
        noOfFloors = 0;
        noOfElevators = 0;
        noOfBasements = 0;
    }

    void TearDown() override
    {
        std::cin.rdbuf(realcin);
        std::cout.rdbuf(realcout);
    }
};

TEST_F(TestSuitForInput, TakeInput_PromptMessage)
{
    mockcin << "8\n2\n2\n";
    takeInput(noOfFloors, noOfElevators, noOfBasements);
    ASSERT_TRUE(mockcout.str().find("NOTE : \nEnter the No of floors greater than 1, \nElevators greater than 0, and \nBasements 0-2\n\n") != std::string::npos);
    ASSERT_TRUE(mockcout.str().find("Enter the number of floors : ") != std::string::npos);
    ASSERT_TRUE(mockcout.str().find("Enter the number of Elevators : ") != std::string::npos);
    ASSERT_TRUE(mockcout.str().find("Enter the number of Basements : ") != std::string::npos);
}

TEST_F(TestSuitForInput, TakeInput_TakeValidInput)
{
    mockcin << "8\n2\n2\n";
    takeInput(noOfFloors, noOfElevators, noOfBasements);
    ASSERT_EQ(noOfFloors, 8);
    ASSERT_EQ(noOfElevators, 2);
    ASSERT_EQ(noOfBasements, 2);
}

TEST_F(TestSuitForInput, TakeInput_InvalidFloorNumber)
{
    mockcin << "abc\n-2\n0\n1\n8\n2\n2\n";
    takeInput(noOfFloors, noOfElevators, noOfBasements);
    ASSERT_TRUE(mockcout.str().find("Invalid Input !... Please Try Again\n") != std::string::npos);
}

TEST_F(TestSuitForInput, TakeInput_InvalidElevatorNumber)
{
    mockcin << "8\nabc\n-2\n0\n2\n2\n";
    takeInput(noOfFloors, noOfElevators, noOfBasements);
    ASSERT_TRUE(mockcout.str().find("Invalid Input !... Please Try Again\n") != std::string::npos);
}

TEST_F(TestSuitForInput, TakeInput_InvalidBasementNumber)
{
    mockcin << "8\n2\nabc\n-2\n3\n2\n";
    takeInput(noOfFloors, noOfElevators, noOfBasements);
    ASSERT_TRUE(mockcout.str().find("Invalid Input !... Please Try Again\n") != std::string::npos);
}

// Building Test
class TestSuitForBuilding : public ::testing::Test
{
public:
    std::stringstream mockcin;
    std::stringstream mockcout;
    std::streambuf *realcin;
    std::streambuf *realcout;
    int noOfFloors, noOfElevators, noOfBasements;
    Building *building;
    std::fstream inputFile;
    void SetUp() override
    {
        realcin = std::cin.rdbuf(mockcin.rdbuf());
        realcout = std::cout.rdbuf(mockcout.rdbuf());
        noOfFloors = 8;
        noOfElevators = 2;
        noOfBasements = 2;
        building = new Building(noOfFloors, noOfElevators, noOfBasements);
        inputFile.open("./input.txt", std::ios::in | std::ios::out | std::ios::trunc);
    }

    void TearDown() override
    {
        std::cin.rdbuf(realcin);
        std::cout.rdbuf(realcout);
        delete building;
    }
};

class MockForBuilding_StartLifts : public Building
{
public:
    MockForBuilding_StartLifts(const int &noOfFloors, const int &noOfElevators, const int &noOfBasements) : Building(noOfFloors, noOfElevators, noOfBasements) {}
    MOCK_METHOD(void, printGuidelines, (), (override));
    MOCK_METHOD(void, takeRealTimeInput, (), (override));
    MOCK_METHOD(void, printCurrentState, (), (override));
};

class MockForBuilding_TakeRealTimeInput : public Building
{
public:
    MockForBuilding_TakeRealTimeInput(const int &noOfFloors, const int &noOfElevators, const int &noOfBasements) : Building(noOfFloors, noOfElevators, noOfBasements) {}
    MOCK_METHOD(void, addRequestToServe, (const int &, const int &), (override));
};

class MockForBuilding_AddRequestToServe : public Building
{
public:
    MockForBuilding_AddRequestToServe(const int &noOfFloors, const int &noOfElevators, const int &noOfBasements) : Building(noOfFloors, noOfElevators, noOfBasements) {}
    MOCK_METHOD(bool, validRequest, (const int &, const int &), (override));
    MOCK_METHOD(bool, elevatorCanBeAssigned, (const Elevator &, const int &, const int &), (override));
    MOCK_METHOD(bool, notInExistingRequests, (const int &), (override));
    MOCK_METHOD(int, findElevatorWithLeastJobs, (), (override));
};

TEST_F(TestSuitForBuilding, StartLifts_CheckFuncCalls)
{
    MockForBuilding_StartLifts mock(noOfFloors, noOfElevators, noOfBasements);
    EXPECT_CALL(mock, printGuidelines()).Times(1);
    EXPECT_CALL(mock, takeRealTimeInput()).Times(1);
    EXPECT_CALL(mock, printCurrentState()).Times(1);
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_PrintUserGuidelines)
{
    inputFile << "q\n";
    inputFile.close();

    building->startLifts();
    EXPECT_TRUE(mockcout.str().find("USER GUIDELINES") != std::string::npos);
}

TEST_F(TestSuitForBuilding, StartLifts_PrintCurrentStatus)
{
    inputFile << "q\n";
    inputFile.close();

    building->startLifts();
    EXPECT_TRUE(mockcout.str().find("CURRENT POSITION OF ELEVATORS") != std::string::npos);
    EXPECT_TRUE(mockcout.str().find("\nRemaining Requests To Serve : ") != std::string::npos);
}

TEST_F(TestSuitForBuilding, StartLifts_StopLifts)
{
    inputFile << "q\n";
    inputFile.close();

    building->startLifts();
    EXPECT_TRUE(mockcout.str().find("Stopping Elevators ...") != std::string::npos);
    EXPECT_TRUE(mockcout.str().find("Elevators Stopped") != std::string::npos);
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_ValidInput_SingleInput)
{
    MockForBuilding_TakeRealTimeInput mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f1d1\nq\n";
    inputFile.close();
    EXPECT_CALL(mock, addRequestToServe(::testing::_, ::testing::_)).Times(1);
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_ValidInput_BasementFloor)
{
    MockForBuilding_TakeRealTimeInput mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f-2d1\nq\n";
    inputFile.close();
    EXPECT_CALL(mock, addRequestToServe(::testing::_, ::testing::_)).Times(1);
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_ValidInput_SameFloor)
{
    MockForBuilding_TakeRealTimeInput mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f1d1\nf1d1\nf1d0\nq\n";
    inputFile.close();
    EXPECT_CALL(mock, addRequestToServe(::testing::_, ::testing::_)).Times(3);
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_ValidInput_SameDirection)
{
    MockForBuilding_TakeRealTimeInput mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f1d1\nf2d1\nf-1d1\nq\n";
    inputFile.close();
    EXPECT_CALL(mock, addRequestToServe(::testing::_, ::testing::_)).Times(3);
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_ValidInput_MulitpleInputs)
{
    MockForBuilding_TakeRealTimeInput mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f1d1\nf2d1\nf-1d1\nf-2d1\nf4d1\nf5d0\nq\n";
    inputFile.close();
    EXPECT_CALL(mock, addRequestToServe(::testing::_, ::testing::_)).Times(6);
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_InValidInput_IncorrectFormat)
{
    MockForBuilding_TakeRealTimeInput mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "asdf\nd4f1\nq\n";
    inputFile.close();
    EXPECT_CALL(mock, addRequestToServe(::testing::_, ::testing::_)).Times(0);
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_InValidInput_FloorDoesNotExist)
{
    MockForBuilding_TakeRealTimeInput mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f9d1\nf-4d1\nf9d0\nf-3d0\nq\n";
    inputFile.close();
    EXPECT_CALL(mock, addRequestToServe(::testing::_, ::testing::_)).Times(0);
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_InValidInput_InvalidDirection)
{
    MockForBuilding_TakeRealTimeInput mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f1d2\nf-1d3\nq\n";
    inputFile.close();
    EXPECT_CALL(mock, addRequestToServe(::testing::_, ::testing::_)).Times(0);
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_AddRequestToServe_ValidRequest_Idle)
{
    MockForBuilding_AddRequestToServe mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f1d1\nq\n";
    EXPECT_CALL(mock, validRequest(::testing::_, ::testing::_)).Times(1).WillOnce(::testing::Return(true));
    EXPECT_CALL(mock, notInExistingRequests(::testing::_)).Times(1).WillOnce(::testing::Return(true));
    EXPECT_CALL(mock, elevatorCanBeAssigned(::testing::_, ::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mock, findElevatorWithLeastJobs()).Times(0);
    inputFile.close();
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_AddRequestToServe_ValidRequest_NotIdle)
{
    MockForBuilding_AddRequestToServe mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f1d1\nf-1d1\nf2d1\nq\n";
    EXPECT_CALL(mock, validRequest(::testing::_, ::testing::_)).WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mock, notInExistingRequests(::testing::_)).WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mock, elevatorCanBeAssigned(::testing::_, ::testing::_, ::testing::_)).WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mock, findElevatorWithLeastJobs()).Times(0);
    inputFile.close();
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

TEST_F(TestSuitForBuilding, StartLifts_TakeRealTimeInput_AddRequestToServe_ValidRequest_NotIdle_CannotAssigned)
{
    MockForBuilding_AddRequestToServe mock(noOfFloors, noOfElevators, noOfBasements);
    inputFile << "f1d1\nf-1d1\nf2d1\nq\n";
    EXPECT_CALL(mock, validRequest(::testing::_, ::testing::_)).WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mock, notInExistingRequests(::testing::_)).WillRepeatedly(::testing::Return(true));
    EXPECT_CALL(mock, elevatorCanBeAssigned(::testing::_, ::testing::_, ::testing::_)).WillRepeatedly(::testing::Return(false));
    EXPECT_CALL(mock, findElevatorWithLeastJobs()).Times(::testing::AtLeast(1)).WillRepeatedly(::testing::Return(0));
    inputFile.close();
    mock.startLifts();
    ASSERT_TRUE(::testing::Mock::VerifyAndClearExpectations(&mock));
}

// Elevator Tests
class TestSuitForElevator : public ::testing::Test
{
public:
    std::stringstream mockcin;
    std::stringstream mockcout;
    std::streambuf *realcin;
    std::streambuf *realcout;
    int noOfFloors, noOfElevators, noOfBasements;
    Elevator *elevator;
    void SetUp() override
    {
        realcin = std::cin.rdbuf(mockcin.rdbuf());
        realcout = std::cout.rdbuf(mockcout.rdbuf());
        noOfFloors = 8;
        noOfElevators = 2;
        noOfBasements = 2;
        elevator = new Elevator(0);
    }

    void TearDown() override
    {
        std::cin.rdbuf(realcin);
        std::cout.rdbuf(realcout);
        delete elevator;
    }
};

TEST_F(TestSuitForElevator, AddRequest)
{
    elevator->addRequest(1, 5);
    elevator->addRequest(0, 5);
    elevator->addRequest(1, -2);
    ASSERT_EQ(elevator->getTotalRequestsServing(), 3);
    EXPECT_EQ(elevator->getUpRequestSize(), 1);
    EXPECT_EQ(elevator->getDownRequestSize(), 2);
}

TEST_F(TestSuitForElevator, AddRequest_EdgeFloors)
{
    elevator->addRequest(1, 7);
    elevator->addRequest(1, -2);
    ASSERT_EQ(elevator->getTotalRequestsServing(), 2);
}

TEST_F(TestSuitForElevator, ServeUpRequest)
{
    elevator->addRequest(1, 5);
    ASSERT_EQ(elevator->getTotalRequestsServing(), 1);
    EXPECT_TRUE(elevator->inIdleState());
    EXPECT_TRUE(elevator->getCurrentFloor() == 0);
    std::thread elevatorThread(&Elevator::run, elevator, noOfFloors, noOfBasements);
    std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC));
    EXPECT_EQ(elevator->getCurrentDirection(), 1);
    elevator->stop();
    elevatorThread.join();
    ASSERT_EQ(elevator->getCurrentFloor(), 1);
}

TEST_F(TestSuitForElevator, ServeDownRequest)
{
    elevator->addRequest(0, 2);
    ASSERT_EQ(elevator->getTotalRequestsServing(), 1);
    EXPECT_TRUE(elevator->inIdleState());
    EXPECT_TRUE(elevator->getCurrentFloor() == 0);
    std::thread elevatorThread(&Elevator::run, elevator, noOfFloors, noOfBasements);
    std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC * 2 + 1));
    EXPECT_FALSE(elevator->isMoving());
    EXPECT_EQ(elevator->getCurrentFloor(), 2);
    std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC + WAITING_TIME_IN_SEC - 1));
    EXPECT_EQ(elevator->getCurrentDirection(), 0);
    elevator->stop();
    elevatorThread.join();
    ASSERT_EQ(elevator->getCurrentFloor(), 1);
}

TEST_F(TestSuitForElevator, ServeBasementRequest)
{
    elevator->addRequest(1, -2);
    ASSERT_EQ(elevator->getTotalRequestsServing(), 1);
    EXPECT_TRUE(elevator->inIdleState());
    EXPECT_TRUE(elevator->getCurrentFloor() == 0);
    std::thread elevatorThread(&Elevator::run, elevator, noOfFloors, noOfBasements);
    std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC * 2 + 1));
    EXPECT_FALSE(elevator->isMoving());
    EXPECT_EQ(elevator->getCurrentFloor(), -2);
    std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC + WAITING_TIME_IN_SEC - 1));
    EXPECT_EQ(elevator->getCurrentDirection(), 1);
    elevator->stop();
    elevatorThread.join();
    ASSERT_EQ(elevator->getCurrentFloor(), -1);
}

TEST_F(TestSuitForElevator, ServeMulitpleRequest_SameDirection)
{
    elevator->addRequest(1, 1);
    elevator->addRequest(1, 2);
    ASSERT_TRUE(elevator->inIdleState());
    ASSERT_TRUE(elevator->getCurrentFloor() == 0);
    std::thread elevatorThread(&Elevator::run, elevator, noOfFloors, noOfBasements);
    std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC + 1));
    EXPECT_TRUE(elevator->getCurrentFloor() == 1);
    EXPECT_FALSE(elevator->isMoving());
    std::this_thread::sleep_for(std::chrono::seconds(WAITING_TIME_IN_SEC + SPEED_OF_LIFT_IN_SEC));
    EXPECT_TRUE(elevator->getCurrentFloor() == 2);
    EXPECT_FALSE(elevator->isMoving());
    elevator->stop();
    elevatorThread.join();
}

TEST_F(TestSuitForElevator, ServeMulitpleRequest_DifferentDirection)
{
    elevator->addRequest(1, 1);
    elevator->addRequest(0, 2);
    ASSERT_TRUE(elevator->inIdleState());
    ASSERT_TRUE(elevator->getCurrentFloor() == 0);
    std::thread elevatorThread(&Elevator::run, elevator, noOfFloors, noOfBasements);
    std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC + 1));
    EXPECT_TRUE(elevator->getCurrentFloor() == 1);
    EXPECT_FALSE(elevator->isMoving());
    std::this_thread::sleep_for(std::chrono::seconds(WAITING_TIME_IN_SEC + SPEED_OF_LIFT_IN_SEC));
    EXPECT_TRUE(elevator->getCurrentFloor() == 2);
    EXPECT_FALSE(elevator->isMoving());
    elevator->stop();
    elevatorThread.join();
}

TEST_F(TestSuitForElevator, ServeMulitpleRequest_CurrentFloor)
{
    elevator->addRequest(1, 0);
    elevator->addRequest(0, 0);
    ASSERT_TRUE(elevator->inIdleState());
    ASSERT_TRUE(elevator->getCurrentFloor() == 0);
    std::thread elevatorThread(&Elevator::run, elevator, noOfFloors, noOfBasements);
    std::this_thread::sleep_for(std::chrono::seconds(SPEED_OF_LIFT_IN_SEC));
    EXPECT_TRUE(elevator->getCurrentFloor() == 0);
    elevator->stop();
    elevatorThread.join();
}
