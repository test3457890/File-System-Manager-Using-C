# Architecture and data structures

## Design goal

Represent a simplified file-system hierarchy using basic C structures rather than operating-system file APIs.

## The node structure

```c
typedef struct Node {
    char name[64];
    int directory;
    struct Node *parent;
    struct Node *children;
    struct Node *next;
} Node;
```

Each directory points to the head of a linked list of children. Each child's `next` pointer links to the next entry in the same directory. The `parent` pointer supports `..` and constructing the current path.

```text
root (/)
  |
  +-- children --> docs/ --next--> readme.txt
                      |
                      +-- children --> notes.txt
```

## Core operations

- **Lookup:** start at the root for an absolute path, otherwise the current directory; traverse each path component by scanning the relevant child list.
- **Create:** validate the final name and parent, allocate a zero-initialized node, and append it to the parent's child list.
- **Move:** ensure the destination is a directory and not a descendant of the source; unlink the source from its old siblings and append it to the destination.
- **Rename:** validate the name and check for a sibling collision before replacing it.
- **Delete:** unlink a file or empty directory, then free its memory. The root cannot be deleted.
- **Shutdown:** recursively free descendants and then their parent nodes.

## Complexity

For a directory with `k` children, a child-name search takes **O(k)** time. A path lookup traverses each component and scans its corresponding directory, giving a cost proportional to the sum of those child-list lengths. Appending a child also takes **O(k)** because the implementation walks to the list's end. Tree display and cleanup visit each node once, **O(n)** for `n` entries.

## Scope and limitations

The virtual file nodes contain no file contents. There is no persistence layer, access-control model, or connection to the host operating system. The current implementation is intentionally small so pointer relationships and memory management remain visible.
