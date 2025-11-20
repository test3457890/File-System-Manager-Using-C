//idea - Adding
#include "add.h"
// Enum for sorting/file type 
/*typedef enum { 
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
*/

void chainNext (Fentry* current, Fentry* add){ //pushnpop less queue
    if (current == NULL || add == NULL){
        printf("Error: current or add is NULL in chainNext\n");
        return;
    } else {
        Fentry* temp = current;
        while (temp->next != NULL) {
            temp = temp->next;
        } //current->next = NULL
    temp->next = add;
    add->next = NULL;
    }
}

int findType(char name[256]){
    printf("Finding type of file...\n"); //debug
    char dot = '.'; // if doesnt work try : const char ch = '.';
    char* end = strchr(name, dot); // comp is whats is after '.' aka the file type
    printf ("After . is |%s|\n", end);  //debug
    if (end==NULL){
    printf("Nothing after dot/no dot at all error. Exiting\n");
    return -1;
  }
  else if (strcmp(end, ".txt")==0 || strcmp(end, ".rtf")==0 || strcmp(end, ".log")==0 || strcmp(end, ".docx")==0){
     printf("text type\n");//debug
     return 1; // .TXT, .RTF, .LOG, and .DOCX.
  }
  else if (strcmp(end, ".jpeg")==0 || strcmp(end, ".jpg")==0 || strcmp(end, ".png")==0 || strcmp(end, ".gif")==0 || strcmp(end, ".tif")==0 || strcmp(end, ".tiff")==0){  
     printf("image type\n");//debug
     return 2;     // .jpeg .jpg .png .gif .tif .tiff
  }
  else if (strcmp(end, ".exe")==0){
     printf("executable type\n");//debug
     return 3; 
  }
  else if (strcmp(end, "lollmaodirectory")==0){ // *.lollmaodirectory //this shit is unneccisarry 
     printf("directory type\n");//debug
     return 4; 
  }
  else{
     printf("Unkown type\n");//debug
     return 0; 
  }
}

Directory addRoot () {
    //og steps: 1 give parent, 2 search name of file, 3 get file type, 4 storing data (idk if file type affects this part), 5 size, 6 date and time of creation, 7 put at end of chain, make next NULL
    // however root should never be talked to, just a starting point. Null is fine
    Directory root;
    root.entry.type = TYPE_DIRECTORY; //3
    root.entry.parent = NULL; //1
    root.entry.data = NULL; //4
    root.entry.size = 0; //5
    root.entry.next = NULL; //7
    root.entry.creation_time = time(NULL); //6
    strcpy(root.entry.name,"ROOT"); //2
    root.children_head = NULL;
    return root;
}
//steps: 1 give parent, 2 give name of file, 3 get file type, 4 storing data (idk if file type affects this part), 5 size,  6 date and time, 7 put at end of chain, make next NULL
void add(int root, Directory *dirParent) {
    if (root == 1) {
        Fentry* victim = malloc(sizeof(Fentry));
        if (victim == NULL) {
            printf("Memory allocation failed for new file entry.\n");
            return;
        }
        victim->parent = dirParent; // 1 Set parent
        printf("What is the name of the file?\n");
        scanf("%s", victim->name); // 2 Input file name
        FILE* fPoint = fopen(victim->name, "r"); // Open file name

        if (fPoint == NULL) { // error check
            perror("Error opening file"); // perror prints a system-specific error message
            exit(0); // Indicate an error
        }


        int typeNum = findType(victim->name); // 3 Determine file type
        if (typeNum==1){
            victim->type= TYPE_TEXT;
        }
        else if (typeNum==2){
            victim->type= TYPE_IMAGE;
        }
        else if (typeNum==3){
            victim->type= TYPE_EXECUTABLE;
        }
        else if (typeNum==4){
            victim->type= TYPE_DIRECTORY; //should never happen
        }
        else{
            victim->type= TYPE_UNKNOWN;
        }
        // 4

        /*
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file); 
        */

        fseek(fPoint, 0, SEEK_END); // 5
        victim->size = ftell(fPoint); // 5
        time(&victim->creation_time); // 6 Set creation time
        printf ("The time listed is %d (debug)\n", victim->creation_time);
        victim->next = NULL; // 7 Set next to NULL

        fclose(fPoint); // Close file from before. File was used to get size and data
        // Check if this is the first file in the directory
        if (dirParent->children_head == NULL) {
            dirParent->children_head = victim;
        } else {
            chainNext(dirParent->children_head, victim);
        }
    } else if (root == 2) {
        Directory* victim2 = malloc(sizeof(Directory));

        if (victim2 == NULL) {
            printf("Memory allocation failed for new directory.\n");
            return;
        }
        
        printf("What is the name of the directory?\n");
        scanf("%s", victim2->entry.name); // 2 Input directory name
        victim2->entry.type = TYPE_DIRECTORY; // 3
        victim2->entry.creation_time = time(NULL); // 6
        victim2->entry.parent = dirParent; // 1
        victim2->entry.next = NULL; // 7

        // Check if this is the first directory in the parent
        if (dirParent->children_head == NULL) {
            dirParent->children_head = &victim2->entry;
        } else {
            chainNext(dirParent->children_head, &victim2->entry);
        }
    } else {
        printf("Invalid operation.\n");
    }
}

//void *data, use fscanf 

/* for size 
#include <stdio.h>
long get_file_size_portable(const char* filename) {
    FILE* file = fopen(filename, "rb"); // Open in binary mode for accurate size
    if (file == NULL) {
        perror("Error opening file");
        return -1; // Indicate error
    }

    fseek(file, 0, SEEK_END); // Move file pointer to the end
    long size = ftell(file); // Get the current position (which is the size)
    fclose(file); // Close the file

    return size;
}
*/
