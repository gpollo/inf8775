#include <notes.hpp>

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <optional>
#include <iostream>
#include <vector>

namespace tp {

std::optional<notes> notes::from_file(const std::filesystem::path& path) {
  std::ifstream file(path, std::ifstream::in);
  if (file.fail()) {
    std::cerr << strerror(errno) << std::endl;
    return {};
  }

  unsigned int note_count = 0;
  if (!(file >> note_count)) {
    std::cerr << strerror(errno) << std::endl;
    return {};
  }

  std::optional<notes> n = std::make_optional<notes>();

  for (unsigned int i = 0; i < note_count; i++) {
    unsigned int note = 0;
    if (!(file >> note)) {
      std::cerr << strerror(errno) << std::endl;
      return {};
    }

    n->values_.push_back(note);
  }

  return n;
}

const std::vector<unsigned int>& notes::get_values() const {
  return values_;
}

std::optional<unsigned int> notes::operator[](unsigned int index) const {
  if (index >= values_.size()) {
    return {};
  }

  return values_[index];
}

unsigned int notes::size() const {
  return values_.size();
}

} /* namespace tp */
