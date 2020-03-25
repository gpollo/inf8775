#ifndef INCLUDE_ALGO_SEARCH_HPP
#define INCLUDE_ALGO_SEARCH_HPP

#include <algo.hpp>
#include <algo_greedy.hpp>
#include <notes.hpp>
#include <transitions.hpp>

#include <utility>
#include <vector>

namespace tp {

class algo_search : public algo  {
  public:
    algo_search(notes n, transitions t, unsigned int iterations);
    std::pair<unsigned int, std::vector<unsigned int>> run() override;
  private:
    std::pair<unsigned int, std::vector<unsigned int>> run(std::vector<unsigned int>& fingers, unsigned int until);
    algo_greedy algo_greedy_;
    unsigned int iterations_;
};

} /* namespace tp */

#endif /* INCLUDE_ALGO_SEARCH_HPP */
