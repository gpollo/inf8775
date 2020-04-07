#include <chromosome.hpp>
#include <chromosome_parallel.hpp>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/parallel_for.h>

#include <algorithm>
#include <map>
#include <vector>

namespace tp::parallel {

void chromosome_costs(std::vector<chromosome*>& c, std::multimap<unsigned int, chromosome*>& r) {
  tbb::concurrent_unordered_multimap<unsigned int, chromosome*> costs;

  tbb::parallel_for(
    tbb::blocked_range<std::vector<chromosome*>::iterator>(
      c.begin(),
      c.end()
    ),
    [&] (auto range) {
      std::for_each(
        range.begin(),
        range.end(),
        [&] (auto chromosome) {
          costs.emplace(chromosome->cost(), chromosome);
        }
      );
    }
  );

  std::for_each(
    costs.begin(),
    costs.end(),
    [&] (auto result) {
      r.emplace(result);
    }
  );
}

} /* namespace tp:tbb */
