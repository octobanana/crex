#include "parg.hh"
using Parg = OB::Parg;

#include "crex.hh"
using Crex = OB::Crex;

#include "ansi_escape_codes.hh"
namespace aec = OB::ANSI_Escape_Codes;

#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <unistd.h>

int program_options(Parg& pg);
std::regex_constants::syntax_option_type regex_options(Parg& pg);
std::regex_constants::match_flag_type regex_flags(Parg& pg);
bool is_tty();
void regex_print(std::string const& regex, std::string const& text,
  Crex::Matches const& matches, bool color);
void regex_print_color(std::string const& regex, std::string const& text,
  Crex::Matches const& matches);
void regex_print_no_color(std::string const& regex, std::string const& text,
  Crex::Matches const& matches);
void regex_print_json(std::string const& regex, std::string const& text,
  Crex::Matches const& matches);
std::string replace(std::string str, std::string key, std::string val);

int program_options(Parg& pg)
{
  pg.name("crex").version("0.1.0 (28.09.2018)");
  pg.description("explore, test, and check regular expressions");
  pg.usage("[flags] [options] [--] [arguments]");
  pg.usage("[-r regex] [-s string] [-e|-b|-x|-a|-g|-E] [-ioc]");
  pg.usage("[-v|--version]");
  pg.usage("[-h|--help]");
  pg.info("Examples", {
    "crex -r '(hello)' -s 'hello world!'",
    "crex --help",
    "crex --version",
  });
  pg.info("Exit Codes", {"0 -> normal", "1 -> error"});
  pg.info("Repository", {
    "https://github.com/octobanana/crex.git",
  });
  pg.info("Homepage", {
    "https://octobanana.com/software/crex",
  });
  pg.author("Brett Robinson (octobanana) <octobanana.dev@gmail.com>");

  // single flags
  pg.set("help,h", "print the help output");
  pg.set("version,v", "print the program version");

  // combinable flags
  pg.set("color,c", "print out results in color");
  pg.set("json,j", "print out results in json");

  // options
  pg.set("string,s", "", "str", "the string to search");
  pg.set("regex,r", "", "str", "the regular expression to use");
  pg.set("icase,i", "character matching should be performed without regard to case");
  pg.set("optimize,o", "instructs the regular expression engine to make matching faster, with the potential cost of making construction slower");
  pg.set("ecmascript,e", "use the modified ECMAScript regular expression grammar");
  pg.set("basic,b", "use the basic POSIX regular expression grammar");
  pg.set("extended,x", "use the extended POSIX regular expression grammar");
  pg.set("awk,a", "use the regular expression grammar used by the awk utility in POSIX");
  pg.set("grep,g", "use the regular expression grammar used by the grep utility in POSIX");
  pg.set("egrep,E", "use the regular expression grammar used by the grep utility, with the -E option, in POSIX");

  int status {pg.parse()};
  if (status < 0)
  {
    std::cout << pg.help() << "\n";
    std::cout << "Error: " << pg.error() << "\n";
    return -1;
  }
  if (pg.get<bool>("help"))
  {
    std::cout << pg.help();
    return 1;
  }
  if (pg.get<bool>("version"))
  {
    std::cout << pg.name() << " v" << pg.version() << "\n";
    return 1;
  }
  return 0;
}

std::regex_constants::syntax_option_type regex_options(Parg& pg)
{
  std::regex_constants::syntax_option_type opts {};

  if (pg.get<bool>("icase"))
  {
    opts |= std::regex::icase;
  }

  if (pg.get<bool>("optimize"))
  {
    opts |= std::regex::optimize;
  }

  if (pg.get<bool>("ecmascript"))
  {
    opts |= std::regex::ECMAScript;
  }
  else if (pg.get<bool>("basic"))
  {
    opts |= std::regex::basic;
  }
  else if (pg.get<bool>("extended"))
  {
    opts |= std::regex::extended;
  }
  else if (pg.get<bool>("awk"))
  {
    opts |= std::regex::awk;
  }
  else if (pg.get<bool>("grep"))
  {
    opts |= std::regex::grep;
  }
  else if (pg.get<bool>("egrep"))
  {
    opts |= std::regex::egrep;
  }
  else
  {
    opts |= std::regex::ECMAScript;
  }

  return opts;
}

