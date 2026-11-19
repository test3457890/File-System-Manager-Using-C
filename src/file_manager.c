/* In-memory file manager: C11, no third-party dependencies.
 * This is a separate, working implementation; the original CS315 files are preserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_MAX_LEN 63
#define LINE_MAX_LEN 1024

typedef struct Node {
    char name[NAME_MAX_LEN + 1];
    int directory;
    struct Node *parent, *children, *next;
} Node;

static Node *root, *cwd;

static Node *new_node(const char *name, int directory, Node *parent) {
    Node *n = calloc(1, sizeof *n);
    if (!n) return NULL;
    snprintf(n->name, sizeof n->name, "%s", name);
    n->directory = directory;
    n->parent = parent;
    return n;
}
static void free_tree(Node *n) {
    while (n) {
        Node *next = n->next;
        free_tree(n->children);
        free(n);
        n = next;
    }
}
static Node *child(Node *dir, const char *name) {
    for (Node *n = dir->children; n; n = n->next)
        if (strcmp(n->name, name) == 0) return n;
    return NULL;
}
static int valid_name(const char *s) {
    size_t len = strlen(s);
    if (!len || len > NAME_MAX_LEN || !strcmp(s, ".") || !strcmp(s, "..")) return 0;
    for (; *s; s++) if (*s == '/' || *s == '\\' || (unsigned char)*s < 32) return 0;
    return 1;
}
static Node *lookup(const char *path) {
    if (!path || !*path) return NULL;
    Node *at = (*path == '/') ? root : cwd;
    char *copy = malloc(strlen(path) + 1);
    if (!copy) return NULL;
    strcpy(copy, path);
    for (char *p = strtok(copy, "/"); p; p = strtok(NULL, "/")) {
        if (!at->directory) { at = NULL; break; }
        if (!strcmp(p, ".")) continue;
        if (!strcmp(p, "..")) { if (at->parent) at = at->parent; continue; }
        at = child(at, p);
        if (!at) break;
    }
    if (at && !at->directory && path[strlen(path) - 1] == '/') at = NULL;
    free(copy);
    return at;
}
static int split_parent(const char *path, Node **parent, char name[NAME_MAX_LEN + 1]) {
    if (!path || !*path) return 0;
    const char *slash = strrchr(path, '/');
    const char *base = slash ? slash + 1 : path;
    if (!valid_name(base)) return 0;
    strcpy(name, base);
    if (!slash) *parent = cwd;
    else if (slash == path) *parent = root;
    else {
        size_t len = (size_t)(slash - path);
        char *prefix = malloc(len + 1);
        if (!prefix) return 0;
        memcpy(prefix, path, len);
        prefix[len] = 0;
        *parent = lookup(prefix);
        free(prefix);
    }
    return *parent && (*parent)->directory;
}
static int add_entry(const char *path, int directory) {
    Node *parent = NULL;
    char name[NAME_MAX_LEN + 1];
    if (!split_parent(path, &parent, name) || child(parent, name)) return 0;
    Node *n = new_node(name, directory, parent);
    if (!n) return 0;
    Node **tail = &parent->children;
    while (*tail) tail = &(*tail)->next;
    *tail = n;
    return 1;
}
static int remove_entry(const char *path) {
    Node *n = lookup(path);
    if (!n || n == root || (n->directory && n->children)) return 0;
    Node **link = &n->parent->children;
    while (*link && *link != n) link = &(*link)->next;
    if (!*link) return 0;
    *link = n->next;
    n->next = NULL;
    if (cwd == n) cwd = n->parent;
    free_tree(n);
    return 1;
}
static int rename_entry(const char *path, const char *name) {
    Node *n = lookup(path);
    if (!n || n == root || !valid_name(name)) return 0;
    Node *existing = child(n->parent, name);
    if (existing && existing != n) return 0;
    strcpy(n->name, name);
    return 1;
}
static int move_entry(const char *path, const char *destination) {
    Node *n = lookup(path), *dest = lookup(destination);
    if (!n || n == root || !dest || !dest->directory || child(dest, n->name)) return 0;
    for (Node *p = dest; p; p = p->parent) if (p == n) return 0;
    Node **link = &n->parent->children;
    while (*link && *link != n) link = &(*link)->next;
    if (!*link) return 0;
    *link = n->next;
    n->parent = dest;
    n->next = NULL;
    Node **tail = &dest->children;
    while (*tail) tail = &(*tail)->next;
    *tail = n;
    return 1;
}
static void print_path(const Node *n) {
    if (n == root) { putchar('/'); return; }
    print_path(n->parent);
    if (n->parent != root) putchar('/');
    fputs(n->name, stdout);
}
static void tree(const Node *n, int depth) {
    for (const Node *p = n->children; p; p = p->next) {
        for (int i = 0; i < depth; i++) fputs("  ", stdout);
        printf("%s%s\n", p->name, p->directory ? "/" : "");
        if (p->directory) tree(p, depth + 1);
    }
}
static void help(void) {
    puts("Commands: help, pwd, ls [path], tree [path], cd <path>,");
    puts("  mkdir <path>, touch <path>, rm <path>, rename <path> <name>,");
    puts("  mv <path> <directory>, quit");
    puts("Paths may be absolute (/docs/file) or relative (../file).");
    puts("Names cannot contain spaces; rm only removes empty directories.");
}
int main(void) {
    root = new_node("", 1, NULL);
    if (!root) { fputs("Out of memory\n", stderr); return 1; }
    cwd = root;
    puts("In-memory File Manager (type help for commands)");
    char line[LINE_MAX_LEN];
    for (;;) {
        fputs("fs> ", stdout);
        fflush(stdout);
        if (!fgets(line, sizeof line, stdin)) break;
        if (!strchr(line, '\n') && !feof(stdin)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            puts("Error: command too long");
            continue;
        }
        char *cmd = strtok(line, " \t\r\n");
        if (!cmd) continue;
        char *a = strtok(NULL, " \t\r\n");
        char *b = strtok(NULL, " \t\r\n");
        char *extra = strtok(NULL, " \t\r\n");
        if (!strcmp(cmd, "quit") || !strcmp(cmd, "exit")) break;
        if (extra) { puts("Error: too many arguments"); continue; }
        if (!strcmp(cmd, "help") && !a) help();
        else if (!strcmp(cmd, "pwd") && !a) { print_path(cwd); putchar('\n'); }
        else if ((!strcmp(cmd, "ls") || !strcmp(cmd, "tree")) && !b) {
            Node *n = a ? lookup(a) : cwd;
            if (!n || !n->directory) puts("Error: directory not found");
            else tree(n, 0);
        } else if (!strcmp(cmd, "cd") && a && !b) {
            Node *n = lookup(a);
            if (!n || !n->directory) puts("Error: directory not found");
            else cwd = n;
        } else if (!strcmp(cmd, "mkdir") && a && !b) {
            puts(add_entry(a, 1) ? "Directory created" : "Error: invalid path or duplicate name");
        } else if (!strcmp(cmd, "touch") && a && !b) {
            puts(add_entry(a, 0) ? "File created" : "Error: invalid path or duplicate name");
        } else if (!strcmp(cmd, "rm") && a && !b) {
            puts(remove_entry(a) ? "Removed" : "Error: not found, root, or nonempty directory");
        } else if (!strcmp(cmd, "rename") && a && b) {
            puts(rename_entry(a, b) ? "Renamed" : "Error: invalid name, path, or duplicate");
        } else if (!strcmp(cmd, "mv") && a && b) {
            puts(move_entry(a, b) ? "Moved" : "Error: invalid destination or cycle");
        } else puts("Unknown command or arguments; type help");
    }
    free_tree(root);
    return 0;
}
