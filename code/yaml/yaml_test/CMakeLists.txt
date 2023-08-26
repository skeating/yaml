project(yaml_test LANGUAGES CXX)

add_executable(yaml_test main.cpp)

target_link_libraries(yaml_test PRIVATE "C:\\Development\\SBML\\dependencies\\YAML_CPP\\lib\\yaml-cppd.lib")

add_test(NAME yaml_test COMMAND yaml_test)
#catch_discovered_tests(yaml_test)