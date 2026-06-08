#!/bin/bash

# Parse and check args
EXECUTABLE_NAME="a.out"
USE_LEAKS=0
while [[ $# -gt 0 ]]; do
  case $1 in
  -l | --leaks)
    USE_LEAKS=1
    shift
    ;;
  *)
    MAIN_FILE="$1"
    shift
    ;;
  esac
done

if [ -z "$MAIN_FILE" ]; then
  echo "Error: Please provide the name of the main C file to execute."
  echo "Usage: $0 [-l|--leaks] <main_c_file>"
  echo "Example: $0 my/c/program.c"
  echo "Example: $0 -l my/c/program.c"
  exit 1
fi
if [ ! -f "$MAIN_FILE" ]; then
  echo "Error: Main file '$MAIN_FILE' not found."
  exit 1
fi

# Collect all .c files under modules/**/src/
UTILITY_FILES=$(find modules -path "*/src/*.c" 2>/dev/null)

# Build -I flags for every modules/**/include/ directory
INCLUDE_FLAGS=$(find modules -type d -name "include" | sed 's/^/-I/' | tr '\n' ' ')

if [ -z "$UTILITY_FILES" ]; then
  echo "Warning: No utility source files found under modules/"
fi

echo "-> Compiling $MAIN_FILE with utilities..."

# Compiling and linking
gcc "$MAIN_FILE" \
  $UTILITY_FILES \
  $INCLUDE_FLAGS \
  -o "$EXECUTABLE_NAME"

if [ $? -eq 0 ]; then
  echo "-> Compilation successful!"
  if [ "$USE_LEAKS" = "1" ]; then
    echo "-> Leak detection enabled"
  fi
  echo "-> Running program..."
  echo "----------------------------------------"
  if [ "$USE_LEAKS" = "1" ]; then
    leaks --atExit -- ./"$EXECUTABLE_NAME"
  else
    ./"$EXECUTABLE_NAME"
  fi
  EXEC_STATUS=$?
  echo "----------------------------------------"
  echo "-> Program finished with exit status $EXEC_STATUS."
else
  echo "-> Compilation FAILED."
  exit 1
fi
