#include <catch.hpp>
#include <population.hpp>

static
tp::population create_population() {
    tp::population pop(6);

    pop.add_infected(0);
    pop.add_infected(2);
    pop.add_infected(3);
    pop.add_infected(6);

    pop.add_relation(0, 1);
    pop.add_relation(0, 2);
    pop.add_relation(0, 3);
    pop.add_relation(0, 4);
    pop.add_relation(0, 5);
    pop.add_relation(1, 2);
    pop.add_relation(1, 3);
    pop.add_relation(3, 4);

    pop.remove_relation(1, 3);
    pop.remove_relation(4, 3);

    return pop;
}

TEST_CASE("Relations are correctly inserted") {
    tp::population pop = create_population();
    REQUIRE(pop.relations().size() == 6);

    REQUIRE(pop.relations(0) != nullptr);
    REQUIRE(pop.relations(0)->size() == 5);
    REQUIRE(pop.relations(0)->find(1) != pop.relations(0)->end());
    REQUIRE(pop.relations(0)->find(2) != pop.relations(0)->end());
    REQUIRE(pop.relations(0)->find(3) != pop.relations(0)->end());
    REQUIRE(pop.relations(0)->find(4) != pop.relations(0)->end());
    REQUIRE(pop.relations(0)->find(5) != pop.relations(0)->end());

    REQUIRE(pop.relations(1) != nullptr);
    REQUIRE(pop.relations(1)->size() == 2);
    REQUIRE(pop.relations(1)->find(0) != pop.relations(1)->end());
    REQUIRE(pop.relations(1)->find(2) != pop.relations(1)->end());

    REQUIRE(pop.relations(2) != nullptr);
    REQUIRE(pop.relations(2)->size() == 2);
    REQUIRE(pop.relations(2)->find(0) != pop.relations(2)->end());
    REQUIRE(pop.relations(2)->find(1) != pop.relations(2)->end());

    REQUIRE(pop.relations(3) != nullptr);
    REQUIRE(pop.relations(3)->size() == 1);
    REQUIRE(pop.relations(3)->find(0) != pop.relations(3)->end());

    REQUIRE(pop.relations(4) != nullptr);
    REQUIRE(pop.relations(4)->size() == 1);
    REQUIRE(pop.relations(4)->find(0) != pop.relations(4)->end());

    REQUIRE(pop.relations(5) != nullptr);
    REQUIRE(pop.relations(5)->size() == 1);
    REQUIRE(pop.relations(5)->find(0) != pop.relations(5)->end());

    REQUIRE(pop.relations(6) == nullptr);
}

TEST_CASE("Infected are correctly inserted") {
    tp::population pop = create_population();
    REQUIRE(pop.infected().size() == 4);

    REQUIRE(pop.infected(0) != nullptr);
    REQUIRE(pop.infected(0)->size() == 2);
    REQUIRE(pop.infected(0)->find(2) != pop.relations(0)->end());
    REQUIRE(pop.infected(0)->find(3) != pop.relations(0)->end());

    REQUIRE(pop.infected(1) != nullptr);
    REQUIRE(pop.infected(1)->size() == 2);
    REQUIRE(pop.infected(1)->find(0) != pop.relations(1)->end());
    REQUIRE(pop.infected(1)->find(2) != pop.relations(1)->end());

    REQUIRE(pop.infected(2) != nullptr);
    REQUIRE(pop.infected(2)->size() == 1);
    REQUIRE(pop.infected(2)->find(0) != pop.relations(2)->end());

    REQUIRE(pop.infected(3) != nullptr);
    REQUIRE(pop.infected(3)->size() == 1);
    REQUIRE(pop.infected(3)->find(0) != pop.relations(3)->end());

    REQUIRE(pop.infected(4) != nullptr);
    REQUIRE(pop.infected(4)->size() == 1);
    REQUIRE(pop.infected(4)->find(0) != pop.relations(4)->end());

    REQUIRE(pop.infected(5) != nullptr);
    REQUIRE(pop.infected(5)->size() == 1);
    REQUIRE(pop.infected(5)->find(0) != pop.relations(5)->end());

    REQUIRE(pop.infected(6) == nullptr);
}

TEST_CASE("Infection is correctly propagated") {
    tp::population pop(6);

    pop.add_infected(0);
    pop.add_infected(1);

    pop.add_relation(0, 2);
    pop.add_relation(0, 3);
    pop.add_relation(1, 2);
    pop.add_relation(1, 5);
    pop.add_relation(2, 3);
    pop.add_relation(2, 4);
    pop.add_relation(3, 4);
    pop.add_relation(3, 5);

    REQUIRE(pop.infected().size() == 2);
    REQUIRE(pop.infected().find(0) != pop.infected().end());
    REQUIRE(pop.infected().find(1) != pop.infected().end());

    REQUIRE(pop.run_iteration(2) == 1);
    REQUIRE(pop.infected().size() == 3);
    REQUIRE(pop.infected().find(0) != pop.infected().end());
    REQUIRE(pop.infected().find(1) != pop.infected().end());
    REQUIRE(pop.infected().find(2) != pop.infected().end());

    REQUIRE(pop.run_iteration(2) == 1);
    REQUIRE(pop.infected().size() == 4);
    REQUIRE(pop.infected().find(0) != pop.infected().end());
    REQUIRE(pop.infected().find(1) != pop.infected().end());
    REQUIRE(pop.infected().find(2) != pop.infected().end());
    REQUIRE(pop.infected().find(3) != pop.infected().end());

    REQUIRE(pop.run_iteration(2) == 2);
    REQUIRE(pop.infected().size() == 6);
    REQUIRE(pop.infected().find(0) != pop.infected().end());
    REQUIRE(pop.infected().find(1) != pop.infected().end());
    REQUIRE(pop.infected().find(2) != pop.infected().end());
    REQUIRE(pop.infected().find(3) != pop.infected().end());
    REQUIRE(pop.infected().find(4) != pop.infected().end());
    REQUIRE(pop.infected().find(5) != pop.infected().end());

    REQUIRE(pop.run_iteration(2) == 0);
    REQUIRE(pop.infected().size() == 6);
    REQUIRE(pop.infected().find(0) != pop.infected().end());
    REQUIRE(pop.infected().find(1) != pop.infected().end());
    REQUIRE(pop.infected().find(2) != pop.infected().end());
    REQUIRE(pop.infected().find(3) != pop.infected().end());
    REQUIRE(pop.infected().find(4) != pop.infected().end());
    REQUIRE(pop.infected().find(5) != pop.infected().end());
}
