#!/bin/bash

# Shared helpers for the minishell differential test suite.
# Sourced by compare.sh, cases.sh and repl.sh.

GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[0;33m"
CYAN='\033[0;36m'
NC="\033[0m"

# ---------------------------------------------------------------------------
# Layout
#
# Every test runs inside SANDBOX, never in the repository. Cases create and
# delete files (out, test1, a/b/c, ...) and some of them walk out with `cd ..`,
# so the shells get their own throwaway directory and the repo stays clean.
# ---------------------------------------------------------------------------

TESTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SANDBOX_ROOT="$TESTS_DIR/.sandbox"

# One sandbox per run, not one per checkout: a suite running in one terminal
# and a REPL in another would otherwise overwrite each other's logs mid-case
# and report failures that never happened. The entry point that sources this
# file first sets the id; compare.sh inherits it.
TEST_RUN_ID="${TEST_RUN_ID:-$$}"
export TEST_RUN_ID
WORK_DIR="$SANDBOX_ROOT/$TEST_RUN_ID"

# Where the shells run. Wiped and rebuilt before each of them, so both start
# from the same state. Everything a case creates lands here.
SANDBOX="$WORK_DIR/run"

# Where the reports are written. Outside SANDBOX on purpose: the wipe must not
# take away what the previous shell produced.
LOG_DIR="$WORK_DIR/results"

# shellcheck disable=SC2034  # read by compare.sh, which sources this file
SUPPRESSIONS="$TESTS_DIR/readline.supp"

# Override with MINISHELL=/path/to/binary to test another build.
MINISHELL="${MINISHELL:-$TESTS_DIR/../minishell}"

# minishell refuses a non-tty stdin on purpose; this lets the suite drive it
# through a here_doc. See src/misc/misc.c
export MINISHELL_TEST=1

# Both shells run under the same, fixed locale. bash sorts what a wildcard
# matches with strcoll, so its order follows LC_COLLATE and the same run
# gives different results on two machines. strcoll is not in the list of
# functions the subject allows, so minishell sorts by byte value, which is
# what LC_ALL=C asks of bash too.
export LC_ALL=C
export LANG=C

# Counters filled by check(), read by print_summary()
PASSED=0
FAILED=0

# Resolve the binary to an absolute path, since every script cd's into SANDBOX.
resolve_minishell()
{
  if [[ ! -x $MINISHELL ]]; then
    echo -e "${RED}error: no minishell binary at '$MINISHELL'${NC}" >&2
    echo "build it with 'make' at the root of the repository," >&2
    echo "or point the suite elsewhere with MINISHELL=/path/to/binary" >&2
    return 1
  fi
  MINISHELL="$(cd "$(dirname "$MINISHELL")" && pwd)/$(basename "$MINISHELL")"
  return 0
}

# Drops the sandboxes of runs that are no longer alive, so they do not pile up.
# The last run's logs survive until the next one starts, which is when you
# actually want to read them.
clean_stale_sandboxes()
{
  local dir pid
  for dir in "$SANDBOX_ROOT"/*; do
    [[ -d $dir ]] || continue
    pid="${dir##*/}"
    [[ $pid == "$TEST_RUN_ID" ]] && continue
    [[ $pid =~ ^[0-9]+$ ]] || continue
    kill -0 "$pid" 2> /dev/null && continue
    chmod -R u+w "$dir" 2> /dev/null
    rm -rf "$dir"
  done
  return 0
}

enter_sandbox()
{
  mkdir -p "$LOG_DIR" "$SANDBOX"
  cd "$SANDBOX" || return 1
}

# Wipes the sandbox and lays down the default fixtures again.
#
# This is called once before minishell runs and once before bash runs: the
# check create and delete files, so replaying the second shell on whatever
# the first one left behind compares two different situations. Both shells
# must start from the same directory, in the same state, at the same path --
# hence one directory reset twice, and not two directories, which would make
# every `pwd` test disagree.
prepare_sandbox()
{
  chmod -R u+w "$SANDBOX" 2> /dev/null
  find "$SANDBOX" -mindepth 1 -delete 2> /dev/null
  # log/ inside the sandbox holds the fixtures the cases read and write
  mkdir -p "$SANDBOX/log"

  # comment this out to test an empty infile, or customize for your taste
  if [[ $INFILE == 1 ]]; then
    printf '%s\n' \
      "Some people... some people like cupcakes, exclusively..." \
      "while myself, I say, there is naught nor ought there be nothing" \
      "So exalted on the face of god's grey earth as that prince of foods..." \
      "the muffin!" \
      "" \
      "Frank Zappa - Muffin Man" > "$SANDBOX/log/infile"
    [[ $INFILE_PERM == 0 ]] && chmod 000 "$SANDBOX/log/infile"
  fi
  if [[ $OUTFILE == 1 ]]; then
    : > "$SANDBOX/log/outfile"
    [[ $OUTFILE_PERM == 0 ]] && chmod 000 "$SANDBOX/log/outfile"
  fi
  if [[ $FILE1 == 1 ]]; then
    : > "$SANDBOX/log/file1"
    [[ $FILE1_PERM == 0 ]] && chmod 000 "$SANDBOX/log/file1"
  fi
  if [[ $FILE2 == 1 ]]; then
    : > "$SANDBOX/log/file2"
    [[ $FILE2_PERM == 0 ]] && chmod 000 "$SANDBOX/log/file2"
  fi

  # There is a no-perm file available anyway
  : > "$SANDBOX/log/file_without_permissions"
  chmod 000 "$SANDBOX/log/file_without_permissions"
  return 0
}

