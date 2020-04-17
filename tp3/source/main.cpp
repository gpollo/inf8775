#include <algorithm.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <cstring>
#include <cerrno>
#include <csignal>
#include <iostream>
#include <unistd.h>

void handle_sigsegv(int signal) {
  std::cerr << "\rSIGSEGV: suspending the application execution" << std::endl;
  pause();
  std::cerr << "\rSIGSEGV: application execution resumed" << std::endl;
}

tp::algorithm* algorithm;
void handle_sigint(int signal) {
  if (signal != SIGINT) {
    return;
  }

  std::cerr << "\rSIGINT: stopping the algorithm" << std::endl;
  if (algorithm != nullptr) {
    algorithm->stop();
  }
}

int setup_sigsegv_handler(void) {
  struct sigaction sigsegv_handler;
  sigsegv_handler.sa_flags = 0;
  sigsegv_handler.sa_handler = &handle_sigint;
  
  if (sigemptyset(&sigsegv_handler.sa_mask) < 0) {
    std::cerr << "Failed to set SIGSEGV handler: ";
    std::cerr << strerror(errno) << std::endl;
    return -1;
  }

  if (sigaction(SIGSEGV, &sigsegv_handler, NULL) < 0) {
    std::cerr << "Failed to set SIGSEGV handler: ";
    std::cerr << strerror(errno) << std::endl;
    return -1;
  }

  return 0;
}

int setup_sigint_handler(void) {
  struct sigaction sigint_handler;
  sigint_handler.sa_flags = 0;
  sigint_handler.sa_handler = &handle_sigint;
  
  if (sigemptyset(&sigint_handler.sa_mask) < 0) {
    std::cerr << "Failed to set SIGINT handler: ";
    std::cerr << strerror(errno) << std::endl;
    return -1;
  }

  if (sigaction(SIGINT, &sigint_handler, NULL) < 0) {
    std::cerr << "Failed to set SIGINT handler: ";
    std::cerr << strerror(errno) << std::endl;
    return -1;
  }

  return 0;
}

int run(tp::algorithm* algo) {
  if (setup_sigsegv_handler() < 0) {
    return 1;
  }

  if (setup_sigint_handler() < 0) {
    return 1;
  }

  if (algo != nullptr) {
    algorithm = algo;
    algorithm->run();
  }

  return 0;
}

static
void show_help(FILE* f, const char* exec_name) {
  fprintf(f, "Usage: %s [OPTION]...\n", exec_name);
  fprintf(f, "\n");
  fprintf(f, "  --dataset PATH     path to the dataset to run\n");
  fprintf(f, "  --virality N       the propagation rate of the virus\n");
  fprintf(f, "  --solutions        print new solutions each time they're found\n");
  fprintf(f, "  --timestamp        print timestamp each time a new solution is found\n"); 
  fprintf(f, "  --help             show this help\n");
}

static
void fail_missing_arg(const char* exec_name, const char* opt) {
  fprintf(stderr, "%s: option '%s' requires an argument\n", exec_name, opt);
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
void fail_negative_arg(const char* exec_name, const char* opt) {
  fprintf(stderr, "%s: option '%s' requires positive number\n", exec_name, opt);
  fprintf(stderr, "Try '%s --help' for more information.\n", exec_name);
  exit(1);
}

static
void fail_incompatible_opts(const char* exec_name, const char* opt1, const char* opt2) {
  fprintf(stderr, "%s: option '%s' is incompatible with '%s'\n", exec_name, opt1, opt2);
  fprintf(stderr, "Try '%s --help' for more information.\n", exec_name);
  exit(1);
}

static
void fail_load_dataset(const char* exec_name, const char* filename, const char* reason) {
  fprintf(stderr, "%s: fail to load dataset file '%s': %s\n", exec_name, filename, reason);
  fprintf(stderr, "Try '%s --help' for more information.\n", exec_name);
  exit(1);
}

int main(int argc, char* argv[]) {
  std::string dataset = "../exemplaires/1000_3000_30_0.txt";
  unsigned int virality = 3;
  bool print_solutions = false;
  bool print_timestamp = false;

  char* exec_name = argv[0];
  for (int i = 1; i < argc; i++) {
    if (strcmp("--dataset", argv[i]) == 0) {
      if (i > argc - 1) {
        fail_missing_arg(exec_name, argv[i]);
      }

      dataset = argv[++i];
    } else if (strcmp("--virality", argv[i]) == 0) {
      if (i > argc - 1) {
        fail_missing_arg(exec_name, argv[i]);
      }

      virality = std::stoi(std::string(argv[++i]));

      if (virality < 0) {
        fail_negative_arg(exec_name, argv[i-1]);
      }
    } else if (strcmp("--solutions", argv[i]) == 0) {
      print_solutions = true;
    } else if (strcmp("--timestamp", argv[i]) == 0) {
      print_timestamp = true;
    } else if (strcmp("--help", argv[i]) == 0) {
      show_help(stdout, exec_name);
      exit(0);
    } else {
      fail_unknown_option(exec_name, argv[i]);
    }
  }

  if (print_solutions && print_timestamp) {
    fail_incompatible_opts(exec_name, "--solutions", "--timestamp");
  }

  auto population_file = tp::population::from_file(dataset);
  if (population_file.index()) {
    fail_load_dataset(exec_name, dataset.c_str(), std::get<std::string>(population_file).c_str());
  }

  tp::settings settings(virality);
  tp::population population = std::get<tp::population>(population_file);
  tp::algorithm algorithm(print_solutions, print_timestamp, settings, population);
  return run(&algorithm);
}
