#ifndef INCLUDE_CHROMOSOME_PARALLEL_HPP
#define INCLUDE_CHROMOSOME_PARALLEL_HPP

#include <chromosome.hpp>

#include <map>
#include <set>
#include <vector>

namespace tp::parallel {

class chromosome_costs {
  public:
    void operator()(std::vector<chromosome*>& chromosomes, unsigned int max);
    const std::multimap<unsigned int, chromosome*>& costs() const;
    const std::set<chromosome*> invalids() const;
  private:
    std::multimap<unsigned int, chromosome*> costs_;
    std::set<chromosome*> invalids_;
};

} /* namespace tp */

#endif /* INCLUDE_CHROMOSOME_PARALLEL_HPP */
