#include "crex.hh"

#include <string>
#include <vector>
#include <regex>

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

std::string Crex::regex()
{
  return _regex;
}

Crex& Crex::text(std::string text_)
{
  _text = std::move(text_);
  return *this;
}

std::string Crex::text()
{
  return _text;
}

Crex& Crex::options(std::regex_constants::syntax_option_type opts_)
{
  _opts = std::move(opts_);
  return *this;
}

std::regex_constants::syntax_option_type Crex::options()
{
  return _opts;
}

Crex& Crex::flags(std::regex_constants::match_flag_type flgs_)
{
  _flgs = std::move(flgs_);
  return *this;
}

std::regex_constants::match_flag_type Crex::flags()
{
  return _flgs;
}

bool Crex::run()
{

  return true;
}

} // namespace OB
