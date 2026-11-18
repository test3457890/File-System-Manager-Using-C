# Roadmap

This page distinguishes **implemented features** from ideas for future development.

## Implemented

- [x] In-memory directory tree with linked-list children
- [x] Absolute and relative path lookup
- [x] Create virtual files and directories
- [x] Navigate and display the hierarchy
- [x] Rename and move entries
- [x] Prevent directory cycles and duplicate sibling names
- [x] Delete files and empty directories
- [x] Release allocated nodes at shutdown
- [x] Automated compilation and smoke tests

## Proposed improvements

- [ ] Save and reload the virtual tree from a portable file format
- [ ] Store and edit virtual file contents
- [ ] Support filenames with spaces and quoted CLI arguments
- [ ] Make `ls` list immediate children and `tree` list recursively
- [ ] Add search by name and optional file metadata
- [ ] Expand unit tests for path and memory edge cases
- [ ] Add a portable Makefile or CMake build
- [ ] Optionally explore a graphical interface

## Design priorities

Keep the code readable for C learners, retain the tree/linked-list educational focus, and avoid introducing real-file operations without clearly documenting their effects. Proposed features are not guaranteed or scheduled.
