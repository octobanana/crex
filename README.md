# crex
Explore, test, and check regular expressions in the terminal.

![crex example](https://raw.githubusercontent.com/octobanana/crex/master/assets/crex.png)

Given your regular expression and text, crex will output matches, capture groups, and details.
crex has a range of options, allowing fine grained control over matching and output.
It uses ECMAScript grammar by default, while also accepting posix, extended posix, awk, grep, and extended grep grammars.
Input can be received by piped stdin, or by using the __-s__ option.
Output options include plain, colour, and json formats.

## Build
Environment:
* tested on linux
* c++ 14 compiler
* cmake

Libraries:
* my [parg](https://github.com/octobanana/parg) library, for parsing cli args, included as `./src/parg.hh`

The following shell commands will build the project:
```bash
git clone <repo_url>
cd crex
./build.sh -r
```
To build the debug version, run the build script without the -r flag.

## Install
The following shell commands will install the project:
```bash
./install.sh -r
```
