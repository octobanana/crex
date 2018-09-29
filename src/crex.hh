#ifndef OB_CREX_HH
#define OB_CREX_HH

#include <string>
#include <vector>
#include <regex>

namespace OB
{

class Crex
{
public:

  Crex();
  ~Crex();

  Crex& regex(std::string regex_);
  std::string regex();

  Crex& text(std::string text_);
  std::string text();

  Crex& options(std::regex_constants::syntax_option_type opts_);
  std::regex_constants::syntax_option_type options();

  Crex& flags(std::regex_constants::match_flag_type flgs_);
  std::regex_constants::match_flag_type flags();

  bool run();

private:
  std::string _regex;
  std::string _text;

  std::regex_constants::syntax_option_type _opts {};
  std::regex_constants::match_flag_type _flgs {};

  std::vector<std::string> split(std::string str, std::string delim, int match = 0)
  {
    std::vector<std::string> vtok;
    size_t start {0};
    auto end = str.find(delim);
    while (end != std::string::npos && match-- > 0) {
      vtok.emplace_back(str.substr(start, end - start));
      start = end + delim.length();
      end = str.find(delim, start);
    }
    vtok.emplace_back(str.substr(start, end));
    return vtok;
  }

}; // class Crex

} // namespace OB

#endif // OB_CREX_HH
