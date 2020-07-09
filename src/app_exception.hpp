#ifndef APP_EXCEPTION_HPP
#define APP_EXCEPTION_HPP

#include <string>
#include <stdexcept>

enum class app_exit_code_t : int {
  success = 0,
  invalid_number_of_arguments,
  invalid_log_level,
  path_does_not_exist,
  path_is_not_a_directory,
  std_exception_caught,
  unknown_exception_caught
};

class app_exception : public std::runtime_error
{
public:
  app_exception(app_exit_code_t app_exit_code, const std::string& message) :
    std::runtime_error{ message },
    app_exit_code_{ app_exit_code }
  {}
  app_exception(app_exit_code_t app_exit_code, const char* message) :
    std::runtime_error{ message },
    app_exit_code_{ app_exit_code }
  {}

  app_exit_code_t get_app_exit_code() const noexcept { return app_exit_code_; }

private:
  app_exit_code_t app_exit_code_;
};

#endif // APP_EXCEPTION_HPP
