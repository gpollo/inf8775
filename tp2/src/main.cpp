#include <algo_greedy.hpp>
#include <algo_dynamic.hpp>
#include <algo_search.hpp>
#include <transitions.hpp>
#include <notes.hpp>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <optional>

void print_fingers(std::vector<unsigned int> fingers) {
  for (unsigned f : fingers) {
    std::cout << f << " ";
  }

  std::cout << std::endl;
}

int main() {
    srand(time(NULL));

    std::optional<tp::transitions> transitions = tp::transitions::from_file("cout_transition.txt");
    if (!transitions) {
        return 1;
    }

    std::optional<tp::notes> notes = tp::notes::from_file("chansons/fur_elise.txt");
//    std::optional<tp::notes> notes = tp::notes::from_file("chansons/twinkle_twinkle.txt");
    if (!notes) {
        return 1;
    }

    tp::algo_greedy algo_greedy(*notes, *transitions);
    auto [cost_greedy, fingers_greedy] = algo_greedy.run();

    std::cout << "greedy algorithm:" << std::endl;
    std::cout << "cost: " << cost_greedy << std::endl;
    std::cout << "fingers: ";
    print_fingers(fingers_greedy);
    std::cout << std::endl;

    tp::algo_dynamic algo_dynamic(*notes, *transitions);
    auto [cost_dynamic, fingers_dynamic] = algo_dynamic.run();

    std::cout << "dynamic programming algorithm:" << std::endl;
    std::cout << "cost: " << cost_dynamic << std::endl;
    std::cout << "fingers: ";
    print_fingers(fingers_dynamic);
    std::cout << std::endl;

    tp::algo_search algo_search(*notes, *transitions, 50000);
    auto [cost_search, fingers_search] = algo_search.run();

    std::cout << "greedy + search algorithm:" << std::endl;
    std::cout << "cost: " << cost_search << std::endl;
    std::cout << "fingers: ";
    print_fingers(fingers_search);
    std::cout << std::endl;

    return 0;
}
