add_executable(unit_tests 
    test_Vec3.cpp
)  

target_link_libraries(unit_tests
    PRIVATE
        miktracer_lib
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(unit_tests)