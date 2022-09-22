#include "headers.h"
#include "cd.h"
#include "pwd.h"

#define ll long long int

char prev_dir[4000], oldpwd[4000];
ll old;

char *cmd = "cd";

void call_CD(char *path)
{
    old = 0;
    path = strtok(NULL, delim);
    if (!path)
    {
        // strcpy(buff2, home_dir);
        for (ll i = 0; i < strlen(home_dir); i++)
        {
            buff2[i] = home_dir[i];
        }
        buff2[strlen(home_dir)] = '\0';
    }
    else
    {
        if (strncmp("-", path, strlen(path)) == 0)
        {
            if (oldpwd[0] == '\0')
            {
                printf("%s: OLDPWD not set\n", cmd);
                return;
            }
            old = 1;
            for (ll i = 0; i < strlen(oldpwd); i++)
            {
                buff2[i] = oldpwd[i];
            }
            buff2[strlen(oldpwd)] = '\0';
        }
        else if (path[0] == '~')
        {
            for (ll i = 0; i < strlen(home_dir); i++)
            {
                buff2[i] = home_dir[i];
            }
            buff2[strlen(home_dir)] = '\0';
            // strcpy(buff2, home_dir);
            strcat(buff2, path + 1);
        }
        else
        {
            // strcpy(buff2, path);
            for (ll i = 0; i < strlen(path); i++)
            {
                buff2[i] = path[i];
            }
            buff2[strlen(path)] = '\0';
        }
        path = strtok(NULL, delim);
    }
    getcwd(prev_dir, 4000);
    if (path)
    {
        printf("%s: too many arguments\n", cmd);
        return;
    }

    if (chdir(buff2) == -1)
        printf("%s: %s: No such file or directory\n", buff2, cmd);
    else
    {
        if (old)
        {
            present_working_directory();
        }
        // strcpy(oldpwd, prev_dir);
        for (ll i = 0; i < strlen(prev_dir); i++)
        {
            oldpwd[i] = prev_dir[i];
        }
        oldpwd[strlen(prev_dir)] = '\0';
    }
    memset(buff2, '\0', strlen(buff2));
}