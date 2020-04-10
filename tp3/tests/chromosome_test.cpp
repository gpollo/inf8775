#include <catch.hpp>
#include <chromosome.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <set>

namespace tp {

class mock_settings : public settings {
  public:
    virtual unsigned int virality() const {
      return 2;
    }

    virtual float initial_isolation_factor() const {
      return 0.5;
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

TEST_CASE("Isolation are correctly inserted in chromosome") {
  tp::population population = create_population();
  tp::mock_settings settings;
  tp::chromosome chromosome(settings, population);

  auto relations = population.relations();

  REQUIRE(chromosome.isolations().size() == 1);
  for (const auto& isolation : chromosome.isolations()) {
    REQUIRE(relations.find(isolation) != relations.end());
  }
}

TEST_CASE("Cost of chromosome is correctly computed") {
  tp::population population = create_population();
  tp::mock_settings settings;

  std::set<std::pair<unsigned int, unsigned int>> i1{};
  tp::chromosome c1(settings, population, i1);
  REQUIRE(c1.cost() == std::nullopt);

  std::set<std::pair<unsigned int, unsigned int>> i2{{3, 5}};
  tp::chromosome c2(settings, population, i2);
  REQUIRE(c2.cost() == std::nullopt);

  std::set<std::pair<unsigned int, unsigned int>> i3{{3, 4}, {3, 5}};
  tp::chromosome c3(settings, population, i3);
  REQUIRE(c3.cost() == std::nullopt);

  std::set<std::pair<unsigned int, unsigned int>> i4{{2, 3}, {3, 4}, {3, 5}};
  tp::chromosome c4(settings, population, i4);
  REQUIRE(c4.cost() == 3);

  std::set<std::pair<unsigned int, unsigned int>> i5{{0,2}, {2, 3}, {3, 4}, {3, 5}};
  tp::chromosome c5(settings, population, i5);
  REQUIRE(c5.cost() == 4);

  std::set<std::pair<unsigned int, unsigned int>> i6{{0, 1}, {0,2}, {2, 3}, {3, 4}, {3, 5}};
  tp::chromosome c6(settings, population, i6);
  REQUIRE(c6.cost() == 5);
}
