#ifndef OB_CREX_HH
#define OB_CREX_HH

#include <string>
#include <vector>
#include <regex>
#include <utility>

namespace OB
{

class Crex
{
public:
  using Match = std::vector<std::pair<std::string, std::pair<size_t, size_t>>>;
  using Matches = std::vector<Match>;

  Crex();
  ~Crex();

  Crex& regex(std::string regex_);
  std::string regex() const;

  Crex& text(std::string text_);
  std::string text() const;

  Crex& options(std::regex_constants::syntax_option_type opts_);
  std::regex_constants::syntax_option_type options() const;

  Crex& flags(std::regex_constants::match_flag_type flgs_);
  std::regex_constants::match_flag_type flags() const;

  bool run();
  Matches const& matches() const;

private:
  std::string _regex;
  std::string _text;

  std::regex_constants::syntax_option_type _opts {};
  std::regex_constants::match_flag_type _flgs {};

  Matches _matches;

  std::pair<std::string, std::string> split(std::string str, std::string delim) const;

}; // class Crex

} // namespace OB

#endif // OB_CREX_HH
