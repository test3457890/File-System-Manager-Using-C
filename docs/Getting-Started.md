# Getting started

## Prerequisites

Install GCC or Clang with C11 support. Git is optional if you download the repository as a ZIP.

## Build on Linux or macOS

```sh
cc -std=c11 -Wall -Wextra -pedantic -o file_manager src/file_manager.c
./file_manager
```

## Build on Windows (MinGW)

```powershell
gcc -std=c11 -Wall -Wextra -pedantic -o file_manager.exe src/file_manager.c
.\file_manager.exe
```

Run these commands from the repository root.

## First exercise

```text
help
mkdir school
cd school
touch assignment.txt
pwd
tree
rename assignment.txt project.txt
cd ..
tree
quit
```

All entries disappear when the process exits. No real file is created by `touch`.

## Troubleshooting

- **Compiler not found:** install GCC/Clang and verify it is on your PATH.
- **Missing `fs_lite.h`:** you are compiling the older coursework source; compile `src/file_manager.c` instead.
- **Invalid path:** create parent directories first and use `pwd` to check your current location.
- **Duplicate name:** each directory can have only one child with a given name.
- **Cannot delete directory:** remove its children first.
- **Spaces in names:** the command parser does not currently support quoted names.

## Testing

GitHub Actions builds the program with warnings treated as errors and runs scripted positive and negative smoke tests with sanitizers. See the [workflow](../.github/workflows/c-build.yml).
