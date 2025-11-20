//idea - Adding
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Enum for sorting/file type 
typedef enum { 
    TYPE_UNKNOWN, //0
    TYPE_TEXT, //1
    TYPE_IMAGE, //2
    TYPE_EXECUTABLE, //3
    TYPE_DIRECTORY //4 Important: Directories are also entries! //If directories, 
} FileType; 
 
//typedef of "Directory"
typedef struct Directory { 
    Fentry entry;         // what im looking for
    Fentry *children_head; // down 
} Directory; 

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

void delete(Fentry *current, char target[256]){

    Fentry* front=current;
    Fentry second;
    while (1==1){
    	if (current->next == NULL){ //fail
    	  printf("Could not find targeted file to delete\n");
        return;
    	}
    	else if (strcmp(target, current->name) != 0){// not same
    	  current=current->next;
    	  if(second==NULL){
    	  	second=front;
    	  }
    	  second=second->next; //second is one "file" behind current
    	  // loops
    	}
    	else{ //aka matches
    		second->next=current->next;
    		return front;
    	}
    }
    return front;
}

