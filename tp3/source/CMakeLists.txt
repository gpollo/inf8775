set(SOURCE_FILES
    algorithm.cpp
    algorithm_basic.cpp
    chromosome.cpp
    chromosome_parallel.cpp
    population.cpp
    settings.cpp
)

target_sources(pandemic PUBLIC ${SOURCE_FILES} main.cpp)
target_sources(pandemic_test PUBLIC ${SOURCE_FILES} main_test.cpp)
