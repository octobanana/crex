#include "parg.hh"
using Parg = OB::Parg;

#include "crex.hh"
using Crex = OB::Crex;

#include <iostream>
#include <unistd.h>

int program_options(Parg& pg);

int program_options(Parg& pg)
{
  pg.name("crex").version("0.1.0 (28.09.2018)");
  pg.description("explore, test, and check regular expressions");
  pg.usage("[flags] [options] [--] [arguments]");
  pg.usage("[-r regex] [-s string]");
  pg.usage("[-v|--version]");
  pg.usage("[-h|--help]");
  pg.info("Examples", {
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

int main(int argc, char *argv[])
{
  Parg pg {argc, argv};
  int pstatus {program_options(pg)};
  if (pstatus > 0) return 0;
  if (pstatus < 0) return 1;

  try
  {
    OB::Crex crex {};
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
