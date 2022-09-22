#include "headers.h"
#include "discover.h"

#define ll long long int

char *my_cat(char *destination, const char *source)
{
    // make `ptr` point to the end of the destination string
    char *ptr = destination + strlen(destination);

    // appends characters of the source to the destination string
    while (*source != '\0')
    {
        *ptr++ = *source++;
    }

    // null terminate destination string
    *ptr = '\0';

    // the destination is returned by standard `my_cat()`
    return destination;
}

void traverse_dir(char *dir, int d_flag, int f_flag)
{
    struct dirent *d;
    DIR *dh = opendir(dir);
    struct stat filetype;
    for (; (d = readdir(dh));)
    // while ((d = readdir(dh)) != NULL)
    {
        stat(d->d_name, &filetype);
        char cwd[4000];
        char path[4000];
        if (S_ISREG(filetype.st_mode) && f_flag)
        {
            char *temp;
            // strcpy(path, my_cat(my_cat(getcwd(cwd, 4000), "/"), d->d_name));
            temp = my_cat(my_cat(getcwd(cwd, 4000), "/"), d->d_name);
            // strcpy(path, temp);
            for (ll i = 0; i < strlen(temp); i++)
            {
                path[i] = temp[i];
            }
            path[strlen(temp)] = '\0';
            printf(".%s", &path[strlen(current_work_dir)]);
            printf("\n");
        }
        else if (S_ISREG(filetype.st_mode) == 0 && d_flag && d->d_name[0] != '.')
        {
            // strcpy(path, my_cat(my_cat(getcwd(cwd, 4000), "/"), d->d_name));
            char *temp3;
            temp3 = my_cat(my_cat(getcwd(cwd, 4000), "/"), d->d_name);
            // strcpy(path, temp3);
            for (ll i = 0; i < strlen(temp3); i++)
            {
                path[i] = temp3[i];
            }
            path[strlen(temp3)] = '\0';
            printf(".%s", &path[strlen(current_work_dir)]);
            printf("\n");
            // strcpy(cwd, dir);
            for (ll i = 0; i < strlen(dir); i++)
            {
                cwd[i] = dir[i];
            }
            cwd[strlen(dir)] = '\0';
            char temp[4000];
            // strcpy(temp, my_cat(my_cat(cwd, "/"), d->d_name));
            char *temp2;
            temp2 = my_cat(my_cat(cwd, "/"), d->d_name);
            for (ll i = 0; i < strlen(temp2); i++)
            {
                temp[i] = temp2[i];
            }
            temp[strlen(temp2)] = '\0';
            // printf("-----%s\n",temp);

            if (chdir(temp))
            {
                perror("discover: ");
                return;
            }
            traverse_dir(temp, d_flag, f_flag);
            chdir("..");
        }
    }
}

ll traverse_file(char *dir, char *target, int d_flag, int f_flag)
{
    struct dirent *d;
    DIR *dh = opendir(dir);
    bool flag = false;
    struct stat filetype;
    // while ((d = readdir(dh)) != NULL)
    for (; (d = readdir(dh));)
    {
        stat(d->d_name, &filetype);
        char cwd[4000];
        char path[4000];
        if (S_ISREG(filetype.st_mode) && f_flag)
        {
            // printf("%s ------ %s\n",d->d_name,target);
            if (strncmp(d->d_name, target, strlen(target)) == 0)
            {
                // strcpy(path, my_cat(my_cat(getcwd(cwd, 4000), "/"), d->d_name));
                char *temp;
                temp = my_cat(my_cat(getcwd(cwd, 4000), "/"), d->d_name);
                // strcpy(path, temp);
                for (ll i = 0; i < strlen(temp); i++)
                {
                    path[i] = temp[i];
                }
                path[strlen(temp)] = '\0';
                printf(".%s", &path[strlen(current_work_dir)]);
                printf("\n");
                flag = true;
            }
        }
        else if (!S_ISREG(filetype.st_mode) && d_flag && d->d_name[0] != '.')
        {
            char cwd[4000];
            // strcpy(cwd, dir);
            for (ll i = 0; i < strlen(dir); i++)
            {
                cwd[i] = dir[i];
            }
            cwd[strlen(dir)] = '\0';
            char temp[4000];
            // strcpy(temp, my_cat(my_cat(cwd, "/"), d->d_name));
            char *temp2;
            temp2 = my_cat(my_cat(cwd, "/"), d->d_name);
            for (ll i = 0; i < strlen(temp2); i++)
            {
                temp[i] = temp2[i];
            }
            temp[strlen(temp2)] = '\0';
            if (strncmp(d->d_name, target, strlen(target)) == 0)
            {
                printf(".%s", &temp[strlen(current_work_dir)]);
                printf("\n");
                flag = true;
            }
            if (chdir(temp))
            {
                perror("discover: ");
                return 0;
            }
            ll x = traverse_file(temp, target, d_flag, f_flag);
            chdir("..");
            flag = flag || x;
        }
    }
    if(!flag)
        return 0;
    else
        return 1;
}

