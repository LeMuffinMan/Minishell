#!/bin/bash

# The differential engine: runs the same input sequence through minishell and
# through bash, then compares stdout, stderr, exit code, the files touched by
# redirections, and optionally the valgrind report.

# shellcheck source-path=SCRIPTDIR
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"

resolve_minishell || exit 1

if [[ -z $1 ]]; then
  echo -e "-----------------------------| the test suite usage |-----------------------------\n"
  echo "No input received"
  echo "usage: ./compare.sh [--leaks] [-r] [--infile=off|000] [...] \"cmd\" [\"cmd\" ...]"
  exit 1
fi

# customize default files variables
# INFILE=1 means it exists / INFILE_PERM=1 means chmod 644
INFILE=1
INFILE_PERM=1
OUTFILE=1
OUTFILE_PERM=1
FILE1=1
FILE1_PERM=1
FILE2=1
FILE2_PERM=1

LEAKS_FLAG=0
REDIR=0

# All these flags are switches to enable or disable some features.
# Unlike the first version they can be given in any order, which matters once
# repl.sh builds the flag list itself.
while [[ $# -gt 0 ]]; do
  case "$1" in
    --leaks)        LEAKS_FLAG=1 ;;
    -r)             REDIR=1 ;;
    --infile=off)   INFILE=0 ;;
    --infile=000)   INFILE_PERM=0 ;;
    --file1=off)    FILE1=0 ;;
    --file1=000)    FILE1_PERM=0 ;;
    --file2=off)    FILE2=0 ;;
    --file2=000)    FILE2_PERM=0 ;;
    --outfile=off)  OUTFILE=0 ;;
    --outfile=000)  OUTFILE_PERM=0 ;;
    *)              break ;;
  esac
  shift
done

enter_sandbox || exit 1

# if your STDOUT appears always KO, read the README and edit this line
prepare_sandbox
PROMPT="$(echo -e "\n" | "$MINISHELL" | awk '{print $1}' | head -1)"

# using \n as a separator to delimitate inputs in get_next_line / read of minishell / bash
INPUT=$(printf "%s\n" "$@")

# using here_doc on minishell and bash to simulate sequences of inputs.
# stdout and stderr are captured in the same run: splitting them into two runs
# would replay the sequence on a directory the first run already modified.
prepare_sandbox
"$MINISHELL" << EOF > "$LOG_DIR/minishell_output" 2> "$LOG_DIR/minishell_stderr"
$INPUT
EOF
# using $? to get last exit code
EXIT_CODE_P=$?

# Since minishell prints its prompt these lines clean it from its output file,
# but first, PROMPT probably includes special characters, sed can make it readable
ESCAPED_PROMPT=$(printf "%s" "$PROMPT" | sed 's/[]\/$*.^[]/\\&/g')

# now we use sed to remove lines beginning with your minishell prompt
# -i edit a file / ^ lines beginning with / /d deletion / .* everything after
sed -i "/^$ESCAPED_PROMPT/d" "$LOG_DIR/minishell_output"
sed -i "s/$ESCAPED_PROMPT.*//" "$LOG_DIR/minishell_output"

# the files touched by redirections, read before the sandbox is reset for bash
MINISHELL_OUTFILE=$(cat log/outfile 2>/dev/null)
MINISHELL_FILE1=$(cat log/file1 2>/dev/null)
MINISHELL_FILE2=$(cat log/file2 2>/dev/null)

# Execute the same test on bash to have a reference, from the same fresh state
prepare_sandbox
bash << EOF > "$LOG_DIR/bash_output" 2> "$LOG_DIR/bash_stderr"
$INPUT
EOF
EXIT_CODE_B=$?

# Driven by a here_doc, bash reports a syntax error over two lines: the
# message, then a copy of the offending source line. Interactive bash prints
# only the message, and minishell is interactive by design, so that second
# line is an artefact of how this script feeds bash and not a difference in
# behaviour. It is dropped before the counts are compared.
sed -i '/^bash: line [0-9]*: `/d' "$LOG_DIR/bash_stderr"

BASH_OUTFILE=$(cat log/outfile 2>/dev/null)
BASH_FILE1=$(cat log/file1 2>/dev/null)
BASH_FILE2=$(cat log/file2 2>/dev/null)

