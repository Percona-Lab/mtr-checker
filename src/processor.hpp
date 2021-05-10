#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <filesystem>
#include <string>
#include <unordered_map>
#include "test_component_set.hpp"

class logger;

class processor
{
public:
  static void execute(logger& sink, const std::filesystem::path& tests_path, const std::filesystem::path& results_path);

private:
  logger* sink_;
  using test_case_container = std::unordered_map<std::string, test_component_set>;
  test_case_container test_cases_;

private:
  processor(logger& sink);
  ~processor();
  processor(const processor&) = delete;
  processor& operator = (const processor&) = delete;
  processor(processor&&) = delete;
  processor& operator = (processor&&) = delete;

  enum class directory_processing_mode_type
  {
    tests,
    results,
    combined
  };

  void process_directory(const std::filesystem::path& root, directory_processing_mode_type directory_processing_mode);
  void analyze_test_cases();
};

#endif // PROCESSOR_HPP
