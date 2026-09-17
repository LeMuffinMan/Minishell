*This project has been created as part of the 42 curriculum by oelleaum, asinsard.*

# Minishell — As beautiful as a shell

A Unix shell rebuilt from scratch in C, implementing the core features of Bash: prompt, history, pipes, redirections, environment variable expansion, signal handling, and a full set of builtins.

Bonus features are built in by default: `&&` / `||` logical operators with parentheses grouping, and wildcard `*` expansion.

---

## Description

Minishell is a ground-up reimplementation of a Unix shell. The input goes through a multi-stage pipeline:

```
readline → lexer → tokenizer → parser → AST → executor
```

The parser builds an **Abstract Syntax Tree** that represents the logical structure of the command (pipelines, boolean operators, groupings). The executor walks this tree recursively, handling process forking, pipe chaining, and I/O redirections at each node.

**Mandatory features:**
- Interactive prompt with command history (`readline`)
- `PATH`-based command resolution, relative and absolute paths
- Single quotes (no interpretation) and double quotes (`$` expanded)
- Redirections: `<` `>` `<<` (heredoc) `>>`
- Pipes `|` — multi-command pipelines
- Environment variable expansion (`$VAR`, `$?`)
- Signal handling: `ctrl-C` (new prompt), `ctrl-D` (exit), `ctrl-\` (ignored)
- Builtins: `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Zero memory leaks (excluding readline internals)

**Bonus features:**
- Logical operators `&&` and `||` with correct short-circuit evaluation
- Parentheses `(...)` for priority grouping
- Wildcard `*` expansion in the current working directory

---

## Instructions

### Dependencies

```bash
sudo apt install make              # Debian/Ubuntu
sudo pacman -S make                # Arch
brew install make                  # macOS
```

### Build

```bash
make        # build minishell
make clean  # remove object files
make fclean # remove object files + binary
make re     # fclean + build
```

### Run

```bash
./minishell
```

No arguments accepted. The shell runs interactively only — it refuses a non-tty
stdin, so it cannot be driven by a pipe or a script. The test suite opts out of
that refusal explicitly with `MINISHELL_TEST=1`; nothing else does.

### Test

```bash
make test        # replay 240 input sequences through minishell and bash, and diff
make test-leaks  # same, under valgrind
```

See [tests/README.md](tests/README.md).

---

## Architecture

```
src/
├── parsing/
│   ├── lexer/       — tokenization of raw input
│   ├── token/       — token classification and syntax validation
│   ├── quote/       — quote handling and removal
│   ├── expand/      — environment variable expansion
│   ├── wildcard/    — glob pattern matching
│   ├── here_doc/    — heredoc input collection
│   └── tree/        — AST construction (pipes, boolops, parentheses)
├── exec/            — AST execution, fork/pipe/redirect
├── builtins/        — echo, cd, pwd, export, unset, env, exit
├── env/             — environment variable management
├── signals/         — signal handlers (SIGINT, SIGQUIT)
├── init/            — shell initialization
└── misc/            — error handling, memory management

tests/               — differential test suite (see tests/README.md)
```

---

## Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — authoritative reference for behavior
- [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html) — history and line editing
- [Advanced Programming in the UNIX Environment](https://www.apuebook.com/) — processes, file descriptors, signals
- [Write a Shell in C — Stephen Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c/) — introductory walkthrough