CLEAN=0
LEAKS=0

# some spicy stuff with valgrind
if [[ $LEAKS_FLAG == 1 ]]; then
  prepare_sandbox
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
    --show-mismatched-frees=yes --track-fds=yes --trace-children=yes \
    --suppressions="$SUPPRESSIONS" "$MINISHELL" << EOF > "$LOG_DIR/valgrind_output" 2>&1
$INPUT
EOF

  # --trace-children follows the shell into the programs it execs, so a pipe
  # also reports what /usr/bin/cat does with its own descriptors -- and cat
  # opens a pipe for splice() that it never closes. That belongs to coreutils,
  # not to minishell. A process that has exec'd carries a Command: line naming
  # its binary; a child that forked but has not exec'd yet has none and is
  # still minishell, which is exactly what has to stay under watch.
  FOREIGN=$(awk -v self="$MINISHELL" \
    '/^==[0-9]+== Command: / { pid = $1; gsub(/[^0-9]/, "", pid);
      if ($3 != self) print pid }' "$LOG_DIR/valgrind_output")
  if [[ -n $FOREIGN ]]; then
    grep -Ev "^==($(tr '\n' '|' <<< "$FOREIGN" | sed 's/|$//'))== " \
      "$LOG_DIR/valgrind_output" > "$LOG_DIR/valgrind_own"
  else
    cp "$LOG_DIR/valgrind_output" "$LOG_DIR/valgrind_own"
  fi

  # Basically, if we find this line in the log file, it means there's a segfault
  if grep -q "Process terminating with default action of signal 11 (SIGSEGV)" "$LOG_DIR/valgrind_own"; then
    echo -e "${RED}SEGMENTATION FAULT !${NC}"
    LEAKS=1
  fi

  # if anything is lost or still reachable, one of these lines is non-zero
  if grep -q "definitely lost: 0 bytes in 0 blocks" "$LOG_DIR/valgrind_own" &&
    grep -q "indirectly lost: 0 bytes in 0 blocks" "$LOG_DIR/valgrind_own" &&
    grep -q "possibly lost: 0 bytes in 0 blocks" "$LOG_DIR/valgrind_own" &&
    grep -q "still reachable: 0 bytes in 0 blocks" "$LOG_DIR/valgrind_own"; then
    echo -e "${GREEN}NO LEAKS${NC}"
  else
    LEAKS=1
    echo -e "${RED}LEAKS !${NC}"
  fi

  # File descriptors still open at exit.
  # --track-fds only names the ones valgrind considers user-owned, so a shell
  # that dup2'd its saved stdin/stdout back onto 0 and 1 shows up here even
  # though it did nothing wrong. Only fd 3 and above is a real leak.
  LEAKED_FDS=$(grep -oE "^==[0-9]+== Open file descriptor [0-9]+" "$LOG_DIR/valgrind_own" |
    grep -oE "[0-9]+$" | awk '$1 >= 3' | sort -un | tr '\n' ' ')
  if [[ -z $LEAKED_FDS ]]; then
    echo -e "${GREEN}FD CLOSED${NC}"
  else
    LEAKS=1
    echo -e "${RED}FD OPEN AT EXIT ! (fd: ${LEAKED_FDS% })${NC}"
  fi

  # Conditional jumps or invalid reads land in ERROR SUMMARY. With
  # --trace-children there is one summary per process, so they are summed.
  # Every "Open file descriptor" report is also counted as an error by valgrind,
  # so those are subtracted: they are reported separately just above.
  TOTAL_ERRORS=$(grep -oE "ERROR SUMMARY: [0-9]+ errors" "$LOG_DIR/valgrind_own" |
    grep -oE "[0-9]+" | awk '{s += $1} END {print s + 0}')
  FD_REPORTS=$(grep -cE "^==[0-9]+== Open file descriptor [0-9]+" "$LOG_DIR/valgrind_own")
  NB_ERR=$((TOTAL_ERRORS - FD_REPORTS))
  [[ $NB_ERR -lt 0 ]] && NB_ERR=0
  if [[ $NB_ERR == 0 ]]; then
    echo -e "${GREEN}NO ERRORS${NC}"
  else
    LEAKS=1
    echo -e "${RED}$NB_ERR ERRORS !${NC}"
  fi

  # if any of previous if was activated, invite user to consult the valgrind log
  if [[ $LEAKS == 1 ]]; then
    echo -e "Full valgrind log : \e]8;;file://$LOG_DIR/valgrind_output\avalgrind_output\e]8;;\a"
  fi
