#!/bin/bash

SCRIPT_NAME=$(basename "$0")

RED='\033[0;31m'
GREEN='\033[0;32m'
CLEAR='\033[0m'
Yellow='\033[0;33m'       
Blue='\033[0;34m'         

log_message() {
  local level="$1"
  local message="$2"
  echo -e "[${level}] (${SCRIPT_NAME}): ${message}" >&2
}

copy_file() {
  local source_file="$1"
  local dest_dir="$2"

  if [ ! -f "$source_file" ]; then
    log_message "${RED}" "${RED}Error: File ${CLEAR} ${Yellow} '$source_file' ${RED} does not exist in source directory${CLEAR}."
    exit 1
  fi

  sudo cp "$source_file" "$dest_dir"
  if [ $? -eq 0 ]; then

    log_message "${GREEN}" " File ${Yellow}'$source_file'${CLEAR} copied successfully to ${Yellow}'$dest_dir'.${CLEAR}"
  else
     log_message "${RED}" "${RED}\nError:\n ${CLEAR}"
    log_message "${RED}" "${RED}Error:${CLEAR} Failed to copy file ${RED}'$source_file'${CLEAR} to ${RED}'$dest_dir'.${CLEAR}"
        log_message "${RED}" "${RED}\n\nError:might have ${GREEN}insuffsient permssion\n  ${CLEAR}"
    exit 2;
  fi
}

source_dir="./"

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  dest_dir="/usr/include/c++/11"
elif [[ "$OSTYPE" == "darwin"* ]]; then
  dest_dir="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1"
else
  log_message "${RED}" "Error: Unsupported OS. This script only works for Linux and macOS.${CLEAR}"
  exit 1
fi

files=("iostreamS.h" "iostreamC.h")

if [ ! -d "$source_dir" ]; then
  log_message "${RED}" "Error: Source directory ${CLEAR} '$source_dir'${RED} does not exist.${CLEAR} "
  exit 1
fi

for file in "${files[@]}"; do
  copy_file "$source_dir/$file" "$dest_dir"
  sleep 1
done
sleep 0.5

echo -e  """ \n\n ${CLEAR} ---- server function ----
 ${GREEN}#include <iostreamS.h>
 int main(){
    char *ip="127.0.0.1";
    server(ip, 6969) ${CLEAR} //where 6969 is the port number
)

\n\n"""
sleep 0.5
echo -e  """ \n\n ${CLEAR} ---- client function ----
 ${GREEN}#include <iostreamC.h>
 int main(){
    char *ip="127.0.0.1";
    client(ip, 6969) ${CLEAR} //where 6969 is the port number
)

\n\n"""

sleep 2
echo -e "${GREEN}\ncopying done${CLEAR}\n"
exit


exit
