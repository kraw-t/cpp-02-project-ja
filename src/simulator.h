#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cstddef>
#include "elevator.h"

class Simulator {
public:
    using weightMap = std::unordered_map<size_t, size_t>;

    Simulator(int millisecondsDelay);

    void initializeFromFile(const std::string& filename);
    void run();
    void printResults() const;

private:
    void parseElevators(std::ifstream& file);
    void parsePassengers(std::ifstream& file);
    void parseInputs(std::ifstream& file);
    // find an elevator to process this queue and assign passenger information to the elevator.
    void assignElevatorTo(const std::shared_ptr<Person> & person);

    int millisecondsDelay;
    std::vector<Elevator> elevators_;
    weightMap weight_map_;
    std::vector<std::shared_ptr<Person>> queue_;
    size_t current_time_{0};
};

