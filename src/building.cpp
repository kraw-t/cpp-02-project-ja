#include "building.h"
#include "iostream"

Building::Building(int numberOfFloors, int numberOfElevators, Elevator elevator)
    : numberOfFloors(numberOfFloors), numberOfElevators(numberOfElevators), elevator(elevator)
{
    if (numberOfElevators != 1)
    {
        std::cout << "This simulator works only for one elevator." << std::endl;
        exit(1);
    }
}

Elevator Building::get_elevator() const{
    return elevator;
}