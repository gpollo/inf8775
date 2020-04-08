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

using cross_settings = std::pair<chromosome*, chromosome*>;

class chromosome_cross {
  public:
    void operator()(std::vector<cross_settings>& settings);
    const std::vector<chromosome*>& created() const;
  private:
    std::vector<chromosome*> created_;   
};

using mutation_settings = std::tuple<chromosome*, unsigned int, unsigned int, unsigned int>;

class chromosome_mutate {
  public:
    void operator()(std::vector<mutation_settings>& settings);
    const std::vector<chromosome*>& created() const;
  private:
    std::vector<chromosome*> created_;
};

} /* namespace tp::parallel */

#endif /* INCLUDE_CHROMOSOME_PARALLEL_HPP */
