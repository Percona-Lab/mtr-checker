#ifndef TEST_COMPONENT_SET_HPP
#define TEST_COMPONENT_SET_HPP

#include <cstdint>
#include <iosfwd>

#include "test_component.hpp"

class test_component_set
{
public:
  test_component_set() noexcept :
    component_bitset_{ 0U }
  {}

  bool is_set(test_component_t test_component) const noexcept
  {
    return (component_bitset_ & (static_cast<underlying_type>(1U) << static_cast<std::size_t>(test_component))) != 0U;
  }

  void set(test_component_t test_component) noexcept
  {
    component_bitset_ |= static_cast<underlying_type>(1U) << static_cast<std::size_t>(test_component);
  }

  bool check() const noexcept
  {
    if (!is_set(test_component_t::test) || !is_set(test_component_t::result))
      return false;
    if (is_set(test_component_t::opt) && (is_set(test_component_t::opt_master) || is_set(test_component_t::opt_slave)))
      return false;
    return true;
  }

private:
  using underlying_type = std::uint16_t;
  static_assert(sizeof(underlying_type) * 8 >= static_cast<std::size_t>(test_component_t::delimiter), "underlying_type must have enough bits");
  underlying_type component_bitset_;
};

std::ostream& operator << (std::ostream& os, const test_component_set& test_components);

#endif // TEST_COMPONENT_SET_HPP
