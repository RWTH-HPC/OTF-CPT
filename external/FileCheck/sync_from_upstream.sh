#!/bin/bash
set -e

if [ "$#" -ne 1 ] || [ "$1" == "--help" ] || [ "$1" == "-h" ]
then
  echo "Helper script to update FileCheck from LLVM sources"
  echo "Usage: sync_from_upstream.sh <path-to-llvm-git-root>"
  exit 1
fi

LLVM_SOURCE_DIR="$1"
# The script's parent directory
TARGET_DIR=$(dirname "$(readlink -f "$0")")
# We use rsync here because it has a very handy exclude option.
RSYNC="rsync -r --exclude=**/CMakeLists.txt"

# FileCheck main file
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/utils/FileCheck/FileCheck.cpp" "${TARGET_DIR}/"

# LLVM Config
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/include/llvm/Config/config.h.cmake" "${TARGET_DIR}/lib/Config/"
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/include/llvm/Config/llvm-config.h.cmake" "${TARGET_DIR}/lib/Config/"
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/include/llvm/Config/abi-breaking.h.cmake" "${TARGET_DIR}/lib/Config/"

# FileCheck library
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/lib/FileCheck/" "${TARGET_DIR}/lib/FileCheck"
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/include/llvm/FileCheck/" "${TARGET_DIR}/lib/FileCheck/include/llvm/FileCheck"

# LLVM Support library
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/lib/Support/" "${TARGET_DIR}/lib/Support"
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/include/llvm/Support/" "${TARGET_DIR}/lib/Support/include/llvm/Support"
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/include/llvm/ADT/" "${TARGET_DIR}/lib/ADT/include/llvm/ADT"

# LLVM Demangle library
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/lib/Demangle/" "${TARGET_DIR}/lib/Demangle"
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/include/llvm/Demangle/" "${TARGET_DIR}/lib/Demangle/include/llvm/Demangle"

# llvm-c
${RSYNC} "${LLVM_SOURCE_DIR}/llvm/include/llvm-c/" "${TARGET_DIR}/lib/llvm-c/include/llvm-c"
