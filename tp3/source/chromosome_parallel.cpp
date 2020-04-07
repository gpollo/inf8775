#include <chromosome.hpp>
#include <chromosome_parallel.hpp>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_unordered_set.h>
#include <tbb/parallel_for.h>

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include <iostream>

namespace tp::parallel {

void chromosome_costs::operator()(std::vector<chromosome*>& chromosomes, unsigned int max) {
  tbb::concurrent_unordered_multimap<unsigned int, chromosome*> concurrent_costs;
  tbb::concurrent_unordered_set<chromosome*> concurrent_invalids;

  tbb::parallel_for(
    tbb::blocked_range<std::vector<chromosome*>::iterator>(
      chromosomes.begin(),
      chromosomes.end()
    ),
    [&] (auto range) {
      std::for_each(
        range.begin(),
        range.end(),
        [&] (auto chromosome) {
          auto cost = chromosome->cost();
          if (cost) {
            concurrent_costs.emplace(cost.value(), chromosome);
          } else {
            concurrent_costs.emplace(max, chromosome);
            concurrent_invalids.insert(chromosome);
          }
        }
      );
    }
  );

  std::for_each(
    concurrent_costs.begin(),
    concurrent_costs.end(),
    [&] (auto result) {
      costs_.emplace(result);
    }
  );

  std::for_each(
    concurrent_invalids.begin(),
    concurrent_invalids.end(),
    [&] (auto result) {
      invalids_.emplace(result);
    }
  );
}

const std::multimap<unsigned int, chromosome*>& chromosome_costs::costs() const {
  return costs_;
}

const std::set<chromosome*> chromosome_costs::invalids() const {
  return invalids_;
}


} /* namespace tp:tbb */
