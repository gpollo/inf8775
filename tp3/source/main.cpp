#include <algorithm.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <cstring>
#include <cerrno>
#include <csignal>
#include <iostream>

#include <unistd.h>

tp::algorithm* algorithm;
void handle_sigint(int signal) {
  std::cerr << "\rSIGINT received" << std::endl;
  if (algorithm != nullptr) {
    algorithm->stop();
  }
}

void run(tp::algorithm* algo) {
  if (algo != nullptr) {
    algorithm = algo;
    algorithm->run();
  }
}

int main(int argc, char* argv[]) {
  std::string filename = "../exemplaires/1000_3000_30_0.txt";

  auto population_file = tp::population::from_file(filename);
  if (population_file.index()) {
    std::cerr << "Failed to load population from file '" << filename << "': ";
    std::cerr << std::get<std::string>(population_file) << std::endl;
    return 1;
  }

  tp::settings settings;
  tp::population population = std::get<tp::population>(population_file);
  tp::algorithm algorithm(settings, population);

  struct sigaction sigint_handler;
  sigint_handler.sa_flags = 0;
  sigint_handler.sa_handler = &handle_sigint;
  
  if (sigemptyset(&sigint_handler.sa_mask) < 0) {
    std::cerr << "Failed to set SIGINT handler: ";
    std::cerr << strerror(errno) << std::endl;
    return 1;
  }

  if (sigaction(SIGINT, &sigint_handler, NULL) < 0) {
    std::cerr << "Failed to set SIGINT handler: ";
    std::cerr << strerror(errno) << std::endl;
    return 1;
  }

  run(&algorithm);

  return 0;
}
