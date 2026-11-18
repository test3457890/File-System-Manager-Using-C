# File System Manager Using C

An educational in-memory file system implemented with a tree of directories and linked lists of children.

## Improved version

The working C11 implementation is in `src/file_manager.c`. The original CS315 project files are preserved for reference.

### Build

Requires GCC or Clang:

```sh
cc -std=c11 -Wall -Wextra -pedantic -o file_manager src/file_manager.c
./file_manager
```

On Windows with MinGW:

```powershell
gcc -std=c11 -Wall -Wextra -pedantic -o file_manager.exe src/file_manager.c
.\file_manager.exe
```

### Commands

- `help`: show available commands
- `pwd`: show the current virtual directory
- `ls [path]`: list a directory and its descendants
- `tree [path]`: display a directory tree
- `cd <path>`: change directory
- `mkdir <path>`: create a directory
- `touch <path>`: create an empty virtual file
- `rm <path>`: remove a file or empty directory
- `rename <path> <new-name>`: rename an entry
- `mv <path> <directory>`: move an entry into another directory
- `quit`: exit and release allocated memory

Example:

```text
mkdir docs
cd docs
touch notes.txt
pwd
ls
cd ..
mv docs /
tree
quit
```

Paths support `/`, `.`, and `..`. Names are limited to 63 characters and cannot contain spaces. This program operates only on a virtual in-memory tree; it does not create, modify, or delete real files.

**Limitations:** Virtual files have names but not contents. Data is not saved when the program exits. Removing a nonempty directory is intentionally blocked. The `ls` command currently shows a recursive listing, similar to `tree`.

### Original project

The original code is under `File System Manager Using C/CS315 Proj`. Some original files reference `fs_lite.h`, which is not present in the repository; therefore the original version cannot be built as committed. The new version is standalone and does not depend on that missing header.
