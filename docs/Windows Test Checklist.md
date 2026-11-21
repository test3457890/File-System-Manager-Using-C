# Windows owner test checklist

Use this during the November 25, 2026 or later Windows testing session. This is a terminal application, so there are no GUI buttons or menus. Record the date, compiler version, terminal, command, result, and a screenshot of any failure. The checks below have not yet been run on Windows.

- [ ] Clone the repository and compile `src/file_manager.c` with MinGW as shown in the README. Confirm a clean compiler result and launch `file_manager.exe` from PowerShell and Command Prompt.
- [ ] Type `help`, `pwd`, `ls`, `tree`, and `quit`. Compare help text to the README command table.
- [ ] Create nested directories and empty files with relative and absolute paths; navigate with `cd /`, `cd ..`, and `cd folder`; verify `pwd` and the displayed tree.
- [ ] Rename and move an entry; verify the old path fails and the new path works. Try duplicate names, nonexistent destinations, a directory moved into itself or its child, and deleting a nonempty directory.
- [ ] Create a virtual file called `file`. Confirm `cd file/..`, `mkdir file/../unexpected`, `rm file/.`, and `rm file/` all fail without changing the tree. Confirm `cd directory/..` remains valid.
- [ ] Try blank input, a command over 1023 characters, a 63-character name, a 64-character name, extra arguments, and a missing argument. Verify the next valid command still works.
- [ ] Close and relaunch the program. Verify the virtual tree is gone, as documented. Confirm commands never create or delete real host files.
- [ ] If Python 3 is installed, run `python tests/cli.py .\file_manager.exe` from the repository root. Capture the exact output and any failure.
- [ ] Compare the five existing terminal screenshots to actual output. Replace only outdated images with authentic captures that contain no personal paths or identifiers.

Automated Linux checks and this manual Windows session are separate. No boxes above are pre-checked.
