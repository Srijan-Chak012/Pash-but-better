#include "headers.h"
#include "command.h"
#include "prompt.h"

#define ll long long int

void prompt() {
    uid = geteuid();
    PWD = getpwuid(uid);
    if (PWD == NULL) {
        perror("");
    }
    // strcpy(user_name, PWD->pw_name);
    for (ll i = 0; i < strlen(PWD->pw_name); i++) {
        user_name[i] = PWD->pw_name[i];
    }
    user_name[strlen(PWD->pw_name)] = '\0';
    gethostname(host_name, 256);
    getcwd(current_work_dir, 4000);
    
    if (strcmp(home_dir, current_work_dir) == 0) {
        buff2[0] = '~';
        // strcpy(buff2 + 1, current_work_dir + home_dir_len);
        for (ll i = 1; i < strlen(current_work_dir) + home_dir_len; i++) {
            buff2[i] = current_work_dir[home_dir_len + i - 1];
        }
        buff2[strlen(current_work_dir) + home_dir_len] = '\0';

        // strcpy(current_work_dir, buff2);
        for (ll i = 0; i < strlen(buff2); i++) {
            current_work_dir[i] = buff2[i];
        }
        current_work_dir[strlen(buff2)] = '\0';
    }
    fflush(stdout);
    printf( RED  "$> " RED "PASH" " 🐼" WHITE " " RED "<" "%s@%s"  RED":" "%s" "> " RESET , user_name, host_name, current_work_dir);
    // fflush(stdout);
}
