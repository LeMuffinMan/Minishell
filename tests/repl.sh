#!/bin/bash

# A small REPL on top of compare.sh: type the command lines you want to test,
# press [Enter] on an empty prompt to replay the whole sequence through
# minishell and bash at once.
#
# To customize this CLI, see the case block below: one branch is one command.

# shellcheck source-path=SCRIPTDIR
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"

resolve_minishell || exit 1
clean_stale_sandboxes
mkdir -p "$LOG_DIR"

# Uncomment this line to help debugging
# set -x

# these variables set timeout duration, valgrind check and redirection check
# just switch one to 1 to enable it by default, or use the CLI commands
TIMEOUT_DURATION=10
VALGRIND_FLAG=0
R_FLAG=0

# Here you can enable or disable files existence or permissions by default
# *_FLAG = 0 means the file will exist during tests / 1 means it will NOT
# *_PERM_FLAG = 0 means chmod 644 / 1 means chmod 000
INFILE_FLAG=0
INFILE_PERM_FLAG=0
FILE1_FLAG=0
FILE1_PERM_FLAG=0
FILE2_FLAG=0
FILE2_PERM_FLAG=0
OUTFILE_FLAG=0
OUTFILE_PERM_FLAG=0

COMPARE="$TESTS_DIR/compare.sh"
CASES="$TESTS_DIR/cases.sh"

ARGS=()
LAST_SEQ=()

# runs the sequence given as arguments and reports a timeout if it hangs
run_sequence()
{
  set_flags
  timeout "${TIMEOUT_DURATION}s" "$COMPARE" "${FLAGS[@]}" "$@" 2> /dev/null
  if [[ $? -eq 124 ]]; then
    echo -e "${RED}TIME OUT !${NC}"
  fi
}

# appends one sequence to cases.sh, escaping what the shell would eat back
append_case()
{
  local arg
  echo -n 'check ' >> "$CASES"
  for arg in "$@"; do
    # shellcheck disable=SC2016  # the backslashes are literal on purpose
    printf '"%s" ' "$(sed -e 's/\\/\\\\/g' -e 's/"/\\"/g' -e 's/\$/\\$/g' -e 's/`/\\`/g' <<< "$arg")" >> "$CASES"
  done
  echo >> "$CASES"
  echo -e "${YELLOW}$*\nadded to cases.sh${NC}"
  echo
}

echo -en "${YELLOW}[minishell-test]\$ ${NC}"

