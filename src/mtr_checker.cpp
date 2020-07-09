#include <cassert>
#include <filesystem>
#include <map>
#include <iostream>

#include "app_exception.hpp"
#include "logger.hpp"
#include "processor.hpp"

int main(int argc, char* argv[])
{
  auto result = app_exit_code_t::success;

  try
  {
    if (argc != 3 && argc != 4)
    {
      auto app_binary_name = std::filesystem::path{ argv[0] }.filename().string();
      throw app_exception{ app_exit_code_t::invalid_number_of_arguments, "use " + app_binary_name + " <log_level:info|warning|error> <test_dir> [<result_dir>]" };
    }

    auto message_severity = get_message_severity_by_label(argv[1]);
    if (message_severity == message_severity_t::delimiter)
      throw app_exception{ app_exit_code_t::invalid_log_level, "invalid log level \"" + std::string(argv[1]) + "\"" };

    auto tests_path = std::filesystem::path{ argv[2] };
    if (!std::filesystem::exists(tests_path))
      throw app_exception{ app_exit_code_t::path_does_not_exist, "\"" + tests_path.string() + "\" does not exist" };
    if (!std::filesystem::is_directory(tests_path))
      throw app_exception{ app_exit_code_t::path_is_not_a_directory, "\"" + tests_path.string() + "\" is not a directory" };
    std::filesystem::path results_path{};
    if (argc == 4)
    {
      results_path = std::filesystem::path{ argv[3] };
      if (results_path == tests_path)
      {
        results_path = std::filesystem::path{};
      }
      else
      {
        if (!std::filesystem::exists(results_path))
          throw app_exception{ app_exit_code_t::path_does_not_exist, "\"" + results_path.string() + "\" does not exist" };
        if (!std::filesystem::is_directory(results_path))
          throw app_exception{ app_exit_code_t::path_is_not_a_directory, "\"" + results_path.string() + "\" is not a directory" };
      }
    }

    logger sink{ std::cout, message_severity };
    processor::execute(sink, tests_path, results_path);
  }
  catch (const app_exception& e)
  {
    std::cout << e.what() << "\n";
    result = e.get_app_exit_code();
  }
  catch (const std::exception& e)
  {
    std::cerr << "std::exception caught: " << e.what() << "\n";
    result = app_exit_code_t::std_exception_caught;
  }
  catch (...)
  {
    std::cerr << "Unknown exception caught\n";
    result = app_exit_code_t::unknown_exception_caught;
  }
  return static_cast<int>(result);
}
