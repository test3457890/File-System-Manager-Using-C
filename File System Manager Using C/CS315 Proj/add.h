#ifndef ADD_H
#define ADD_H
#include "fs_lite.h"

void chainNext (Fentry* current, Fentry* add);
int findType(char name[256]);
Directory addRoot ();
void add(int root, Directory *dirParent);
extern Directory* current_dir;

#endif // ADD_H