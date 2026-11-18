# Commands and examples

All commands run at the `fs>` prompt. Arguments are separated by whitespace, so filenames cannot contain spaces.

| Command | Example | Description |
| --- | --- | --- |
| `help` | `help` | Show available commands |
| `pwd` | `pwd` | Print current path |
| `ls [path]` | `ls /docs` | Recursively list descendants |
| `tree [path]` | `tree /` | Show directory hierarchy |
| `cd <path>` | `cd ../docs` | Change directory |
| `mkdir <path>` | `mkdir /docs` | Create a virtual directory |
| `touch <path>` | `touch /docs/a.txt` | Create an empty virtual file entry |
| `rm <path>` | `rm /docs/a.txt` | Delete a file or empty directory |
| `rename <path> <name>` | `rename /docs/a.txt b.txt` | Rename a node |
| `mv <path> <directory>` | `mv /docs/b.txt /` | Move a node into a directory |
| `quit` | `quit` | Exit and free memory |

## Example: reorganize a project

```text
mkdir school
mkdir school/notes
touch school/todo.txt
touch school/notes/week1.txt
tree /
rename school/todo.txt tasks.txt
mv school/tasks.txt school/notes
tree /
```

## Path rules

- `/school/notes` starts at the virtual root.
- `school/notes` starts at the current directory.
- `.` means the current directory; `..` moves to the parent, staying at root if already there.
- Names must be 1–63 characters and cannot contain spaces, slashes, or backslashes.
- A move cannot create a cycle or overwrite an existing sibling.
- `rm` refuses to remove a nonempty directory.

**Note:** `ls` and `tree` currently share the same recursive listing behavior. Neither command reads your real computer's files.
