#ifndef DELETE_H
#define DELETE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Enum for sorting/file type
typedef enum {
    TYPE_UNKNOWN,
    TYPE_TEXT,
    TYPE_IMAGE,
    TYPE_EXECUTABLE,
    TYPE_DIRECTORY // special process to add directory
} FileType;

/*
typedef enum {

} name;
*/

/*
typedef struct name2 {
    Fentry index1; // static index
    Fentry* index2; // pointer index
} name2;
*/



// Structure for a file or directory entry
typedef struct Fentry {
    char name[256];      // Name of the file/folder
    FileType type;       // File type (TEXT, IMAGE, DIRECTORY, etc.)
    time_t creation_time; // Time of creation (for sorting)
    long size;           // Size in bytes (optional, but good practice)
    void *data;          // Pointer to the actual file content (for files)
    struct Directory *parent; // Pointer to the parent directory
    struct Fentry *next;   // Pointer for a linked list within a Directory
} Fentry;

typedef struct Directory { 
    Fentry entry;         // The directory itself is a Fentry, what im looking for
    Fentry *children_head; // Head of a linked list of files/folders inside 
} Directory; 

#endif // DELETE_H