void discover(char **commands, int cmd_num)
{
    if (cmd_num > 0 && cmd_num < 2)
    {
        printf(".\n");
        char cwd[4000];
        getcwd(cwd, 4000);
        traverse_dir(cwd, 1, 1);
        return;
    }
    else
    {
        ll d_flag = 0;
        ll f_flag = 0;
        ll target_dir_flag = 0;
        ll file_to_find_flag = 0;
        char target_dir[4000];
        char file_to_find[4000];
        for (ll i = 1; i < cmd_num; i++)
        {
            if (strncmp(commands[i], "-d", strlen(commands[i])) == 0)
                d_flag = 1;
            else if (strncmp(commands[i], "-f", strlen(commands[i])) == 0)
                f_flag = 1;
            else if (commands[i][0] == '\"')
            {
                file_to_find_flag = 1;
                // strcpy(file_to_find, &commands[i][1]);
                for (ll j = 1; j < strlen(commands[i]); j++)
                {
                    file_to_find[j - 1] = commands[i][j];
                }
                file_to_find[strlen(file_to_find) - 1] = '\0';
            }
            else
            {
                target_dir_flag = 1;
                // strcpy(target_dir, commands[i]);
                for (ll j = 0; j < strlen(commands[i]); j++)
                {
                    target_dir[j] = commands[i][j];
                }
                target_dir[strlen(target_dir)] = '\0';
            }
        }
        if (!d_flag && !f_flag)
        {
            d_flag = 1;
            f_flag = 1;
        }
        if (!file_to_find_flag)
        {
            if (!target_dir_flag)
            {
                printf(".\n");
                char cwd[3000];
                getcwd(cwd, 3000);
                traverse_dir(cwd, d_flag, f_flag);
            }
            else
            {
                if (target_dir[0] == '.' && target_dir[1] == '.')
                {
                    chdir("..");
                    char parent[4000];
                    getcwd(parent, 4000);
                    printf("%s\n", parent);

                    // my_cat(parent, &target_dir[2]);
                    // printf("%s\n", parent);
                    if (chdir(parent) != 0)
                    {
                        perror("Error: ");
                        return;
                    }
                    if (d_flag)
                        printf("%s\n", target_dir);
                    traverse_dir(parent, d_flag, f_flag);
                    chdir(current_work_dir);
                }
                else if (target_dir[0] == '.')
                {
                    char temp[4000];
                    if (strncmp(current_work_dir, "~", strlen(current_work_dir)) == 0)
                    // strcpy(temp, home_dir);
                    {
                        for (ll i = 0; i < strlen(home_dir); i++)
                        {
                            temp[i] = home_dir[i];
                        }
                        temp[strlen(home_dir)] = '\0';
                    }
                    // else
                    //     strcpy(temp, current_work_dir);
                    else
                    {
                        // strcpy(temp, current_work_dir);
                        for (ll i = 0; i < strlen(current_work_dir); i++)
                        {
                            temp[i] = current_work_dir[i];
                        }
                        temp[strlen(current_work_dir)] = '\0';
                    }
                    // printf("%s\n",temp);
                    // my_cat(temp, &target_dir[1]);
                    // printf("%s\n",temp);
                    if (chdir(temp))
                    {
                        perror("Error: ");
                        return;
                    }
                    if (d_flag)
                        printf("%s\n", target_dir);
                    traverse_dir(temp, d_flag, f_flag);
                    chdir(current_work_dir);
                }
                else if (target_dir[0] == '~')
                {
                    char temp[4000];
                    // strcpy(temp, home_dir);
                    for (ll i = 0; i < strlen(home_dir); i++)
                    {
                        temp[i] = home_dir[i];
                    }
                    temp[strlen(home_dir)] = '\0';
                    // printf("%s\n",temp);
                    // my_cat(temp, &target_dir[1]);
                    // printf("%s\n",temp);
                    if (chdir(temp))
                    {
                        perror("Error: ");
                        return;
                    }
                    if (d_flag)
                        printf("%s\n", target_dir);
                    traverse_dir(temp, d_flag, f_flag);
                    chdir(current_work_dir);
                }
                else
                {
                    if (target_dir[0] == '/')
                    {
                        if (!chdir(target_dir))
                        {
                            traverse_dir(target_dir, d_flag, f_flag);
                            chdir(current_work_dir);
                        }
                        else
                        {
                            char path[4000];
                            // strcpy(path, current_work_dir);
                            for (ll i = 0; i < strlen(current_work_dir); i++)
                            {
                                path[i] = current_work_dir[i];
                            }
                            path[strlen(current_work_dir)] = '\0';
                            if (chdir(my_cat(path, target_dir)) != 0)
                            {
                                perror("Error: ");
                                return;
                            }
                            traverse_dir(path, d_flag, f_flag);
                            chdir(current_work_dir);
                        }
                    }
                    else
                    {
                        char path[4000];
                        // strcpy(path, current_work_dir);
                        for (ll i = 0; i < strlen(current_work_dir); i++)
                        {
                            path[i] = current_work_dir[i];
                        }
                        path[strlen(current_work_dir)] = '\0';

                        my_cat(path, "/");
                        my_cat(path, target_dir);
                        // printf("%s\n", path);
                        chdir(path);
                        traverse_dir(path, d_flag, f_flag);
                        chdir(current_work_dir);
                    }
                }
            }
        }
        else
        {
            if (!target_dir_flag)
            {
                char cwd[3000];
                getcwd(cwd, 3000);
                if (!traverse_file(cwd, file_to_find, d_flag, f_flag))
                {
                    printf("the file doesnt exist\n");
                }
            }
            else
            {
                int x = 0;

                if (target_dir[0] == '.' && target_dir[1] == '.')
                {
                    chdir("..");
                    char parent[4000];
                    getcwd(parent, 4000);
                    // my_cat(parent, &target_dir[2]);
                    chdir(parent);
                    traverse_file(parent, file_to_find, d_flag, f_flag);
                    chdir(current_work_dir);
                }
                else if (target_dir[0] == '.')
                {
                    char temp[4000];
                    if (strncmp(current_work_dir, "~", strlen(current_work_dir)) == 0)
                    // strcpy(temp, home_dir);
                    {
                        for (ll i = 0; i < strlen(home_dir); i++)
                        {
                            temp[i] = home_dir[i];
                        }
                        temp[strlen(home_dir)] = '\0';
                    }
                    // else
                    //     strcpy(temp, current_work_dir);
                    else
                    {
                        // strcpy(temp, current_work_dir);
                        for (ll i = 0; i < strlen(current_work_dir); i++)
                        {
                            temp[i] = current_work_dir[i];
                        }
                        temp[strlen(current_work_dir)] = '\0';
                    }
                    // printf("%s\n",temp);
                    // my_cat(temp, &target_dir[1]);
                    // printf("%s\n",temp);
                    chdir(temp);
                    x = traverse_file(temp, file_to_find, d_flag, f_flag);
                    chdir(current_work_dir);
                }
                else if (target_dir[0] == '~')
                {
                    char temp[4000];
                    // strcpy(temp, home_dir);

                    for (ll i = 0; i < strlen(home_dir); i++)
                    {
                        temp[i] = home_dir[i];
                    }
                    temp[strlen(home_dir)] = '\0';
                    printf("%s\n", temp);
                    // my_cat(temp, &target_dir[1]);
                    // printf("%s\n",temp);
                    chdir(temp);
                    traverse_file(temp, file_to_find, d_flag, f_flag);
                    chdir(current_work_dir);
                }

                else
                {
                    if (target_dir[0] == '/')
                    {
                        if (chdir(target_dir) > -1 && chdir(target_dir) < 1)
                        {
                            traverse_file(target_dir, file_to_find, d_flag, f_flag);
                            chdir(current_work_dir);
                        }
                        else
                        {
                            char path[4000];
                            // strcpy(path, current_work_dir);
                            for (ll i = 0; i < strlen(current_work_dir); i++)
                            {
                                path[i] = current_work_dir[i];
                            }
                            path[strlen(current_work_dir)] = '\0';
                            if (chdir(my_cat(path, target_dir)))
                            {
                                perror("discover: ");
                                return;
                            }
                            
                            traverse_file(path, file_to_find, d_flag, f_flag);
                            chdir(current_work_dir);
                        }
                    }
                    else
                    {
                        char path[4000];
                        // strcpy(path, current_work_dir);
                        for (ll i = 0; i < strlen(current_work_dir); i++)
                        {
                            path[i] = current_work_dir[i];
                        }
                        path[strlen(current_work_dir)] = '\0';
                        my_cat(path, "/");
                        my_cat(path, target_dir);
                        chdir(path);
                        traverse_file(path, file_to_find, d_flag, f_flag);
                        chdir(current_work_dir);
                    }
                }
            }
        }
    }
}