fi

# All following lines basically operate diffs and greps on the log files,
# to determine if the test is passed or not
CLEAN=$LEAKS

# print result for STDOUT : a diff on the two output files
if diff -q "$LOG_DIR/minishell_output" "$LOG_DIR/bash_output" > /dev/null; then
  echo -e "STDOUT : ${GREEN}OK${NC}"
else
  echo -e "STDOUT : ${RED}KO${NC}"
  CLEAN=1
  diff "$LOG_DIR/minishell_output" "$LOG_DIR/bash_output"
fi

# Checking stderr is not a plain diff on purpose: bash prefixes its errors with
# "bash: line 1:" where minishell prefixes with its own name, so the two files
# never match byte for byte even when the behaviour is identical. What must
# match is *which* errors were raised, and how many times each.
ERROR_MISSING=0
ERROR_PATTERNS=(
  "command not found"
  "Permission denied"
  "Is a directory"
  "No such file or directory"
  "Not a directory"
  "numeric argument required"
  "exit: too many arguments"
  "syntax error"
  "not a valid identifier"
)

for PATTERN in "${ERROR_PATTERNS[@]}"; do
  # -i ignores case difference. We assume you stick to bash wording;
  # add your own messages to ERROR_PATTERNS if you don't.
  if [[ $(grep -ic "$PATTERN" "$LOG_DIR/bash_stderr") != $(grep -ic "$PATTERN" "$LOG_DIR/minishell_stderr") ]]; then
    ERROR_MISSING=1
  fi
done

# and the total number of error lines must match too, to catch the messages
# that are not in the list above
if [[ $(wc -l < "$LOG_DIR/bash_stderr") -ne $(wc -l < "$LOG_DIR/minishell_stderr") ]]; then
  ERROR_MISSING=1
fi

# printing result for STDERR
if [[ $ERROR_MISSING == 0 ]]; then
  echo -e "STDERR : ${GREEN}OK${NC}"
else
  echo -e "STDERR : ${RED}KO${NC}"
  diff "$LOG_DIR/minishell_stderr" "$LOG_DIR/bash_stderr"
  CLEAN=1
fi

# comparing exit code
if [[ "$EXIT_CODE_P" -ne "$EXIT_CODE_B" ]]; then
  echo -e "EXIT : ${RED}KO${NC}"
  echo -e "bash : $EXIT_CODE_B\nminishell: $EXIT_CODE_P"
  CLEAN=1
else
  echo -e "EXIT : ${GREEN}OK${NC}"
fi

# Option redirection : we compare all files available to test redirections
if [[ $REDIR == 1 ]]; then
  if [[ "$MINISHELL_OUTFILE" != "$BASH_OUTFILE" ]] ||
    [[ "$MINISHELL_FILE1" != "$BASH_FILE1" ]] ||
    [[ "$MINISHELL_FILE2" != "$BASH_FILE2" ]]; then
    echo -e "REDIR > : ${RED}KO${NC}"
    CLEAN=1
  else
    echo -e "REDIR > : ${GREEN}OK${NC}"
  fi
  # leave both versions side by side, so --print=outfile shows what each
  # shell actually wrote into the file
  side_by_side()
  {
    printf "minishell %s :\n\n%s\n" "$1" "$2"
    printf -- "--------------------------------\n"
    printf "bash %s :\n\n%s\n" "$1" "$3"
  }
  side_by_side outfile "$MINISHELL_OUTFILE" "$BASH_OUTFILE" > "$LOG_DIR/outfile"
  side_by_side file1 "$MINISHELL_FILE1" "$BASH_FILE1" > "$LOG_DIR/file1"
  side_by_side file2 "$MINISHELL_FILE2" "$BASH_FILE2" > "$LOG_DIR/file2"
fi

exit $CLEAN
