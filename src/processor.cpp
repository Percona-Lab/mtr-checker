#include <map>
#include <string>
#include <string_view>

#include "processor.hpp"

#include "test_component_set.hpp"
#include "logger.hpp"

static bool has_suffix(const std::string& s, std::string_view suffix) noexcept
{
  return s.size() >= suffix.size() &&
    s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static constexpr std::string_view extension_test = ".test";
static constexpr std::string_view extension_result = ".result";
static constexpr std::string_view extension_cnf = ".cnf";
static constexpr std::string_view extension_opt = ".opt";
static constexpr std::string_view extension_sh = ".sh";
static constexpr std::string_view extension_combinations = ".combinations";

static constexpr std::string_view suffix_master = "-master";
static constexpr std::string_view suffix_slave = "-slave";
static constexpr std::string_view suffix_client = "-client";

static constexpr std::string_view stem_suit_opt = "suite";

processor::processor(logger& sink) :
  sink_{ &sink }, test_cases_{}
{}

processor::~processor() = default;

void processor::process_directory(const std::filesystem::path & root, directory_processing_mode_type directory_processing_mode)
{
  for (const auto& entry : std::filesystem::directory_iterator(root))
  {
    const auto& p = entry.path();
    auto filename = p.filename();
    if (!std::filesystem::is_regular_file(p))
      sink_->log(message_severity_t::error, "found \"", filename.string(), "\" that is not a regular file");
    else
    {
      if (std::filesystem::file_size(p) == 0)
        sink_->log(message_severity_t::warning, "found \"", filename.string(), "\" of size 0");

      auto stem = filename.stem().string();
      auto extension = filename.extension().string();
      auto test_component = test_component_t::delimiter;

      if (extension == extension_result)
        test_component = test_component_t::result;
      else if (extension == extension_test)
        test_component = test_component_t::test;
      else if (extension == extension_cnf)
        test_component = test_component_t::cnf;
      else if (extension == extension_combinations)
        test_component = test_component_t::combinations;
      else if (extension == extension_opt)
      {
        if (has_suffix(stem, suffix_master))
        {
          test_component = test_component_t::opt_master;
          stem.resize(stem.size() - suffix_master.size());
        }
        else if (has_suffix(stem, suffix_slave))
        {
          test_component = test_component_t::opt_slave;
          stem.resize(stem.size() - suffix_slave.size());
        }
        else if (has_suffix(stem, suffix_client))
        {
          test_component = test_component_t::opt_client;
          stem.resize(stem.size() - suffix_client.size());
        }
        else
          test_component = test_component_t::opt;
      }
      else if (extension == extension_sh)
      {
        if (has_suffix(stem, suffix_master))
        {
          test_component = test_component_t::sh_master;
          stem.resize(stem.size() - suffix_master.size());
        }
        else if (has_suffix(stem, suffix_slave))
        {
          test_component = test_component_t::sh_slave;
          stem.resize(stem.size() - suffix_slave.size());
        }
      }

      if (test_component == test_component_t::delimiter)
      {
        sink_->log(directory_processing_mode == directory_processing_mode_type::results ? message_severity_t::error : message_severity_t::warning, "found unknown file \"", filename.string(), "\"");
      }
      else
      {
        bool update_test_cases = true;
        if (test_component == test_component_t::opt && stem == stem_suit_opt)
        {
          sink_->log(message_severity_t::info, "found  \"", stem_suit_opt, extension_opt, "\"");
          update_test_cases = false;
        }
        if (directory_processing_mode == directory_processing_mode_type::tests && test_component == test_component_t::result)
        {
          sink_->log(message_severity_t::error, "unexpected file found in the tests directory \"", filename.string(), "\"");
          update_test_cases = false;
        }
        if (directory_processing_mode == directory_processing_mode_type::results && test_component != test_component_t::result)
        {
          sink_->log(message_severity_t::error, "unexpected file found in the results directory \"", filename.string(), "\"");
          update_test_cases = false;
        }
        if (update_test_cases) {
          assert(!test_cases_[stem].is_set(test_component));
          test_cases_[stem].set(test_component);
        }
      }
    }
  }
}

/* static */
void processor::execute(logger & sink, const std::filesystem::path & tests_path, const std::filesystem::path & results_path)
{
  processor proc{ sink };

  if (results_path.empty())
  {
    proc.process_directory(tests_path, directory_processing_mode_type::combined);
  }
  else
  {
    proc.process_directory(tests_path, directory_processing_mode_type::tests);
    proc.process_directory(results_path, directory_processing_mode_type::results);
  }
  proc.analyze_test_cases();

}

void processor::analyze_test_cases()
{
  for (const auto& [name, components] : test_cases_)
  {
    if (!components.check())
      sink_->log(message_severity_t::error, "inconsistent test case \"", name, "\": ", components);
    else
      sink_->log(message_severity_t::info, "verified test case \"", name, "\": ", components);
  }
}