# replays one case through compare.sh and turns its output into one report line
check()
{
  local output report ko=0
  local -a filtered_args run_args

  # valgrind is a switch on the whole run, not something written in the cases
  run_args=()
  [[ ${TEST_LEAKS:-0} == 1 ]] && run_args+=("--leaks")
  run_args+=("$@")

  # the colour stripping is done after, and not in a pipe: the exit status of
  # `var=$(cmd | sed)` is the one of sed, which would swallow the 124 of timeout
  output=$(timeout "${TIMEOUT_DURATION}s" "$TESTS_DIR/compare.sh" "${run_args[@]}")
  local rc=$?
  output=$(sed -r 's/\x1B\[[0-9;]*m//g' <<< "$output")

  # keep only the command lines, so the report shows the test and not its flags
  while [[ $# -gt 0 ]]; do
    case "$1" in
      --leaks|--as|-r|--infile=*|--file1=*|--file2=*|--outfile=*)
        shift
        ;;
      *)
        filtered_args+=("$1")
        shift
        ;;
    esac
  done

  if [[ $rc -eq 124 || -z $output ]]; then
    echo -e "${filtered_args[*]} : ${RED}TIME OUT !${NC}"
    ((FAILED++))
    return
  fi

  report="$output"

  # each check below maps one compare.sh verdict to one line in the report
  local check
  for check in "STDOUT : KO|STDOUT" \
               "STDERR : KO|STDERR" \
               "EXIT : KO|EXIT" \
               "REDIR > : KO|REDIR >" \
               "SEGMENTATION FAULT|SEGMENTATION FAULT" \
               "LEAKS !|LEAKS" \
               "ERRORS !|VALGRIND ERRORS" \
               "FD OPEN AT EXIT !|FD LEAK"; do
    local pattern="${check%%|*}"
    local label="${check##*|}"
    if grep -qF "$pattern" <<< "$report"; then
      echo -e "${filtered_args[*]} : $label :${RED} KO${NC}"
      ko=1
    fi
  done

  if [[ $ko == 0 ]]; then
    echo -e "${filtered_args[*]} :${GREEN} OK${NC}"
    ((PASSED++))
  else
    ((FAILED++))
  fi
}

print_summary()
{
  local total=$((PASSED + FAILED))
  echo
  echo -e "${CYAN}────────────────────────────────────────${NC}"
  echo -e " cases run : $total"
  echo -e " passed      : ${GREEN}$PASSED${NC}"
  echo -e " failed      : ${RED}$FAILED${NC}"
  echo -e "${CYAN}────────────────────────────────────────${NC}"
  [[ $FAILED -eq 0 ]]
}

