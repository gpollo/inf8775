#include <transitions.hpp>

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <optional>
#include <iostream>
#include <utility>

namespace tp {

std::optional<transitions> transitions::from_file(const std::filesystem::path& path) {
  std::ifstream file(path, std::ifstream::in);
  if (file.fail()) {
    std::cerr << strerror(errno) << std::endl;
    return {};
  }

  std::optional<transitions> t = std::make_optional<transitions>();

  unsigned int note1 = 0;
  unsigned int finger1 = 0;
  unsigned int note2 = 0;
  unsigned int finger2 = 0;
  while (true) {
    if (!(file >> t->costs_[note1][finger1][note2][finger2])) {
      std::cerr << strerror(errno) << std::endl;
      return {};
    }

    finger2++;

    if (finger2 >= k_finger_count) {
      finger2 = 0;
      note2++;
    }

    if (note2 >= k_total_note) {
      note2 = 0;
      finger1++;
    }

    if (finger1 >= k_finger_count) {
      finger1 = 0;
      note1++;
    }

    if (note1 >= k_total_note) {
      break;
    }
  }

  return t;
}

unsigned int transitions::cost(unsigned int n1, unsigned int f1, unsigned int n2, unsigned int f2) const {
  return costs_[n1][f1][n2][f2];
}

unsigned int transitions::best(unsigned int n1, unsigned int f1, unsigned int n2) const {
  unsigned int next = 0;

  for (unsigned int i = 0; i < k_finger_count; i++) {
    if (costs_[n1][f1][n2][i] < costs_[n1][f1][n2][next]) {
      next = i;
    }
  }

  return next;
}

std::pair<unsigned int, unsigned int> transitions::best(unsigned int n1, unsigned int n2) const {
  unsigned int f1 = 0;
  unsigned int f2 = 0;

  for (unsigned int i = 0; i < k_finger_count; i++) {
    for (unsigned int j = 0; j < k_finger_count; j++) {
      if (costs_[n1][i][n2][j] < costs_[n1][f1][n2][f2]) {
        f1 = i;
        f2 = i;
      }
    }
  }

  return std::pair<unsigned int, unsigned int>(f1, f2);
}

} /* namespace tp */
