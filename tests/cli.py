"""Regression checks for the in-memory file manager's command interface."""

import os
import subprocess
import sys


def session(*commands):
    result = subprocess.run(
        [sys.argv[1] if len(sys.argv) > 1 else "./file_manager"],
        input="\n".join(commands) + "\n",
        text=True,
        capture_output=True,
        env=os.environ.copy(),
        check=True,
    )
    assert not result.stderr, result.stderr
    replies = result.stdout.split("fs> ")
    assert replies[0] == "In-memory File Manager (type help for commands)\n"
    assert len(replies) == len(commands) + 1, replies
    return [reply.rstrip("\n") for reply in replies[1:]]


def check(commands, expected):
    actual = session(*commands)
    assert actual == expected, "\n".join(
        f"{command!r}: expected {want!r}, got {got!r}"
        for command, want, got in zip(commands, expected, actual)
        if want != got
    )


check(
    ["mkdir docs", "mkdir docs", "mkdir docs/notes", "touch docs/notes/task.txt",
     "rm docs", "mv docs docs/notes", "cd docs/notes", "pwd",
     "rename task.txt final.txt", "cd /", "mkdir archive",
     "mv docs/notes/final.txt archive", "tree /", "rm docs/notes",
     "rm docs", "tree /", "quit"],
    ["Directory created", "Error: invalid path or duplicate name", "Directory created",
     "File created", "Error: not found, root, or nonempty directory",
     "Error: invalid destination or cycle", "", "/docs/notes", "Renamed", "",
     "Directory created", "Moved", "docs/\n  notes/\narchive/\n  final.txt",
     "Removed", "Removed", "archive/\n  final.txt", ""],
)

name = "n" * 63
check(
    ["mkdir " + name, "mkdir " + name + "n", "touch " + name,
     "touch /", "cd " + name, "pwd", "cd /missing", "pwd",
     "x" * 1024, "help", "quit"],
    ["Directory created", "Error: invalid path or duplicate name",
     "Error: invalid path or duplicate name", "Error: invalid path or duplicate name",
     "", "/" + name, "Error: directory not found", "/" + name,
     "Error: command too long",
     "Commands: help, pwd, ls [path], tree [path], cd <path>,\n"
     "  mkdir <path>, touch <path>, rm <path>, rename <path> <name>,\n"
     "  mv <path> <directory>, quit\n"
     "Paths may be absolute (/docs/file) or relative (../file).\n"
     "Names cannot contain spaces; rm only removes empty directories.", ""],
)

check(
    ["touch file", "mkdir file/../surprise", "cd file/..", "rm file/.",
     "rm file/", "mkdir docs", "touch docs/note", "cd docs/../", "pwd",
     "tree /", "quit"],
    ["File created", "Error: invalid path or duplicate name",
     "Error: directory not found", "Error: not found, root, or nonempty directory",
     "Error: not found, root, or nonempty directory", "Directory created",
     "File created", "", "/", "file\ndocs/\n  note", ""],
)

print("PASS: CLI paths, moves, rejection cases, file traversal, name limits, and long-command recovery")
