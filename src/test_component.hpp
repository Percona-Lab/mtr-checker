#ifndef TEST_COMPONENT_HPP
#define TEST_COMPONENT_HPP

#include <cstddef>
#include <algorithm>
#include <ostream>
#include <string_view>

#define MTR_CHECKER_TEST_COMPONENT_ITEMS() \
  MTR_CHECKER_TEST_COMPONENT_X(test)       \
  MTR_CHECKER_TEST_COMPONENT_X(result)     \
  MTR_CHECKER_TEST_COMPONENT_X(cnf)        \
  MTR_CHECKER_TEST_COMPONENT_X(opt)        \
  MTR_CHECKER_TEST_COMPONENT_X(opt_master) \
  MTR_CHECKER_TEST_COMPONENT_X(opt_slave ) \
  MTR_CHECKER_TEST_COMPONENT_X(opt_client)

#define MTR_CHECKER_TEST_COMPONENT_X(ITEM) ITEM ,
enum class test_component_t
{
  MTR_CHECKER_TEST_COMPONENT_ITEMS()
  delimiter
};
#undef MTR_CHECKER_TEST_COMPONENT_X

#define MTR_CHECKER_TEST_COMPONENT_X(ITEM) #ITEM ,
static constexpr std::string_view test_component_labels[static_cast<std::size_t>(test_component_t::delimiter) + 1] =
{
  MTR_CHECKER_TEST_COMPONENT_ITEMS()
  ""
};
#undef MTR_CHECKER_TEST_COMPONENT_X

constexpr std::string_view get_test_component_label(test_component_t test_component) noexcept
{
  return test_component_labels[std::min(static_cast<std::size_t>(test_component), static_cast<std::size_t>(test_component_t::delimiter))];
}

inline std::ostream& operator << (std::ostream& os, test_component_t test_component)
{
  return os << get_test_component_label(test_component);
}

#endif // TEST_COMPONENT_HPP
