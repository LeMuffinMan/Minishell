#!/usr/bin/env python3
"""Types a scripted session into ./tests/repl.sh, for recording.

The REPL is spawned on a pseudo-terminal, so the tty echoes what we send the
same way it would echo a real keyboard. Nothing here is faked: the output is
the REPL actually running against the binary at the root of the repository.

    asciinema rec --overwrite -c tests/demo/play.py tests/demo/repl.cast
    agg tests/demo/repl.cast tests/demo/repl.gif
"""

import fcntl
import os
import pty
import select
import signal
import struct
import subprocess
import sys
import termios
import time

ROWS, COLS = 22, 96
TYPING_DELAY = 0.05

# (text to type, seconds to wait afterwards)
SCRIPT = [
    ("./tests/repl.sh", 1.2),
    # One line that looks harmless, and is not: bash pipes it, minishell does not
    ("echo a|cat", 0.8),
    # An empty prompt sends the whole sequence to both shells
    ("", 3.5),
    # Same sequence again, with a valgrind pass on top
    ("!v", 7.0),
    ("bye", 1.0),
]


def set_window_size(fd):
    fcntl.ioctl(fd, termios.TIOCSWINSZ, struct.pack("HHHH", ROWS, COLS, 0, 0))


def drain(master, seconds):
    """Copy whatever the child prints to our stdout for that long."""
    deadline = time.time() + seconds
    while True:
        left = deadline - time.time()
        if left <= 0:
            return True
        if not select.select([master], [], [], left)[0]:
            continue
        try:
            chunk = os.read(master, 4096)
        except OSError:
            return False
        if not chunk:
            return False
        sys.stdout.buffer.write(chunk)
        sys.stdout.buffer.flush()


def main():
    master, slave = pty.openpty()
    set_window_size(slave)
    child = subprocess.Popen(
        ["bash", "--norc", "--noprofile"],
        stdin=slave, stdout=slave, stderr=slave,
        preexec_fn=os.setsid,
        env={**os.environ, "PS1": "$ ", "TERM": "xterm-256color"},
    )
    os.close(slave)

    drain(master, 0.5)
    for text, pause in SCRIPT:
        for char in text:
            os.write(master, char.encode())
            drain(master, TYPING_DELAY)
        os.write(master, b"\r")
        if not drain(master, pause):
            break

    drain(master, 0.8)
    os.close(master)
    try:
        child.send_signal(signal.SIGHUP)
        child.wait(timeout=5)
    except Exception:
        child.kill()
    return 0


if __name__ == "__main__":
    sys.exit(main())
