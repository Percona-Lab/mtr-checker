#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <ostream>
#include <cassert>
#include <utility>

#define MTR_CHECKER_MESSAGE_SEVERITY_ITEMS() \
  MTR_CHECKER_MESSAGE_SEVERITY_X(info)       \
  MTR_CHECKER_MESSAGE_SEVERITY_X(warning)    \
  MTR_CHECKER_MESSAGE_SEVERITY_X(error)

#define MTR_CHECKER_MESSAGE_SEVERITY_X(ITEM) ITEM ,
enum class message_severity_t
{
  MTR_CHECKER_MESSAGE_SEVERITY_ITEMS()
  delimiter
};
#undef MTR_CHECKER_MESSAGE_SEVERITY_X

#define MTR_CHECKER_MESSAGE_SEVERITY_X(ITEM) #ITEM ,
static constexpr std::string_view message_severity_labels[static_cast<std::size_t>(message_severity_t::delimiter) + 1] =
{
  MTR_CHECKER_MESSAGE_SEVERITY_ITEMS()
  ""
};
#undef MTR_CHECKER_MESSAGE_SEVERITY_X

constexpr std::string_view get_message_severity_label(message_severity_t message_severity) noexcept
{
  return message_severity_labels[std::min(static_cast<std::size_t>(message_severity), static_cast<std::size_t>(message_severity_t::delimiter))];
}

inline std::ostream& operator << (std::ostream& os, message_severity_t message_severity)
{
  return os << get_message_severity_label(message_severity);
}

constexpr message_severity_t get_message_severity_by_label(std::string_view message_severity) noexcept
{
  std::size_t index = 0;
  while (index < static_cast<std::size_t>(message_severity_t::delimiter) && message_severity_labels[static_cast<std::size_t>(index)] != message_severity)
    ++index;
  return static_cast<message_severity_t>(index);
}

class logger
{
public:
  logger(std::ostream& os, message_severity_t min_severity) :
    os_{ &os }, min_severity_{ min_severity }
  {
    assert(min_severity < message_severity_t::delimiter);
  }

  logger(const logger&) = delete;
  logger& operator = (const logger&) = delete;
  logger(logger&&) = delete;
  logger& operator = (logger&&) = delete;

  message_severity_t get_min_severity() const noexcept { return min_severity_; }
  void set_min_severity(message_severity_t min_severity) noexcept { min_severity_ = min_severity; }

  template<typename... ArgumentTypePack>
  void log(message_severity_t severity, ArgumentTypePack&&... arguments)
  {
    if (severity < min_severity_)
      return;
    assert(severity < message_severity_t::delimiter);

    *os_ << '[' << severity << "]: ";
    (*os_ << ... << std::forward<ArgumentTypePack>(arguments));
    *os_ << '\n';
  }
private:
  std::ostream* os_;
  message_severity_t min_severity_;
};

#endif // LOGGER_HPP
