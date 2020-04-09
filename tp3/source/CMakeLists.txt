set(SOURCE_FILES
    algorithm.cpp
    chromosome.cpp
    chromosome_parallel.cpp
    greedy.cpp
    population.cpp
    settings.cpp
)

target_sources(pandemic PUBLIC ${SOURCE_FILES} main.cpp)
target_sources(pandemic_test PUBLIC ${SOURCE_FILES} main_test.cpp)
