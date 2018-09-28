# crex
Explore, test, and check regular expressions in the terminal.

Given your regular expression and text, crex will output matches, capture groups, and details.
crex also has a range of options, allowing fine grained control over matching and output.

## Build
Environment:
* tested on linux
* c++ 14 compiler
* cmake

Libraries:
* my [parg](https://github.com/octobanana/parg) library, for parsing cli args, included as `./src/parg.hh`

The following shell commands will build the project:
```bash
git clone <repo_name>
cd <repo_name>
./build.sh -r
```
To build the debug version, run the build script without the -r flag.

## Install
The following shell commands will install the project:
```bash
./install.sh -r
```
