#ifndef INCLUDE_ALGO_DYNAMIC_HPP
#define INCLUDE_ALGO_DYNAMIC_HPP

#include <algo.hpp>
#include <notes.hpp>
#include <transitions.hpp>

#include <map>
#include <utility>
#include <vector>

namespace tp {

class algo_dynamic : public algo  {
  public:
    algo_dynamic(notes n, transitions t);
    std::pair<unsigned int, std::vector<unsigned int>> run() override;
  private:
    unsigned int find_min(unsigned int note_index, unsigned int current_finger);
    std::vector<unsigned int> get_finger_list(unsigned int current_finger) const;
    std::map<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int>> results_;
};

} /* namespace tp */

#endif /* INCLUDE_ALGO_DYNAMIC_HPP */