# a user cheatsheet for CLI commands
print_help() {
  echo -e "${YELLOW}╔════════════════════════════════ minishell test suite ═════════════════════════════╗
║                                                                              ║
║  ${CYAN}Basic Usage:${YELLOW}                                                                ║
║    • Enter commands to build a test sequence                                 ║
║    • Press [Enter] with empty input to execute the sequence                  ║
║    • Use ${CYAN}!${YELLOW} to recall last sequence, ${CYAN}!!${YELLOW} to recall and execute it              ║
║                                                                              ║
║  ${CYAN}Quick Actions:${YELLOW}                                                              ║
║    ${CYAN}!${YELLOW}        - Recall last sequence (display only)                            ║
║    ${CYAN}!!${YELLOW}       - Recall and execute last sequence                               ║
║    ${CYAN}!v${YELLOW}       - Recall and execute last sequence with valgrind                 ║
║    ${CYAN}!>${YELLOW}       - Recall and execute last sequence with redirection test         ║
║    ${CYAN}!br${YELLOW}      - Rebuild minishell, then replay                                 ║
║    ${CYAN}-o/--oops${YELLOW} - Remove last input from current sequence                       ║
║                                                                              ║
║  ${CYAN}Session Control:${YELLOW}                                                            ║
║    ${CYAN}bye/quit${YELLOW}      - Exit the test suite and clean up                              ║
║                                                                              ║
║  ${CYAN}Flag Management:${YELLOW}                                                            ║
║    ${CYAN}--valgrind/-vg${YELLOW}      - Toggle valgrind check                               ║
║    ${CYAN}->${YELLOW}                  - Toggle redirection test                             ║
║    ${CYAN}--infile${YELLOW}            - Toggle infile existence                             ║
║    ${CYAN}--infile-perm${YELLOW}       - Toggle infile permissions                           ║
║    ${CYAN}--file1${YELLOW}             - Toggle file1 existence                              ║
║    ${CYAN}--file1-perm${YELLOW}        - Toggle file1 permissions                            ║
║    ${CYAN}--file2${YELLOW}             - Toggle file2 existence                              ║
║    ${CYAN}--file2-perm${YELLOW}        - Toggle file2 permissions                            ║
║    ${CYAN}--outfile${YELLOW}           - Toggle outfile existence                            ║
║    ${CYAN}--outfile-perm${YELLOW}      - Toggle outfile permissions                          ║
║    ${CYAN}-pf/--print=flags${YELLOW}   - Display current flag status                         ║
║                                                                              ║
║  ${CYAN}Print Options:${YELLOW}                                                              ║
║    ${CYAN}--print=stdout${YELLOW}     - Display stdout log                                   ║
║    ${CYAN}--print=stderr${YELLOW}     - Display stderr log                                   ║
║    ${CYAN}--print=valgrind${YELLOW}   - Display valgrind output                              ║
║    ${CYAN}--print=outfile${YELLOW}    - Display outfile contents                             ║
║    ${CYAN}-ps/--print=seq${YELLOW}    - Display current sequence                             ║
║    ${CYAN}-pls/--print=last-seq${YELLOW} - Display last executed sequence                    ║
║                                                                              ║
║  ${CYAN}Advanced Features:${YELLOW}                                                          ║
║    ${CYAN}--suite${YELLOW}       - Run the whole cases.sh suite                          ║
║    ${CYAN}--add-case${YELLOW}    - Add current/last sequence to cases.sh                 ║
║                                                                              ║
║  ${CYAN}Note:${YELLOW} Any other input will be added to the test sequence                    ║
╚══════════════════════════════════════════════════════════════════════════════╝${NC}"
}

# --print=stdout output
print_stdout()
{
  echo -e "$YELLOW"
  echo "Bash STDOUT"
  echo ""
  cat "$LOG_DIR/bash_output"
  echo ""
  echo "------"
  echo ""
  echo "Minishell STDOUT"
  echo ""
  cat "$LOG_DIR/minishell_output"
  echo ""
  echo "Diff :"
  diff "$LOG_DIR/minishell_output" "$LOG_DIR/bash_output"
  echo -e "$NC"
}

# --print=stderr output
print_stderr()
{
  echo -e "$YELLOW"
  echo "Bash STDERR"
  echo ""
  cat "$LOG_DIR/bash_stderr"
  echo ""
  echo "------"
  echo ""
  echo "Minishell STDERR"
  echo ""
  cat "$LOG_DIR/minishell_stderr"
  echo ""
  echo "Diff :"
  diff "$LOG_DIR/minishell_stderr" "$LOG_DIR/bash_stderr"
  echo -e "$NC"
}

# formats flags into arguments readable by compare.sh
set_flags()
{
  FLAGS=()
  [[ $VALGRIND_FLAG == 1 ]] && FLAGS+=("--leaks")
  [[ $R_FLAG == 1 ]] && FLAGS+=("-r")
  [[ $INFILE_FLAG == 1 ]] && FLAGS+=("--infile=off")
  [[ $INFILE_PERM_FLAG == 1 ]] && FLAGS+=("--infile=000")
  [[ $FILE1_FLAG == 1 ]] && FLAGS+=("--file1=off")
  [[ $FILE1_PERM_FLAG == 1 ]] && FLAGS+=("--file1=000")
  [[ $FILE2_FLAG == 1 ]] && FLAGS+=("--file2=off")
  [[ $FILE2_PERM_FLAG == 1 ]] && FLAGS+=("--file2=000")
  [[ $OUTFILE_FLAG == 1 ]] && FLAGS+=("--outfile=off")
  [[ $OUTFILE_PERM_FLAG == 1 ]] && FLAGS+=("--outfile=000")
  return 0
}

# so much flags, it needed a helper to not loose track of your tests
print_flags()
{
  local on_off exists perm
  echo -e "$YELLOW"
  [[ $VALGRIND_FLAG == 1 ]] && on_off="ON" || on_off="OFF"
  echo -e "valgrind           : $on_off"
  [[ $R_FLAG == 1 ]] && on_off="ON" || on_off="OFF"
  echo -e "redirection test   : $on_off"
  echo
  local f
  for f in INFILE FILE1 FILE2 OUTFILE; do
    local exists_var="${f}_FLAG"
    local perm_var="${f}_PERM_FLAG"
    [[ ${!exists_var} == 1 ]] && exists="does not exist" || exists="exists"
    [[ ${!perm_var} == 1 ]] && perm="chmod 000" || perm="chmod 644"
    printf "%-18s : %s\n" "$(tr '[:upper:]' '[:lower:]' <<< "$f")" "$exists"
    printf "%-18s : %s\n" "$(tr '[:upper:]' '[:lower:]' <<< "$f")" "$perm"
    echo
  done
  echo -e "$NC"
}
