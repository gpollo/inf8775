#ifndef INCLUDE_TRANSITIONS_HPP
#define INCLUDE_TRANSITIONS_HPP

#include <filesystem>
#include <optional>
#include <utility>

namespace tp {

class transitions {
  public:
    static const unsigned int k_octave_count = 2;
    static const unsigned int k_note_per_octave = 12;
    static const unsigned int k_total_note = k_octave_count * k_note_per_octave;
    static const unsigned int k_finger_count = 5;

    static std::optional<transitions> from_file(const std::filesystem::path& path);
    unsigned int cost(unsigned int n1, unsigned int f1, unsigned int n2, unsigned int f2) const;
    unsigned int best(unsigned int n1, unsigned int f1, unsigned int n2) const;
    std::pair<unsigned int, unsigned int> best(unsigned int n1, unsigned int n2) const;
  private:
    unsigned int costs_[k_total_note][k_finger_count][k_total_note][k_finger_count];
};

} /* namespace tp */

#endif /* INCLUDE_TRANSITIONS_HPP */
