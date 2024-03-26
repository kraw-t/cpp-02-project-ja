#include "simulator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <thread>
#include <chrono>
#include "elevator.hpp"
#include "human.hpp"

std::vector<Elevator*> p_elevators;
std::vector<Human*> p_humans;
std::vector<std::vector<int>> sim_inputs;

std::ifstream openFile(const std::string& filename);

Simulator::Simulator(int millisecondsDelay) : millisecondsDelay(millisecondsDelay) {}

void Simulator::initializeFromFile(const std::string& filename) {
    auto file { openFile(filename) };
    parseElevators(file);
    parsePassengers(file);
    parseInputs(file);
}

std::ifstream openFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Exception! Cannot open file!");
    }

    return file;
}

void Simulator::parseElevators(std::ifstream& file) {
    std::string line;
    std::getline(file, line);
    std::istringstream iss { line };

    int numberOfElevators {};
    int numberOfFloors {};
    int elevatorCapacity {};

    if (!(iss >> numberOfElevators >> numberOfFloors >> elevatorCapacity)) {
        throw std::runtime_error("Exception! Cannot parse the first line of the file!");
    }

    for (int i = 0; i < numberOfElevators; i++) {
        Elevator* p_elevator = new Elevator(numberOfFloors, elevatorCapacity);
        p_elevators.push_back(p_elevator);
    }
}

void Simulator::parsePassengers(std::ifstream& file) {
    std::string line;
    std::getline(file, line);
    std::istringstream iss { line };

    int numberOfPassengers {};

    if (!(iss >> numberOfPassengers)) {
        throw std::runtime_error("Exception! Cannot parse the number of passengers!");
    }

    while (numberOfPassengers-- && std::getline(file, line)) {
        iss = std::istringstream { line };

        int id;
        int weight;

        if (!(iss >> id >> weight)) {
            throw std::runtime_error("Exception! Cannot parse the remaining lines of the file!");
        }

        Human* p_human = new Human(id, weight);
        p_humans.push_back(p_human);
    }
}

void Simulator::parseInputs(std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        auto iss = std::istringstream { line };

        int time {};
        int id {};
        int pickUpFloor {};
        int dropOffFloor {};

        if (!(iss >> time >> id >> pickUpFloor >> dropOffFloor)) {
            throw std::runtime_error("Exception! Cannot parse the remaining lines of the file!");
        }

        sim_inputs.push_back({time, id, pickUpFloor, dropOffFloor});
    }
}

void Simulator::run() {
    for (uint16_t time = 0; time < 60; time++) {  // ミスによる無限ループ回避のためfor文使用

        // 現在の状態を表示する
        std::cout << "Step: " << time << ", ";
        std::cout << "Floor: " << p_elevators[0]->get_current_floor() << ", State: " << p_elevators[0]->get_current_state_str() << ", ";
        std::cout << "On boarding: ";
        for (Human* p_human : p_humans) {
            if (p_human->get_human_state() == HumanState::OnBoarding) {
                std::cout << p_human->get_id() << " ";
            }
        }
        std::cout << std::endl;

        // 現時刻に入力すべき情報を確認
        for (int i = 0; i < sim_inputs.size(); i++) {
            // 誰かがエレベーターを呼ぶ時刻であれば、IDが一致するHumanがエレベーターを呼ぶ
            if (sim_inputs[i][0] == time) {
                for (Human* p_human : p_humans) {
                    p_human->determine_call_elevator(sim_inputs[i][1], sim_inputs[i][2], sim_inputs[i][3]);
                }
            }
        }

        // 人が降りる
        for (Human* p_human : p_humans) {
            p_human->determine_get_off_elevator(p_elevators[0]->get_current_floor(), p_elevators[0]->get_current_state());
        }

        // 現在乗っている人が居る場合は、現在の上昇下降状態を維持、いない場合は次の行動(上昇/下降/待機)を決める
        bool is_passenger_exist = false;
        for (Human* p_human : p_humans) {
            is_passenger_exist |= (p_human->get_human_state() == HumanState::OnBoarding);
        }
        if (!is_passenger_exist) {
            // 居ない場合の次の行動を決める
            // エレベーターを呼んで待っている人が居るなら、最も近い階で呼んでいる人のところへ行く行動をとる
            // 呼び方によって近い階が変わってエレベーターが上下するが一旦無視
            p_elevators[0]->determine_next_action(p_humans);
        }

        // 人が乗る（重量制限があるので、全員降りてから乗るようにfor分を分ける）
        for (Human* p_human : p_humans) {
            p_human->determine_board_elevator(p_elevators[0]->get_current_floor(), p_elevators[0]->get_current_state());
        }

        // エレベーターを動かす
        p_elevators[0]->move();

        // Sim終了を判定する
        // if (!is_passenger_exist)
        // 条件作りこめてない

        // オプション指定のステップ間遅延
        std::this_thread::sleep_for(std::chrono::milliseconds(this->millisecondsDelay));
    }
    

    // delete all instances
    for (Elevator* p_elevator : p_elevators) {
    //    std::cout << "num_of_floor: " << p_elevator->num_of_floor << ", max_weight: " << p_elevator->max_load_kg << std::endl;
        delete p_elevator;
    }
    for (Human* p_human : p_humans) {
    //    std::cout << "id: " << p_human->id << ", weight: " << p_human->weight << std::endl;
        delete p_human;
    }
}
