#include "parg.hh"
using Parg = OB::Parg;

#include "crex.hh"
using Crex = OB::Crex;

#include <iostream>
#include <regex>
#include <unistd.h>

int program_options(Parg& pg);
std::regex_constants::syntax_option_type regex_options(Parg& pg);
std::regex_constants::match_flag_type regex_flags(Parg& pg);
bool is_tty();

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
  if (pg.get<bool>("basic"))
  {
    opts |= std::regex::basic;
  }
  if (pg.get<bool>("extended"))
  {
    opts |= std::regex::extended;
  }
  if (pg.get<bool>("awk"))
  {
    opts |= std::regex::awk;
  }
  if (pg.get<bool>("grep"))
  {
    opts |= std::regex::grep;
  }
  if (pg.get<bool>("egrep"))
  {
    opts |= std::regex::egrep;
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

int main(int argc, char *argv[])
{
  Parg pg {argc, argv};
  int pstatus {program_options(pg)};
  if (pstatus > 0) return 0;
  if (pstatus < 0) return 1;

  try
  {
    OB::Crex crex {};
    crex.regex(pg.get("regex"));
    crex.text(pg.get_stdin() + pg.get("string"));
    crex.options(regex_options(pg));
    crex.flags(regex_flags(pg));

    if (! crex.run())
    {
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
