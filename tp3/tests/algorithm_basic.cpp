#include <catch.hpp>
#include <algorithm_basic.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <set>

static
tp::population create_population() {
  tp::population pop(6);

  pop.add_infected(0);
  pop.add_infected(1);

  pop.add_relation({0, 2});
  pop.add_relation({0, 3});
  pop.add_relation({1, 2});
  pop.add_relation({1, 5});
  pop.add_relation({2, 3});
  pop.add_relation({2, 4});
  pop.add_relation({3, 4});
  pop.add_relation({3, 5});

  return pop;
}

TEST_CASE("basic algorithm for isolating infected should return correct isolations") {
  tp::population population = create_population();
  tp::settings settings(1);
  tp::algorithm_basic algorithm_basic(settings, population);

  auto solutions = algorithm_basic.isolate_infected();
  REQUIRE(solutions.contains({0, 2}));
  REQUIRE(solutions.contains({0, 3}));
  REQUIRE(solutions.contains({1, 2}));
  REQUIRE(solutions.contains({1, 5}));
}
