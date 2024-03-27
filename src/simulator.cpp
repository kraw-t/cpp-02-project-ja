#include "simulator.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::ifstream openFile(const std::string &filename);

Simulator::Simulator(int millisecondsDelay) : millisecondsDelay(millisecondsDelay) {}

std::ifstream openFile(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Exception! Cannot open file!");
    }

    return file;
}

Building Simulator::getBuildingFromFile(std::ifstream &file)
{
    std::string line;
    std::getline(file, line);
    std::istringstream iss{line};

    int numberOfElevators{};
    int numberOfFloors{};
    int elevatorCapacity{};

    if (!(iss >> numberOfElevators >> numberOfFloors >> elevatorCapacity))
    {
        throw std::runtime_error("Exception! Cannot parse the first line of the file!");
    }
    Elevator elevator(elevatorCapacity);
    Building building(numberOfFloors, numberOfElevators, elevator);
    return building;
}

std::vector<Passenger> Simulator::getPassengersFromFile(std::ifstream &file)
{
    std::string line;
    std::getline(file, line);
    std::istringstream iss{line};
    std::vector<Passenger> passengers;

    int numberOfPassengers{};

    if (!(iss >> numberOfPassengers))
    {
        throw std::runtime_error("Exception! Cannot parse the number of passengers!");
    }

    while (numberOfPassengers-- && std::getline(file, line))
    {
        iss = std::istringstream{line};

        int id;
        int weight;

        if (!(iss >> id >> weight))
        {
            throw std::runtime_error("Exception! Cannot parse the remaining lines of the file!");
        }
        Passenger passenger(id, weight);
        passengers.push_back(passenger);
    }
    return passengers;
}

std::vector<std::map<std::string, int>> Simulator::getElevatorInfoFromFile(std::ifstream &file)
{
    std::string line;
    std::vector<std::map<std::string, int>> elevator_info_vector;
    while (std::getline(file, line))
    {
        auto iss = std::istringstream{line};

        int time{};
        int id{};
        int pickUpFloor{};
        int dropOffFloor{};

        if (!(iss >> time >> id >> pickUpFloor >> dropOffFloor))
        {
            throw std::runtime_error("Exception! Cannot parse the remaining lines of the file!");
        }
        std::map<std::string, int> elevator_info_map =
            {
                {"time", time},
                {"id", id},
                {"pickUpFloor", pickUpFloor},
                {"dropOffFloor", dropOffFloor},
                {"isPickedUp", 0},
                {"isDroppedOff", 0}};
        elevator_info_vector.push_back(elevator_info_map);
    }
    return elevator_info_vector;
}

void Simulator::run(const std::string &filename)
{
    // Initialize.
    auto file{openFile(filename)};
    Building building = getBuildingFromFile(file);
    std::vector<Passenger> passengers = getPassengersFromFile(file);
    std::vector<std::map<std::string, int>> elevator_info_vector = getElevatorInfoFromFile(file);
    Elevator elevator = building.get_elevator();
    // Main loop.
    int time_count = 0;
    std::vector<int> destinations;
    while (1)
    {
        bool finish_flag = true;
        int current_floor = elevator.get_current_floor();

        for (auto &info : elevator_info_vector)
        {
            if (info["time"] == time_count && info["isPickedUp"] == 0)
            {
                destinations.push_back(info["pickUpFloor"]);
                std::cout << "Passenger ID " << info["id"] << " called the elevator." << std::endl;
                if (elevator.get_state() == "waiting")
                {
                    elevator.set_destination_floor(destinations[0]);
                    destinations.erase(destinations.begin());
                    elevator.set_state("sending");
                }
            }

            if (time_count >= info["time"] && current_floor == info["pickUpFloor"] && info["isPickedUp"] == 0)
            {
                int passenger_weight = 0;
                Passenger picked_up_passenger(0, 0);
                for (auto &passenger : passengers)
                {
                    if (passenger.get_id() == info["id"])
                    {
                        passenger_weight = passenger.get_weight();
                        picked_up_passenger = passenger;
                        break;
                    }
                }
                if ( (elevator.get_current_weight() + passenger_weight) > elevator.get_elevator_capacity()){
                    std::cout << "Elevator is over capacity. Person ID " << info["id"] << " could not be picked up." << std::endl;
                    continue;
                }
                elevator.set_passengers(picked_up_passenger);
                elevator.set_current_weight(elevator.get_current_weight() + passenger_weight);
                std::cout << "Passenger ID " << info["id"] << " picked up." << std::endl;
                info["isPickedUp"] = 1;
                if (current_floor == elevator.get_destination_floor())
                {
                    elevator.set_destination_floor(info["dropOffFloor"]);
                    elevator.set_state("sending");
                }
                else
                {
                    destinations.push_back(info["dropOffFloor"]);
                    destinations.erase(destinations.begin());
                }
            }
            else if (current_floor == info["dropOffFloor"] && current_floor == elevator.get_destination_floor() && info["isPickedUp"] == 1 && info["isDroppedOff"] == 0)
            {
                int passenger_weight = 0;
                for (auto &passenger : passengers)
                {
                    if (passenger.get_id() == info["id"])
                    {
                        passenger_weight = passenger.get_weight();
                        elevator.remove_passengers(passenger);
                        break;
                    }
                }
                elevator.set_current_weight(elevator.get_current_weight() - passenger_weight);
                std::cout << "Passenger ID " << info["id"] << " dropped off." << std::endl;
                info["isDroppedOff"] = 1;
                if (destinations.size() == 0)
                {
                    elevator.set_destination_floor(0);
                    elevator.set_state("waiting");
                }
                else
                {
                    elevator.set_destination_floor(destinations[0]);
                    destinations.erase(destinations.begin());
                    elevator.set_state("sending");
                }
            }
            if (info["isDroppedOff"] == 0)
            {
                finish_flag = false;
            }
        }

        if (finish_flag)
        {
            std::cout << "All the persons are dropped off. End simulation." << std::endl;
            break;
        }

        if (current_floor < elevator.get_destination_floor())
        {
            elevator.set_current_floor(current_floor + 1);
        }
        else if (current_floor > elevator.get_destination_floor())
        {
            elevator.set_current_floor(current_floor - 1);
        }

        time_count++;

        // print results.
        std::cout << "Step" << time_count << ", ";
        std::cout << "current floor: " << elevator.get_current_floor() << ", ";
        std::cout << "current elevator weight: " << elevator.get_current_weight() << ", ";
        std::cout << "current number of passengers: " << elevator.get_passengers().size() << ", ";
        std::cout << "current state: " << elevator.get_state() << ", ";
        std::cout << "current destinations: " << elevator.get_destination_floor() << std::endl;
    }
}