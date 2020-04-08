#include <chromosome.hpp>
#include <chromosome_parallel.hpp>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_unordered_set.h>
#include <tbb/concurrent_vector.h>
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

void chromosome_cross::operator()(std::vector<cross_settings>& settings) {
  tbb::concurrent_vector<chromosome*> concurrent_created;

  tbb::parallel_for(
    tbb::blocked_range<std::vector<cross_settings>::iterator>(
      settings.begin(),
      settings.end()
    ),
    [&] (auto range) {
      std::for_each(
        range.begin(),
        range.end(),
        [&] (auto settings) {
          chromosome* c1 = std::get<0>(settings);
          chromosome* c2 = std::get<1>(settings);
          auto [n1, n2] = c1->cross(c2);
          concurrent_created.push_back(n1);
          concurrent_created.push_back(n2);
        }
      );
    }
  );

  std::for_each(
    concurrent_created.begin(),
    concurrent_created.end(),
    [&] (auto result) {
      created_.push_back(result);
    }
  );
}

const std::vector<chromosome*>& chromosome_cross::created() const {
  return created_;
}

void chromosome_mutate::operator()(std::vector<mutation_settings>& settings) {
  tbb::concurrent_vector<chromosome*> concurrent_created;

  tbb::parallel_for(
    tbb::blocked_range<std::vector<mutation_settings>::iterator>(
      settings.begin(),
      settings.end()
    ),
    [&] (auto range) {
      std::for_each(
        range.begin(),
        range.end(),
        [&] (auto settings) {
          chromosome* chromosome = std::get<0>(settings);
          unsigned int add = std::get<1>(settings);
          unsigned int remove = std::get<2>(settings);
          unsigned int update = std::get<3>(settings);
          concurrent_created.push_back(chromosome->mutate(add, remove, update));
        }
      );
    }
  );

  std::for_each(
    concurrent_created.begin(),
    concurrent_created.end(),
    [&] (auto result) {
      created_.push_back(result);
    }
  );
}

const std::vector<chromosome*>& chromosome_mutate::created() const {
  return created_;
}

} /* namespace tp::parallel */
