#ifndef __LIST_DIR_H_
#define __LIST_DIR_H_

struct element {
    char Name[300], Path[4000];
};

// long long int get_total(char *);
// void flag(char *, char *, struct stat *);
// void ls_dir(char *);
// void ls (char *, int num);

void ls(char ** s, int num);

#endif
