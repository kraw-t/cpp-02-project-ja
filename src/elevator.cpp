#include <stdexcept>
#include <vector>
#include <list>
#include <cstdlib>
#include <string>
#include "human.hpp"
#include "elevator.hpp"

Elevator::Elevator(int floors, int weight) : num_of_floor(floors), max_load_kg(weight) {
  this->current_state = ElevatorState::Waiting;
  this->current_floor = 0;
}

void Elevator::move() {
  if (this->current_state == ElevatorState::Rising) {
    this->current_floor++;
  } else if (this->current_state == ElevatorState::Descending) {
    this->current_floor--;
  } else {
    // ElevatorState::Waitingのときは移動しない
  }
}

int Elevator::get_current_floor() const {
  return this->current_floor;
}

ElevatorState Elevator::get_current_state() const {
  return this->current_state;
}

void Elevator::determine_next_action(std::vector<Human*> p_humans) {
  int nearest_floor = -1;  // 待ち人なしを判断するため無効値で初期化
  int min_difference_floor = this->num_of_floor + 1;
  ElevatorState next_state_candidate = ElevatorState::Waiting;

  // エレベーターを待っている人の中で最も現在階に近い人を探す  
  for (const Human* p_human : p_humans) {
    if (((p_human->get_human_state() == HumanState::UpWaiting)
      || (p_human->get_human_state() == HumanState::DownWaiting))) {
      // 現在階からの近さを判定
      if (std::abs(p_human->get_floor_original() - this->current_floor) < min_difference_floor) {
        nearest_floor = p_human->get_floor_original();
        if (nearest_floor == this->current_floor) {
          if (p_human->get_human_state() == HumanState::UpWaiting) {
            next_state_candidate = ElevatorState::Rising;
          } else {
            next_state_candidate = ElevatorState::Descending;
          }
        } else if (nearest_floor > this->current_floor) {
          next_state_candidate = ElevatorState::Rising;
        } else {
          next_state_candidate = ElevatorState::Descending;
        }
        min_difference_floor = std::abs(p_human->get_floor_original() - this->current_floor);
      }
    }
  }
  this->current_state = next_state_candidate;
}

std::string Elevator::get_current_state_str() const {
    if (this->current_state == ElevatorState::Rising) {
        return "Rising";
    } else if (this->current_state == ElevatorState::Descending) {
        return "Descending";
    } else {
        return "Waiting";
    }
}
