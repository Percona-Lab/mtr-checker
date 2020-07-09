#include "test_component_set.hpp"

std::ostream& operator << (std::ostream& os, const test_component_set& test_components)
{
  bool first = true;
  os << "[";
  for (std::size_t u = 0U; u < static_cast<std::size_t>(test_component_t::delimiter); ++u)
  {
    auto casted_u = static_cast<test_component_t>(u);
    if (test_components.is_set(casted_u))
    {
      if (first)
        first = false;
      else
        os << ',';
      os << ' ' << casted_u;
    }
  }
  os << " ]";
  return os;
}
