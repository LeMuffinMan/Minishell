`repl.gif` in the test suite README is generated, not captured by hand.

`play.py` spawns `tests/repl.sh` on a pseudo-terminal and types a scripted
session into it. Nothing is staged: the output is the REPL actually running
against the binary at the root of the repository, so the demo cannot drift
away from what the suite really does.

To re-record it after a change:

```bash
make                                  # the GIF shows the binary that is there
asciinema rec --overwrite -c tests/demo/play.py tests/demo/repl.cast
agg tests/demo/repl.cast tests/demo/repl.gif
```

Both `repl.cast` and `repl.gif` are committed: the cast is the source, the GIF
is what the README displays.

Needs `asciinema` and `agg` (`asciinema-agg` on Arch).