std::regex_constants::match_flag_type regex_flags(Parg& pg)
{
  std::regex_constants::match_flag_type flgs {std::regex_constants::match_not_null};
  return flgs;
}

bool is_tty()
{
  return isatty(STDOUT_FILENO);
}

std::string replace(std::string str, std::string key, std::string val)
{
  size_t pos {0};
  for (;;)
  {
    pos = str.find(key, pos);
    if (pos == std::string::npos) break;
    str.replace(pos, key.size(), val);
    pos += val.size();
  }
  return str;
}

class Color_Ring
{
public:
  Color_Ring(std::vector<std::string> colors):
    _colors {colors}
  {
  }

  std::string next()
  {
    auto res = get();
    if (++_pos > _colors.size() - 1)
    {
      _pos = 0;
    }
    return res;
  }

  std::string get() const
  {
    return _colors.at(_pos);
  }

  Color_Ring& reset()
  {
    _pos = 0;
    return *this;
  }

private:
  std::vector<std::string> const _colors;
  size_t _pos {0};
};

struct Style
{
  std::vector<std::string> h1 {aec::fg_white, aec::bold, aec::underline};
  std::vector<std::string> h2 {aec::fg_green, aec::bold};
  std::vector<std::string> h3 {aec::fg_magenta, aec::bold};
  std::vector<std::string> num {aec::fg_cyan, aec::bold};
  std::string match {aec::fg_green};
  std::string plain {};
  std::vector<std::string> special {aec::fg_white, aec::bold};
  Color_Ring ring {{
    aec::fg_magenta,
    aec::fg_blue,
    aec::fg_cyan,
    aec::fg_green,
    aec::fg_yellow,
    aec::fg_red
  }};
};

void regex_print(std::string const& regex, std::string const& text,
  Crex::Matches const& matches, bool color)
{
  if (color)
  {
    regex_print_color(regex, text, matches);
  }
  else
  {
    regex_print_no_color(regex, text, matches);
  }
}

void regex_print_color(std::string const& regex, std::string const& text,
  Crex::Matches const& matches)
{
  Style style;

  std::stringstream ss_head;
  std::stringstream ss;

  ss_head
  << aec::wrap("Regex", style.h1) << "\n"
  << regex
  << "\n\n"
  << aec::wrap("Text", style.h1) << "\n";

  ss
  << "\n\n"
  << aec::wrap("Matches", style.h1) << "[" << aec::wrap(matches.size(), style.num) << "]\n";

  size_t pos {0};

  for (size_t i = 0; i < matches.size(); ++i)
  {
    style.ring.reset();

    auto const& match = matches.at(i);

    ss
    << aec::wrap("Match", style.h2) << "[" << aec::wrap(i + 1, style.num) << "] "
    << aec::wrap(match.at(0).second.first, style.num)
    << "-"
    << aec::wrap(match.at(0).second.second, style.num)
    << aec::wrap(" |", style.special);

    std::string const& tmatch {match.at(0).first};
    size_t const offset {match.at(0).second.first};

    size_t prev {0};
    size_t begin {0};
    size_t end {0};

    std::stringstream ss_text;
    std::stringstream ss_match;

    for (size_t j = 1; j < match.size(); ++j)
    {
      auto ftext = aec::wrap(match.at(j).first, style.ring.next());

      begin = match.at(j).second.first - offset;
      end = match.at(j).second.second - offset;

      if (prev < begin)
      {
        ss_text
        << aec::wrap(tmatch.substr(prev + 1, begin - prev - 1), style.plain);
      }
      ss_text
      << ftext;

      prev = end;

      ss_match
      << aec::wrap("Group", style.h3) << "[" << aec::wrap(j, style.num) << "] "
      << aec::wrap(begin, style.num)
      << "-"
      << aec::wrap(end, style.num)
      << aec::wrap(" |", style.special)
      << ftext
      << aec::wrap("|\n", style.special);
    }

    if (prev < tmatch.size() - 1)
    {
      ss_text
      << aec::wrap(tmatch.substr(prev + 1, begin - prev - 1), style.plain);
    }
    ss_match
    << "\n";

    ss
    << ss_text.str()
    << aec::wrap("|\n", style.special)
    << ss_match.str();

    if (pos < offset)
    {
      ss_head
      << aec::wrap(text.substr(pos + 1, offset - pos - 1), style.plain);
    }
    pos = offset + end;

    ss_head
    << aec::wrap("(", style.special)
    << ss_text.str()
    << aec::wrap(")[", style.special)
    << aec::wrap(i + 1, style.num)
    << aec::wrap("]", style.special);
  }

  if (pos < text.size() - 1)
  {
    ss_head
    << aec::wrap(text.substr(pos + 1, text.size() - pos - 1), style.plain);
  }

  std::cout << ss_head.str() << ss.str();
}

