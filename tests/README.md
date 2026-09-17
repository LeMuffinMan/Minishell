# Differential test suite

This suite replays the same input sequence through **minishell** and through
**bash**, then compares what comes out of each: standard output, standard
error, exit code, the files touched by redirections, and — on demand — the
valgrind report.

Nothing is asserted against a hand-written expectation. bash *is* the
expectation.

```bash
make test          # run the whole suite
make test-leaks    # same, under valgrind (much slower)
make test-clean    # remove the sandbox
```

![the REPL catching a parsing bug](demo/repl.gif)

Above: `echo a|cat` typed into the REPL. bash pipes it; minishell goes looking
for a command named `a|cat`. The second pass adds valgrind.

---

## What a comparison covers

| Check | How |
|---|---|
| `STDOUT` | plain `diff` between the two outputs, minishell's prompt stripped first |
| `STDERR` | per-message counts, not a `diff` — see below |
| `EXIT` | exit code of the here_doc session |
| `REDIR >` | contents of `log/outfile`, `log/file1`, `log/file2` after the run (`-r`) |
| leaks, errors, fds | valgrind, with `--track-fds` and `--trace-children` |

**Why stderr is not a diff.** bash prefixes its errors with `bash: line 1:`
where minishell uses its own name, so the two files never match byte for byte
even when the behaviour is identical. What must match is *which* errors were
raised and how many times each, so the engine counts occurrences of the known
messages (`command not found`, `Permission denied`, `syntax error`, …) plus the
total number of error lines. Add your own wording to `ERROR_PATTERNS` in
`compare.sh` if your shell phrases things differently.

**Why other programs are not audited.** `--trace-children` follows the shell
into everything it execs, so a pipe also reports what `/usr/bin/cat` does with
its own descriptors — and cat opens a pipe for `splice()` that it never closes.
A process that has exec'd carries a `Command:` line naming its binary and is
left out of the counts; a child that forked but has not exec'd yet has no
`Command:` line of its own, is still minishell, and stays under watch. That is
where a shell actually leaks.

**Why fd 0, 1 and 2 are ignored.** `--track-fds` reports every descriptor
valgrind considers user-owned. A shell that saved its standard streams and
`dup2`'d them back at cleanup shows up there without having done anything
wrong, and valgrind counts each report in `ERROR SUMMARY`. The engine
subtracts them, and flags only fd 3 and above as a real leak.

---

## Running it

The suite drives minishell through a here_doc, so stdin is not a terminal.
minishell refuses that on purpose (`src/misc/misc.c`), which is why it also
honours `MINISHELL_TEST=1`: the refusal stays the default, the test suite opts
out of it explicitly. `lib.sh` exports the variable, so there is nothing to do
by hand.

To point the suite at another build:

```bash
MINISHELL=/path/to/other/minishell ./tests/cases.sh
```

Each run gets its own directory under `tests/.sandbox`, named after its pid,
so a suite and a REPL running side by side never overwrite each other. Inside
it, the shells run in `run/`, which is wiped and rebuilt **before minishell
and again before bash**: the cases create and delete files, and replaying the
second shell on whatever the first one left behind would compare two different
situations. Reports land in `results/`, outside the directory that gets wiped.
A run drops the sandboxes of runs whose process is gone, so the last one stays
readable until the next one starts.

---

## The REPL

```bash
./tests/repl.sh
```

Type command lines to build a sequence, press Enter on an empty prompt to run
it. `-h` prints the full command list; the useful ones are `!!` (replay),
`!v` (replay under valgrind), `!>` (replay with the redirection check), `-o`
(drop the last line) and `--add-case` (store the sequence in `cases.sh`).

```
[minishell-test]$ pwd
[minishell-test]$ cd ..
[minishell-test]$ pwd
[minishell-test]$
STDOUT : OK
STDERR : OK
EXIT : OK
[minishell-test]$ !v
NO LEAKS
FD CLOSED
NO ERRORS
STDOUT : OK
STDERR : OK
EXIT : OK
```

---

## Adding cases

One line in `cases.sh` is one sequence. Each argument is one input line.

```bash
check "pwd" "cd .." "pwd"
check "-r" "echo -e '180g milk' > log/outfile"
```

Flags come first: `-r` for the redirection check, `--infile=off`,
`--outfile=000` and friends to run against a missing or unreadable file.
Valgrind is not written into the cases — it is a switch on the whole run
(`TEST_LEAKS=1`).

---

## What this cannot test

The engine feeds minishell through a pipe, so everything that needs a terminal
is out of reach and has to be checked by hand:

- signals — `ctrl-C` at the prompt, `ctrl-\`, `ctrl-C` inside a here_doc
- `ctrl-D` on a non-empty line
- readline itself: history, arrow keys, line editing
- anything about the prompt being redrawn

Making these automatable would mean allocating a pty instead of a pipe.
