//idea - Adding
//Big O notation of O(n)
// One loops, no faster method, data goes in, file entry goes out. Does search for the end of the linked list to place new entry at the.
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
*/ // comment end

void chainNext (Fentry* current, Fentry* add){ // moves a new file entry to end of linked list
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

int findType(char name[256]){ // uses the suffix of the file to find file type. Matches with built in def
    printf("Finding type of file...\n"); //debug
    char dot = '.'; // if doesnt work try : const char ch = '.';
    char* end = strchr(name, dot); // comp is whats is after '.' aka the file type
    printf ("After . is |%s|\n", end);  //debug
    if (end==NULL){
    printf("Nothing after dot/no dot at all error. Exiting\n");
    return -1;
  }
  //a dictionary would honestly be faster, but im too lazy
  else if (strcmp(end, ".txt")==0 || strcmp(end, ".rtf")==0 || strcmp(end, ".log")==0 || strcmp(end, ".docx")==0 || strcmp(end, ".c")==0 || strcmp(end, ".py")==0 || strcmp(end, ".cpp")==0 || strcmp(end, ".cxx")==0 || strcmp(end, ".java")==0 || strcmp(end, ".js")==0 || strcmp(end, ".ts")==0 || strcmp(end, ".rb")==0 || strcmp(end, ".php")==0 || strcmp(end, ".go")==0 || strcmp(end, ".rs")==0 || strcmp(end, ".swift")==0 || strcmp(end, ".pl")==0 || strcmp(end, ".r")==0 || strcmp(end, ".lua")==0){
     printf("text type\n");//debug
     return 1; // .TXT, .RTF, .LOG, .DOCX, .c, .py, .cpp, .cxx, .java, .js, .ts, .rb, .php, .go, .rs, .swift, .pl, .r, .lua
  }
  else if (strcmp(end, ".jpeg")==0 || strcmp(end, ".jpg")==0 || strcmp(end, ".png")==0 || strcmp(end, ".gif")==0 || strcmp(end, ".tif")==0 || strcmp(end, ".tiff")==0){  
     printf("image type\n");//debug
     return 2;     // .jpeg .jpg .png .gif .tif .tiff
  }
  else if (strcmp(end, ".exe")==0 || strcmp(end, ".sh")==0 || strcmp(end, ".bat")==0 || strcmp(end, ".ps1")==0 || strcmp(end, ".out")==0 || strcmp(end, ".bin")==0){
     printf("executable type\n");//debug
     return 3; // .exe, .sh, .bat, .ps1, .out, .bin
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

Directory addRoot () { // used to specifically add the very first directory
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

void add(int root, Directory *dirParent) { // add new directory or file. Root=1 is new file, and root=2 is direcotry
    if (dirParent == NULL) {
        fprintf(stderr, "add: dirParent is NULL\n");
        return;
    }

    if (root == 1) { // adding a new file
        Fentry *victim = malloc(sizeof *victim); // 0 give form
        if (!victim) { // malloc error check
            printf(" Malloc failed \n");
            perror("malloc");
            return;
        }
        victim->parent = dirParent; // 1 parent


        printf("Enter filename: ");
        if (scanf("%255s", victim->name) != 1) { // 2 input name
            free(victim);
            return;
        }

        FILE *fPoint = fopen(victim->name, "rb"); // open file (search for file)
        if (fPoint == NULL) {
            perror("fopen");
            free(victim);
            return;
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
        
        victim->data = NULL; // 4 yeah

        if (fseek(fPoint, 0, SEEK_END) != 0) { 
            perror("fseek");
            fclose(fPoint);
            free(victim);
            return;
        }
        long size = ftell(fPoint);
        if (size < 0) {
            perror("ftell");
            fclose(fPoint);
            free(victim);
            return;
        }
        victim->size = size; // 5 file size
        fclose(fPoint);

        victim->creation_time = time(NULL); // 6 time
        victim->next = NULL; // 7 next is null;

        if (dirParent->children_head == NULL) {
            dirParent->children_head = victim; // 7 start of linked list
        } else {
            chainNext(dirParent->children_head, victim); // 7 next in linked list
        }

        return;
    }

    if (root == 2) { // add a new directory
        Directory *victim2 = malloc(sizeof *victim2); // 0 void given form
        if (!victim2) { perror("malloc"); return; }
        memset(victim2, 0, sizeof *victim2);

        printf("Enter directory name: ");
        if (scanf("%255s", victim2->entry.name) != 1) { // 2 name directory (no searching)
            free(victim2); 
            return; 
        }

        victim2->entry.type = TYPE_DIRECTORY; // 3 type is direcotry
        victim2->entry.creation_time = time(NULL); // 6 also time
        victim2->entry.parent = dirParent; // 1 parent
        victim2->entry.next = NULL; // 7 next is null
        victim2->children_head = NULL; // start of its own linked list

        if (dirParent->children_head == NULL) {
            dirParent->children_head = &victim2->entry; // 7 start of link list
        } else {
            chainNext(dirParent->children_head, &victim2->entry); // 7 placed at en do linked list
        }

        return;
    }

    fprintf(stderr, "add: unknown root value %d\n", root);
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
