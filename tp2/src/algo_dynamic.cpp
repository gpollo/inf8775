#include <algo.hpp>
#include <algo_dynamic.hpp>
#include <notes.hpp>
#include <transitions.hpp>

#include <array>
#include <algorithm>
#include <exception>
#include <map>
#include <utility>
#include <vector>

namespace tp {

algo_dynamic::algo_dynamic(notes n, transitions t) : algo(n, t) {}

std::pair<unsigned int, std::vector<unsigned int>> algo_dynamic::run() {
  if (notes_.get_values().size() < 2) {
    return std::make_pair(0, std::vector<unsigned int>());
  }

  std::array<unsigned int, transitions::k_finger_count> sub_results;
  for (unsigned int f = 0; f < transitions::k_finger_count; f++) {
    sub_results[f] = find_min(0, f);
  }

  int best = std::min_element(sub_results.begin(), sub_results.end()) - sub_results.begin();
  auto fingers = get_finger_list(best);

  return std::make_pair(sub_results[best], fingers);
}

unsigned int algo_dynamic::find_min(unsigned int note_index, unsigned int current_finger) {
  if (note_index == notes_.size()-1) {
    return 0;
  }

  auto key = std::make_pair(note_index, current_finger);
  auto it = results_.find(key);
  if (it != results_.end()) {
    return it->second.first;
  }

  std::array<unsigned int, transitions::k_finger_count> sub_results;
  for (unsigned int next_finger = 0; next_finger < transitions::k_finger_count; next_finger++) {
    auto n1 = notes_[note_index];
    auto n2 = notes_[note_index+1];
    if (!n1 || !n2) {
      throw std::runtime_error("unexpected missing note");
    }

    sub_results[next_finger] = transitions_.cost(*n1, current_finger, *n2, next_finger) + find_min(note_index + 1, next_finger);
  }

  int best = std::min_element(sub_results.begin(), sub_results.end()) - sub_results.begin();
  auto value = std::make_pair(sub_results[best], best);
  if (!results_.emplace(key, value).second) {
    throw std::runtime_error("key already exists in the map");
  }

  return value.first;
}

std::vector<unsigned int> algo_dynamic::get_finger_list(unsigned int current_finger) const {
  auto fingers = std::vector<unsigned int>();
  fingers.push_back(current_finger);

  for (unsigned int note_index = 0; note_index < notes_.size() - 1; note_index++) {
    auto key = std::make_pair(note_index, current_finger);
    auto it = results_.find(key);
    if (it == results_.end()) {
      throw std::runtime_error("key not found");
    }

    current_finger = it->second.second;
    fingers.push_back(current_finger);
  }

  return fingers;
}

} /* namespace tp */
