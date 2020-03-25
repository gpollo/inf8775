#ifndef INCLUDE_ALGO_HPP
#define INCLUDE_ALGO_HPP

#include <notes.hpp>
#include <transitions.hpp>

#include <utility>
#include <vector>

namespace tp {

class algo {
  public:
    algo(notes n, transitions t);
    virtual std::pair<unsigned int, std::vector<unsigned int>> run() = 0;
  protected:
    notes notes_;
    transitions transitions_;
};

} /* namespace tp */

#endif /* INCLUDE_ALGO_HPP */