void regex_print_no_color(std::string const& regex, std::string const& text,
  Crex::Matches const& matches)
{
  std::stringstream ss;

  ss
  << "Regex\n"
  << regex
  << "\n\n"
  << "Text\n"
  << text
  << "\n\n"
  << "Matches[" << matches.size() << "]\n";

  for (size_t i = 0; i < matches.size(); ++i)
  {
    auto const& match = matches.at(i);

    ss
    << "Match[" << i + 1 << "] "
    << match.at(0).second.first << "-" << match.at(0).second.second
    << " |" << match.at(0).first << "|\n";

    for (size_t j = 1; j < match.size(); ++j)
    {
      ss
      << "Group[" << j << "] "
      << match.at(j).second.first << "-" << match.at(j).second.second
      << " |" << match.at(j).first << "|\n";
    }
    ss
    << "\n";
  }

  std::cout << ss.str();
}

void regex_print_json(std::string const& regex, std::string const& text,
  Crex::Matches const& matches)
{
  auto const get_matches = [&]() {
    std::stringstream ss;

    for (size_t i = 0; i < matches.size(); ++i)
    {
      auto const& match = matches.at(i);

      ss
      << "[";

      for (size_t j = 0; j < match.size(); ++j)
      {
        ss
        << "{"
        << "\"text\":\"" << replace(match.at(j).first, "\"", "\\\"") << "\","
        << "\"begin\":" << match.at(j).second.first << ","
        << "\"end\":" << match.at(j).second.second
        << "}";

        if (j != match.size() - 1)
        {
          ss
          << ",";
        }
      }

      ss
      << "]";

      if (i != matches.size() - 1)
      {
        ss
        << ",";
      }
    }

    return ss.str();
  };

  std::stringstream ss;

  ss
  << "{"
  << "\"regex\":\"" << replace(regex, "\"", "\\\"") << "\","
  << "\"text\":\"" << replace(text, "\"", "\\\"") << "\","
  << "\"matches\":"
  << "["
  << get_matches()
  << "]}";

  std::cout << ss.str();
}

int main(int argc, char *argv[])
{
  Parg pg {argc, argv};
  int pstatus {program_options(pg)};
  if (pstatus > 0) return 0;
  if (pstatus < 0) return 1;

  try
  {
    std::string regex {pg.get("regex")};
    std::string text {pg.get_stdin() + pg.get("string")};

    if (regex.empty() || text.empty())
    {
      return 1;
    }

    OB::Crex crex {};
    crex.regex(regex);
    crex.text(text);
    crex.options(regex_options(pg));
    crex.flags(regex_flags(pg));

    if (! crex.run())
    {
      return 1;
    }

    if (pg.get<bool>("json"))
    {
      regex_print_json(regex, text, crex.matches());
    }
    else
    {
      regex_print(regex, text, crex.matches(), pg.get<bool>("color"));
    }
  }
  catch (std::exception const& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "Error: an unexpected error occurred\n";
    return 1;
  }

  return 0;
}
