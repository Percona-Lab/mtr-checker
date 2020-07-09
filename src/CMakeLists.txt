cmake_minimum_required (VERSION 3.8)

add_executable (mtr_checker "mtr_checker.cpp" "app_exception.hpp" "test_component.hpp" "test_component_set.hpp" "test_component_set.cpp" "logger.hpp" "processor.hpp" "processor.cpp")

set_target_properties(mtr_checker PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)
