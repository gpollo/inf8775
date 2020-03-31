#include <algo_greedy.hpp>
#include <algo_dynamic.hpp>
#include <algo_search.hpp>
#include <transitions.hpp>
#include <notes.hpp>

#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <optional>
#include <vector>

static
void print_fingers(std::vector<unsigned int> fingers) {
  for (unsigned f : fingers) {
    std::cout << f << " ";
  }

  std::cout << std::endl;
}

static
void show_help(FILE* f, const char* exec_name) {
  fprintf(f, "Usage: %s [OPTION]...\n", exec_name);
  fprintf(f, "\n");
  fprintf(f, "  --greedy           use greedy algorithm [DEFAULT]\n");
  fprintf(f, "  --dynamic          use dynamic programming algorithm\n");
  fprintf(f, "  --search           use neighbors searching algorithm\n");
  fprintf(f, "  --benchmark        print execution time in milliseconds\n");
  fprintf(f, "  --solution         print computed solution\n");
  fprintf(f, "  --cost             print solution's cost\n");
  fprintf(f, "  --iterations N     the number of iterations for searching algorithm\n");
  fprintf(f, "  --song FILE        the song file to run the algorithm\n");
  fprintf(f, "  --keys FILE        the transitions cost file to run the algorithm\n");
  fprintf(f, "  --help             show this help\n");
}

static
void fail_missing_arg(const char* exec_name, const char* opt) {
  fprintf(stderr, "%s: option '%s' requires an argument\n", exec_name, opt);
  fprintf(stderr, "Try '%s --help' for more information.\n", exec_name);
  exit(1);
}

static
void fail_negative_arg(const char* exec_name, const char* opt) {
  fprintf(stderr, "%s: option '%s' requires positive number\n", exec_name, opt);
  fprintf(stderr, "Try '%s --help' for more information.\n", exec_name);
  exit(1);
}

static
void fail_unknown_option(const char* exec_name, const char* opt) {
  fprintf(stderr, "%s: unrecognized option '%s'\n", exec_name, opt);
  fprintf(stderr, "Try '%s --help' for more information.\n", exec_name);
  exit(1);
}

static
void fail_load_transitions(const char* exec_name, const char* filename) {
  fprintf(stderr, "%s: fail to load transitions file '%s'\n", exec_name, filename);
  fprintf(stderr, "Try '%s --help' for more information.\n", exec_name);
  exit(1);
}

static
void fail_load_song(const char* exec_name, const char* filename) {
  fprintf(stderr, "%s: fail to load song file '%s'\n", exec_name, filename);
  fprintf(stderr, "Try '%s --help' for more information.\n", exec_name);
  exit(1);
}

static
void fail_multiple_algo(const char* exec_name) {
  fprintf(stderr, "%s: multiple algorithm specified\n", exec_name);
  fprintf(stderr, "Try '%s --help' for more information.\n", exec_name);
  exit(1);
}

int main(int argc, char** argv) {
  srand(time(NULL));

  bool use_greedy = false;
  bool use_dynamic = false;
  bool use_search = false;
  bool print_benchmark = false;
  bool print_solution = false;
  bool print_cost = false;
  int use_opt_count = 0;
  int iterations = 10000;
  std::string file_song = "songs/fur_elise.txt";
  std::string file_transitions = "cout_transition.txt";

  char* exec_name = argv[0];
  for (int i = 1; i < argc; i++) {
    if (strcmp("--greedy", argv[i]) == 0) {
      use_greedy = true;
      use_opt_count++;
    } else if (strcmp("--dynamic", argv[i]) == 0) {
      use_dynamic = true;
      use_opt_count++;
    } else if (strcmp("--search", argv[i]) == 0) {
      use_search = true;
      use_opt_count++;
    } else if (strcmp("--benchmark", argv[i]) == 0) {
      print_benchmark = true;
    } else if (strcmp("--solution", argv[i]) == 0) {
      print_solution = true;     
    } else if (strcmp("--cost", argv[i]) == 0) {
      print_cost = true;     
    } else if (strcmp("--iterations", argv[i]) == 0) {
      if (i > argc - 1) {
        fail_missing_arg(exec_name, argv[i]);
      }

      iterations = std::stoi(std::string(argv[++i]));

      if (iterations < 0) {
        fail_negative_arg(exec_name, argv[i-1]);
      }
    } else if (strcmp("--song", argv[i]) == 0) {
      if (i > argc - 1) {
        fail_missing_arg(exec_name, argv[i]);
      }
      
      file_song = std::string(argv[++i]);
    } else if (strcmp("--keys", argv[i]) == 0) {
      if (i > argc - 1) {
        fail_missing_arg(exec_name, argv[i]);
      }
      
      file_transitions = std::string(argv[++i]);
    } else if (strcmp("--help", argv[i]) == 0) {
      show_help(stdout, exec_name);
      exit(0);
    } else {
      fail_unknown_option(exec_name, argv[i]);
    }
  }

  std::optional<tp::notes> notes = tp::notes::from_file(file_song);
  if (!notes) {
    fail_load_song(exec_name, file_song.c_str());
  }

  std::optional<tp::transitions> transitions = tp::transitions::from_file(file_transitions);
  if (!transitions) {
    fail_load_transitions(exec_name, file_transitions.c_str());
  }

  if (use_opt_count == 0) {
    use_greedy = true;
  } else if (use_opt_count > 1) {
    fail_multiple_algo(exec_name);
  }

  tp::algo* algo;

  if (use_greedy) {
    algo = new tp::algo_greedy(*notes, *transitions);
  }

  if (use_dynamic) {
    algo = new tp::algo_dynamic(*notes, *transitions);
  }

  if (use_search) {
    algo = new tp::algo_search(*notes, *transitions, iterations);
  }

  struct timespec start;
  struct timespec end;

  if (clock_gettime(CLOCK_MONOTONIC_RAW, &start) < 0) {
    perror("clock_gettime");
    exit(1);
  }

  auto [cost, fingers] = algo->run();

  if (clock_gettime(CLOCK_MONOTONIC_RAW, &end) < 0) {
    perror("clock_gettime");
    exit(1);
  }

  if (print_benchmark) {
    unsigned long long elapsed;
    elapsed = (end.tv_sec - start.tv_sec) * 1000000000;
    elapsed += (end.tv_nsec - start.tv_nsec);
    printf("%.6f\n", ((float) elapsed) / ((float) 10e6));
  }

  if (print_solution) {
    print_fingers(fingers);
  }

  if (print_cost) {
    std::cout << cost << std::endl;
  }

  delete algo;
  return 0;
}
