set(TEST_SOURCE_FILES
    algorithm_basic.cpp
    population_test.cpp
    chromosome_test.cpp
)

target_sources(pandemic_test PUBLIC ${TEST_SOURCE_FILES})