# As a simple CLI, I used a read in a while, with a switch case. Each case
# corresponds to one command; an input matching none is added to the sequence.
# IFS= and -r let the user type at the the test suite prompt exactly the way they
# would type at the bash or minishell prompt.
while IFS= read -r INPUT; do
  case "$INPUT" in
    # the ! commands lead tests quickly, loading the last sequence back
    # and executing it with an additional option
    "!")
      ARGS=("${LAST_SEQ[@]}")
      printf "${YELLOW}%s\n" "${LAST_SEQ[@]}"
      echo -en "$NC"
      ;;
    "!!")
      run_sequence "${LAST_SEQ[@]}"
      ;;
    # replay the last sequence with a valgrind check on top
    "!v")
      VALGRIND_FLAG=1
      run_sequence "${LAST_SEQ[@]}"
      VALGRIND_FLAG=0
      ;;
    # replay the last sequence with a redirection check on top
    "!>")
      R_FLAG=1
      run_sequence "${LAST_SEQ[@]}"
      R_FLAG=0
      ;;
    # rebuild minishell, then replay
    "!br")
      if make -C "$TESTS_DIR/.."; then
        run_sequence "${LAST_SEQ[@]}"
      fi
      ;;
    "--bake-re")
      make -C "$TESTS_DIR/.."
      ;;
    "-h"|"--help")
      print_help
      ;;
    # quit and clean up the sandbox
    "bye"|"quit")
      chmod -R u+w "$WORK_DIR" 2> /dev/null
      rm -rf "$WORK_DIR" 2> /dev/null
      exit 0
      ;;
    "--print=stdout"|"-po")
      print_stdout
      ;;
    "--print=stderr"|"-pe")
      print_stderr
      ;;
    "--print=valgrind"|"-pvg")
      cat "$LOG_DIR/valgrind_output" 2> /dev/null
      ;;
    "--print=outfile")
      cat "$LOG_DIR/outfile" 2> /dev/null
      ;;
    # Valgrind test switch
    "--valgrind"|"-vg")
      VALGRIND_FLAG=$((1 - VALGRIND_FLAG))
      echo -e "${YELLOW}valgrind_flag = $( [[ $VALGRIND_FLAG -eq 1 ]] && echo ON || echo OFF )${NC}"
      ;;
    # Redirection test switch
    "- >")
      R_FLAG=$((1 - R_FLAG))
      echo -e "${YELLOW}> = $( [[ $R_FLAG -eq 1 ]] && echo ON || echo OFF )${NC}"
      ;;
    "--print=flags"|"-pf")
      print_flags
      ;;
    # Display the sequence in the buffer
    "--print=seq"|"-ps")
      printf "${YELLOW}%s\n" "${ARGS[@]}"
      echo -en "$NC"
      ;;
    # Display the last sequence used
    "--print=last-seq"|"-pls")
      printf "${YELLOW}%s\n" "${LAST_SEQ[@]}"
      echo -en "$NC"
      ;;
    # run the whole suite
    "--suite")
      "$CASES"
      ;;
    "--suite-leaks")
      TEST_LEAKS=1 "$CASES"
      ;;
    # after a test, use ! then --add-case to store the sequence in cases.sh
    "--add-case")
      if [[ ${#ARGS[@]} -ne 0 ]]; then
        append_case "${ARGS[@]}"
      elif [[ ${#LAST_SEQ[@]} -ne 0 ]]; then
        append_case "${LAST_SEQ[@]}"
      else
        echo -e "${RED}nothing to add${NC}"
      fi
      ;;
    # remove the last input from the sequence
    "--oops"|"-o")
      if [[ ${#ARGS[@]} -gt 0 ]]; then
        echo -e "${YELLOW}removed : ${ARGS[-1]}${NC}"
        unset 'ARGS[-1]'
      fi
      ;;
    # This block sets switches for existing files and permissions
    "--infile")
      INFILE_FLAG=$((1 - INFILE_FLAG))
      echo -e "${YELLOW}infile existing = $( [[ $INFILE_FLAG -eq 0 ]] && echo ON || echo OFF )${NC}"
      ;;
    "--infile-perm")
      INFILE_PERM_FLAG=$((1 - INFILE_PERM_FLAG))
      echo -e "${YELLOW}infile perm = $( [[ $INFILE_PERM_FLAG -eq 0 ]] && echo ON || echo OFF )${NC}"
      ;;
    "--file1")
      FILE1_FLAG=$((1 - FILE1_FLAG))
      echo -e "${YELLOW}file1 existing = $( [[ $FILE1_FLAG -eq 0 ]] && echo ON || echo OFF )${NC}"
      ;;
    "--file1-perm")
      FILE1_PERM_FLAG=$((1 - FILE1_PERM_FLAG))
      echo -e "${YELLOW}file1 perm = $( [[ $FILE1_PERM_FLAG -eq 0 ]] && echo ON || echo OFF )${NC}"
      ;;
    "--file2")
      FILE2_FLAG=$((1 - FILE2_FLAG))
      echo -e "${YELLOW}file2 existing = $( [[ $FILE2_FLAG -eq 0 ]] && echo ON || echo OFF )${NC}"
      ;;
    "--file2-perm")
      FILE2_PERM_FLAG=$((1 - FILE2_PERM_FLAG))
      echo -e "${YELLOW}file2 perm = $( [[ $FILE2_PERM_FLAG -eq 0 ]] && echo ON || echo OFF )${NC}"
      ;;
    "--outfile")
      OUTFILE_FLAG=$((1 - OUTFILE_FLAG))
      echo -e "${YELLOW}outfile existing = $( [[ $OUTFILE_FLAG -eq 0 ]] && echo ON || echo OFF )${NC}"
      ;;
    "--outfile-perm")
      OUTFILE_PERM_FLAG=$((1 - OUTFILE_PERM_FLAG))
      echo -e "${YELLOW}outfile perm = $( [[ $OUTFILE_PERM_FLAG -eq 0 ]] && echo ON || echo OFF )${NC}"
      ;;

    # add your custom cmd here
    # "--cmd")
    #   <execution>
    #   ;;

    # no input : if the buffer is not empty, the sequence goes to compare.sh
    # if the buffer is empty, print a helper
    "")
      if [[ ${#ARGS[@]} -eq 0 ]]; then
        echo -e "${YELLOW}No inputs recorded"
        echo ""
        echo -e "use -h or --help for usage informations${NC}"
        print_flags
      else
        run_sequence "${ARGS[@]}"
        LAST_SEQ=("${ARGS[@]}")
        ARGS=()
      fi
      ;;
    # Any other input is added to the sequence to test
    *)
      ARGS+=("$INPUT")
      ;;
  esac
  echo -en "${YELLOW}[minishell-test]\$ ${NC}"
done
