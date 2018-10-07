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
  auto regex_begin = std::sregex_iterator(_text.begin(), _text.end(), rx, _flgs);
  auto regex_end = std::sregex_iterator();

  size_t pos {0};
  for (auto imatch = regex_begin; imatch != regex_end; ++imatch)
  {
    std::smatch match {*imatch};
    pos = static_cast<size_t>(match.position());
    std::string tmp {match[0].str()};

    _matches.emplace_back(Match());

    _matches.back().emplace_back(
      std::make_pair(match[0].str(),
      std::make_pair(pos, pos + static_cast<size_t>(match[0].length()) - 1))
    );

    size_t pos_match {pos};
    for (size_t i = 1; i < match.size(); ++i)
    {
      auto v = split(tmp, match[i].str());
      pos_match += v.first.size();

      _matches.back().emplace_back(
        std::make_pair(match[i].str(),
        std::make_pair(pos_match, pos_match + static_cast<size_t>(match[i].length()) - 1))
      );

      tmp = v.second;
      pos_match += static_cast<size_t>(match[i].length());
    }
  }

  return (! _matches.empty());
}

Crex::Matches const& Crex::matches() const
{
  return _matches;
}

std::pair<std::string, std::string> Crex::split(std::string str, std::string delim) const
{
  std::pair<std::string, std::string> ptok {"", ""};
  auto pos = str.find(delim);
  if (pos != std::string::npos) {
    ptok.first = str.substr(0, pos);
    if (pos + delim.size() != std::string::npos)
    {
      ptok.second = str.substr(pos + delim.size());
    }
  }
  return ptok;
}

} // namespace OB
