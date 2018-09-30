#include "crex.hh"

#include <string>
#include <vector>
#include <regex>
#include <utility>

namespace OB
{

Crex::Crex()
{
}

Crex::~Crex()
{
}

Crex& Crex::regex(std::string regex_)
{
  _regex = std::move(regex_);
  return *this;
}

std::string Crex::regex() const
{
  return _regex;
}

Crex& Crex::text(std::string text_)
{
  _text = std::move(text_);
  return *this;
}

std::string Crex::text() const
{
  return _text;
}

Crex& Crex::options(std::regex_constants::syntax_option_type opts_)
{
  _opts = std::move(opts_);
  return *this;
}

std::regex_constants::syntax_option_type Crex::options() const
{
  return _opts;
}

Crex& Crex::flags(std::regex_constants::match_flag_type flgs_)
{
  _flgs = std::move(flgs_);
  return *this;
}

std::regex_constants::match_flag_type Crex::flags() const
{
  return _flgs;
}

bool Crex::run()
{
  std::regex rx {_regex, _opts};

  std::string tmp = _text;
  size_t pos {0};
  std::smatch match;
  while (std::regex_search(tmp, match, rx, _flgs))
  {
    pos += std::string(match.prefix()).size();
    std::string t {match[0]};

    _matches.emplace_back(Match());

    _matches.back().emplace_back(std::make_pair(std::string(match[0]), std::make_pair(pos, pos + std::string(match[0]).size() - 1)));

    size_t pos_match {pos};
    for (size_t i = 1; i < match.size(); ++i)
    {
      auto v = split(t, std::string(match[i]), 2);

      pos_match += v.at(0).size();

      _matches.back().emplace_back(std::make_pair(std::string(match[i]), std::make_pair(pos_match, pos_match + std::string(match[i]).size() - 1)));

      t = v.at(1);
      pos_match += std::string(match[i]).size();
    }

    pos += std::string(match[0]).size();
    tmp = std::string(match.suffix());

    if (tmp.empty()) break;
  }

  return (! _matches.empty());
}

Crex::Matches const& Crex::matches() const
{
  return _matches;
}

std::vector<std::string> Crex::split(std::string str, std::string delim, int times) const
{
  std::vector<std::string> vtok;
  size_t start {0};
  auto end = str.find(delim);
  while (end != std::string::npos && times-- > 0)
  {
    vtok.emplace_back(str.substr(start, end - start));
    start = end + delim.length();
    end = str.find(delim, start);
  }
  vtok.emplace_back(str.substr(start, end));
  return vtok;
}

} // namespace OB
