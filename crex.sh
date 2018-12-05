#!/usr/bin/env bash
set -e

# name: crex.sh
# info: crex interactive mode using entr
#       file_regex contains the regex text
#       file_text contains the text to apply the regex on
#       while running this script edit to the two files
#         and see the output changes on save

if [[ $# == 2 ]]; then
  printf "$1\n$2\n" | entr -c -s "cat ${2} | crex -r \"\$(head -1 ${1})\""
elif [[ $# == 3 ]]; then
  printf "$2\n$3\n" | entr -c -s "cat ${3} | crex ${1} -r \"\$(head -1 ${2})\""
else
  printf "crex.sh:\n"
  printf "  crex interactive mode using entr\n\n"
  printf "usage:\n"
  printf "  crex.sh <file_regex> <file_text>\n"
  printf "  crex.sh <crex_flags> <file_regex> <file_text>\n\n"
  printf "example:\n"
  printf "  crex.sh regex.txt text.txt\n"
  printf "  crex.sh -oc regex.txt text.txt\n"
  exit 1
fi
