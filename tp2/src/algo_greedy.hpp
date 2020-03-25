#ifndef INCLUDE_ALGO_GREEDY_HPP
#define INCLUDE_ALGO_GREEDY_HPP

#include <algo.hpp>
#include <notes.hpp>
#include <transitions.hpp>

#include <utility>
#include <vector>

namespace tp {

class algo_greedy : public algo  {
  public:
    algo_greedy(notes n, transitions t);
    std::pair<unsigned int, std::vector<unsigned int>> run() override;
};

} /* namespace tp */

#endif /* INCLUDE_ALGO_GREEDY_HPP */
