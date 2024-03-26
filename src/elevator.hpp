#pragma once
#include <vector>
#include <string>
#include <list>
#include "human.hpp"

// エレベーターの動作状態
enum class ElevatorState {
  Rising,      // 上昇中
  Descending,  // 下降中
  Waiting      // 待機中
};

class Human;

class Elevator {
 public:
  Elevator(int floors, int weight);
  void move();
  int get_current_floor() const;
  ElevatorState get_current_state() const;
  void determine_next_action(std::vector<Human*> p_humans);
  std::string get_current_state_str() const;

 private:
  // members
  int num_of_floor;
  int max_load_kg;
  int current_floor;
  ElevatorState current_state;
};