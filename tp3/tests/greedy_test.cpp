#include <catch.hpp>
#include <greedy.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <set>

#include <iostream>

namespace tp {

class greedy_mock_settings : public settings {
  public:
    virtual unsigned int virality() const {
      return 1;
    }
};

} // namespace tp

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

TEST_CASE("Greedy algorithm return correct isolations") {
  tp::population population = create_population();
  tp::greedy_mock_settings settings;
  tp::greedy greedy(settings, population);

  auto solutions = greedy.run();
  REQUIRE(solutions.contains({0, 2}));
  REQUIRE(solutions.contains({0, 3}));
  REQUIRE(solutions.contains({1, 2}));
  REQUIRE(solutions.contains({1, 5}));
}
