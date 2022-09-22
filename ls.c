#include "headers.h"
#include "ls.h"

#define ll long long int

#define RED "\033[01;91m"
#define MAGENTA "\033[01;35m"
#define brightBLUE "\033[00;94m"
#define WHITE "\033[00;37m"
#define RESET "\033[00m"
#define boldYELLOW "\033[01;33m"

char *cat(char *destination, const char *source)
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

int isDirectory(const char *path)
{
    struct stat statbuf;
    // if (stat(path, &statbuf) != 0)
    //     return 0;
    // return S_ISDIR(statbuf.st_mode);
    int return_value;
    return_value = (stat(path, &statbuf) != 0) ? 0 : S_ISDIR(statbuf.st_mode);
    return return_value;
}

void ls_normal(char *dir)
{
    char *cmd = "ls";
    if (isDirectory(dir) == 0)
    {
        FILE *f = fopen(dir, "r");
        if (!f)
        {
            printf("%s: cannot access '%s': No such file or directory\n", cmd, dir);
            return;
        }
        printf("%s\n", dir);
        return;
    }
    DIR *p;
    struct dirent *d;
    p = opendir(dir);
    if (!p)
    {
        perror("ls: Could not access file or directory");
    }
    else
    {
        char **sorted = malloc(100 * sizeof(char *));
        ll *type = malloc(100 * sizeof(ll));
        ll sz = 0;
        while (d = readdir(p))
        {
            char *filename = d->d_name;
            struct stat fileStat;
            if (filename[0] == '.')
                continue;
            sorted[sz] = filename;
            stat(filename, &fileStat);
            if (d->d_type == 4)
                type[sz] = 1;
            else
            {
                if (fileStat.st_mode & S_IXUSR)
                    type[sz] = 3;
                else
                    type[sz] = 2;
            }
            sz++;
        }
        char *temp = malloc(1000 * sizeof(char));
        int temptype;
        ll i = 0;
        // for (int i = 0; i < sz; i++)
        while (i < sz)
        {
            ll j = i + 1;
            // for (int j = i + 1; j < sz; j++)
            while (j < sz)
            {
                if (strncmp(sorted[i], sorted[j], strlen(sorted[j])) > 0)
                {
                    // strcpy(temp, sorted[i]);
                    int k = 0;
                    for (k = 0; k < strlen(sorted[i]); k++)
                        temp[k] = sorted[i][k];
                    temp[k] = '\0';

                    // strcpy(sorted[i], sorted[j]);
                    k = 0;
                    for (k = 0; k < strlen(sorted[j]); k++)
                        sorted[i][k] = sorted[j][k];
                    sorted[i][k] = '\0';

                    // strcpy(sorted[j], temp);
                    k = 0;
                    for (k = 0; k < strlen(temp); k++)
                        sorted[j][k] = temp[k];
                    sorted[j][k] = '\0';
                    temptype = type[i];
                    type[i] = type[j];
                    type[j] = temptype;
                }
                j++;
            }
            i++;
        }
        for (int i = 0; i < sz; i++)
        {
            if (type[i] == 1)
                printf(RED "%s\n" RESET, sorted[i]);
            else if (type[i] == 2)
                printf(brightBLUE "%s\n" RESET, sorted[i]);
            else if (type[i] == 3)
                printf(boldYELLOW "%s\n" RESET, sorted[i]);
        }
    }
    closedir(p);
}

void lsa(char *dir)
{
    char *cmd = "ls";
    if (isDirectory(dir) == 0)
    {
        FILE *f = fopen(dir, "r");
        if (f == NULL)
        {
            printf("%s: cannot access '%s': No such file or directory\n", cmd, dir);
            return;
        }
        printf("%s\n", dir);
        return;
    }
    DIR *p;
    struct dirent *d;
    p = opendir(dir);
    if (!p)
    {
        perror("ls: Could not access file or directory");
        return;
    }
    else
    {
        char **sorted = malloc(100 * sizeof(char *));
        ll *type = malloc(100 * sizeof(ll));

        ll sz = 0;
        while (d = readdir(p))
        {
            struct stat fileStat;
            char *filename = d->d_name;
            sorted[sz] = filename;
            stat(d->d_name, &fileStat);
            // printf("%d ", d->d_type);
            if (d->d_type == 4)
                type[sz] = 1;
            else
            {
                if (fileStat.st_mode & S_IXUSR)
                {
                    type[sz] = 3;
                    // printf("here");
                }
                else
                    type[sz] = 2;
                // type[sz] = 2;
            }
            // type[sz] = 2;
            sz++;
        }
        char *temp = malloc(1000 * sizeof(char));
        int temptype;
        ll i = 0;
        // for (int i = 0; i < sz; i++)
        while (i < sz)
        {
            ll j = i + 1;
            // for (int j = i + 1; j < sz; j++)
            while (j < sz)
            {
                if (strncmp(sorted[i], sorted[j], strlen(sorted[j])) > 0)
                {
                    int k = 0;
                    // strcpy(temp, sorted[i]);
                    for (k = 0; k < strlen(sorted[i]); k++)
                        temp[k] = sorted[i][k];
                    temp[k] = '\0';

                    // strcpy(sorted[i], sorted[j]);
                    k = 0;
                    for (k = 0; k < strlen(sorted[j]); k++)
                        sorted[i][k] = sorted[j][k];
                    sorted[i][k] = '\0';

                    // strcpy(sorted[j], temp);
                    k = 0;
                    for (k = 0; k < strlen(temp); k++)
                        sorted[j][k] = temp[k];
                    sorted[j][k] = '\0';
                    temptype = type[i];
                    type[i] = type[j];
                    type[j] = temptype;
                }
                j++;
            }
            i++;
        }
        for (int i = 0; i < sz; i++)
        {
            if (type[i] == 1)
                printf(RED "%s\n" RESET, sorted[i]);
            else if (type[i] == 2)
                printf(brightBLUE "%s\n" RESET, sorted[i]);
            else
                printf(boldYELLOW "%s\n" RESET, sorted[i]);
        }
    }
    closedir(p);
}

void lsl(char *dir)
{
    char *cmd = "ls";
    if (!isDirectory(dir))
    {
        FILE *f = fopen(dir, "r");
        if (!f)
        {
            printf("%s: cannot access '%s': No such file or directory\n", cmd, dir);
            return;
        }
        struct stat s_file;
        stat(dir, &s_file);
        // printf((s_file.st_mode & S_IRUSR) ? "r" : "-");
        if (s_file.st_mode & S_IRUSR)
            printf("r");
        else
            printf("-");
        // printf((s_file.st_mode & S_IWUSR) ? "w" : "-");
        if (s_file.st_mode & S_IWUSR)
            printf("w");
        else
            printf("-");
        // printf((s_file.st_mode & S_IXUSR) ? "x" : "-");
        if (s_file.st_mode & S_IXUSR)
            printf("x");
        else
            printf("-");
        // printf((s_file.st_mode & S_IRGRP) ? "r" : "-");
        if (s_file.st_mode & S_IRGRP)
            printf("r");
        else
            printf("-");
        // printf((s_file.st_mode & S_IWGRP) ? "w" : "-");
        if (s_file.st_mode & S_IWGRP)
            printf("w");
        else
            printf("-");
        // printf((s_file.st_mode & S_IXGRP) ? "x" : "-");
        if (s_file.st_mode & S_IXGRP)
            printf("x");
        else
            printf("-");
        // printf((s_file.st_mode & S_IROTH) ? "r" : "-");
        if (s_file.st_mode & S_IROTH)
            printf("r");
        else
            printf("-");
        // printf((s_file.st_mode & S_IWOTH) ? "w" : "-");
        if (s_file.st_mode & S_IWOTH)
            printf("w");
        else
            printf("-");
        // printf((s_file.st_mode & S_IXOTH) ? "x" : "-");
        if (s_file.st_mode & S_IXOTH)
            printf("x");
        else
            printf("-");

        struct tm *time;
        time = localtime(&s_file.st_mtime);
        // printf(" %3d", (int)s_file.st_nlink);
        // printf(" %8.8s", getpwuid(s_file.st_uid)->pw_name);
        // printf("%8.8s", getgrgid(s_file.st_gid)->gr_name);
        // printf(" %6lld", (long long int)s_file.st_size);
        printf(" %3d %8.8s %8.8s %6lld ", (int)s_file.st_nlink, getpwuid(s_file.st_uid)->pw_name, getgrgid(s_file.st_gid)->gr_name, (long long int)s_file.st_size);

        char date[1000];

        strftime(date, 1000, "%b  %d %H:%M", time);
        printf(" %s ", date);
        // printf("\n%s", dir);
        if (S_ISDIR(s_file.st_mode))
            printf(RED "%s\n" RESET, dir);
        else
        {
            if (s_file.st_mode & S_IXUSR)
                printf(boldYELLOW "%s\n" RESET, dir);
            else
                printf(brightBLUE "%s\n" RESET, dir);
        }
        return;
    }
    DIR *p;
    struct dirent *d;
    p = opendir(dir);
    if (!p)
    {
        perror("ls: Error");
    }
    else
    {
        struct stat s_file;
        char **sorted = malloc(100 * sizeof(char *));
        ll sz = 0;
        long long int blocks = 0;
        for (; d = readdir(p);)
        {
            char *filename = d->d_name;
            if (filename[0] == '.')
                continue;
            sorted[sz] = filename;
            ++sz;
            char path[1000];
            int k = 0;
            // strcpy(path, dir);
            for (k = 0; k < strlen(dir); k++)
                path[k] = dir[k];
            path[k] = '/';
            k++;
            path[k] = '\0';
            // cat(path, "/");
            cat(path, filename);
            stat(path, &s_file);
            ll mem_blocks = blocks;
            blocks = mem_blocks + s_file.st_blocks;
        }
        printf("total %lld\n", blocks / 2);
        char *temp = malloc(1000 * sizeof(char));
        ll i = 0;
        // for (int i = 0; i < sz; i++)
        while (i < sz)
        {
            ll j = 0;
            // for (int j = i + 1; j < sz; j++)
            while (j < sz)
            {
                if (strncmp(sorted[i], sorted[j], strlen(sorted[i])) > 0)
                {
                    ll k = 0;
                    // strcpy(temp, sorted[i]);
                    for (k = 0; k < strlen(sorted[i]); k++)
                        temp[k] = sorted[i][k];
                    temp[k] = '\0';

                    // strcpy(sorted[i], sorted[j]);
                    k = 0;
                    for (k = 0; k < strlen(sorted[j]); k++)
                        sorted[i][k] = sorted[j][k];
                    sorted[i][k] = '\0';

                    // strcpy(sorted[j], temp);
                    k = 0;
                    for (k = 0; k < strlen(temp); k++)
                        sorted[j][k] = temp[k];
                    sorted[j][k] = '\0';
                }
                j++;
            }
            i++;
        }
        i = 0;
        // for (int i = 0; i < sz; i++)
        while (i < sz)
        {
            struct stat s_file;
            char *filename = sorted[i];
            char *filepath;
            filepath = malloc(100 * sizeof(char));
            // filepath of all files
            // strcpy(filepath, dir);
            int k = 0;
            for (k = 0; k < strlen(dir); k++)
                filepath[k] = dir[k];
            filepath[k] = '\0';
            cat(filepath, "/");
            cat(filepath, filename);

            stat(filepath, &s_file);
            // printf((s_file.st_mode & S_IRUSR) ? "r" : "-");
            if (s_file.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");
            // printf((s_file.st_mode & S_IWUSR) ? "w" : "-");
            if (s_file.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");
            // printf((s_file.st_mode & S_IXUSR) ? "x" : "-");
            if (s_file.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");
            // printf((s_file.st_mode & S_IRGRP) ? "r" : "-");
            if (s_file.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");
            // printf((s_file.st_mode & S_IWGRP) ? "w" : "-");
            if (s_file.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");
            // printf((s_file.st_mode & S_IXGRP) ? "x" : "-");
            if (s_file.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");
            // printf((s_file.st_mode & S_IROTH) ? "r" : "-");
            if (s_file.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");
            // printf((s_file.st_mode & S_IWOTH) ? "w" : "-");
            if (s_file.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");
            // printf((s_file.st_mode & S_IXOTH) ? "x" : "-");
            if (s_file.st_mode & S_IXOTH)
                printf("x");
            else
                printf("-");

            struct tm *time;
            char date[1000];

            time = localtime(&s_file.st_mtime);
            strftime(date, 1000, "%b  %d %H:%M", time);

            // printf(" %3d", (int)s_file.st_nlink);
            // printf(" %8.8s", getpwuid(s_file.st_uid)->pw_name);
            // printf("%8.8s", getgrgid(s_file.st_gid)->gr_name);
            // printf(" %6lld", (long long int)s_file.st_size);
            // printf(" %s ", date);
            printf(" %3d %8.8s %8.8s %6lld %s ", (int)s_file.st_nlink, getpwuid(s_file.st_uid)->pw_name, getgrgid(s_file.st_gid)->gr_name, (long long int)s_file.st_size, date);

            if (S_ISDIR(s_file.st_mode))
                printf(RED "%s\n" RESET, filename);
            else
            {
                if (s_file.st_mode & S_IXUSR)
                    printf(boldYELLOW "%s\n" RESET, filename);
                else
                    printf(brightBLUE "%s\n" RESET, filename);
            }
            i++;
        }
    }
    closedir(p);
}

void lsla(char *dir)
{
    char *cmd = "ls";
    if (!isDirectory(dir))
    {
        FILE *f = fopen(dir, "r");
        if (!f)
        {
            printf("%s: cannot access '%s': No such file or directory\n", cmd, dir);
            return;
        }
        struct stat s_file;
        stat(dir, &s_file);
        // printf((s_file.st_mode & S_IRUSR) ? "r" : "-");
        if (s_file.st_mode & S_IRUSR)
            printf("r");
        else
            printf("-");
        // printf((s_file.st_mode & S_IWUSR) ? "w" : "-");
        if (s_file.st_mode & S_IWUSR)
            printf("w");
        else
            printf("-");
        // printf((s_file.st_mode & S_IXUSR) ? "x" : "-");
        if (s_file.st_mode & S_IXUSR)
            printf("x");
        else
            printf("-");
        // printf((s_file.st_mode & S_IRGRP) ? "r" : "-");
        if (s_file.st_mode & S_IRGRP)
            printf("r");
        else
            printf("-");
        // printf((s_file.st_mode & S_IWGRP) ? "w" : "-");
        if (s_file.st_mode & S_IWGRP)
            printf("w");
        else
            printf("-");
        // printf((s_file.st_mode & S_IXGRP) ? "x" : "-");
        if (s_file.st_mode & S_IXGRP)
            printf("x");
        else
            printf("-");
        // printf((s_file.st_mode & S_IROTH) ? "r" : "-");
        if (s_file.st_mode & S_IROTH)
            printf("r");
        else
            printf("-");
        // printf((s_file.st_mode & S_IWOTH) ? "w" : "-");
        if (s_file.st_mode & S_IWOTH)
            printf("w");
        else
            printf("-");
        // printf((s_file.st_mode & S_IXOTH) ? "x" : "-");
        if (s_file.st_mode & S_IXOTH)
            printf("x");
        else
            printf("-");

        struct tm *time;
        char date[1000];
        time = localtime(&s_file.st_mtime);
        strftime(date, 1000, "%b  %d %H:%M", time);

        // printf(" %3d", (int)s_file.st_nlink);
        // printf(" %8.8s", getpwuid(s_file.st_uid)->pw_name);
        // printf("%8.8s", getgrgid(s_file.st_gid)->gr_name);
        // printf(" %6lld", (long long int)s_file.st_size);
        // printf(" %s ", date);

        printf(" %3d %8.8s %8.8s %6lld %s ", (int)s_file.st_nlink, getpwuid(s_file.st_uid)->pw_name, getgrgid(s_file.st_gid)->gr_name, (long long int)s_file.st_size, date);
        if (S_ISDIR(s_file.st_mode))
            printf(RED "%s\n" RESET, dir);
        else if (s_file.st_mode & S_IXUSR)
            printf(boldYELLOW "%s\n" RESET, dir);
        else
            printf(brightBLUE "%s\n" RESET, dir);
        return;
    }
    DIR *p;
    struct dirent *d;
    p = opendir(dir);
    if (!p)
    {
        perror("Error");
    }
    else
    {
        char **sorted = malloc(100 * sizeof(char *));
        ll sz = 0;
        long long int blocks = 0;
        for (; d = readdir(p);)
        {
            struct stat s_file;
            char *filename = d->d_name;
            sorted[sz] = filename;
            ++sz;
            char path[1000];
            // strcpy(path, dir);
            ll k = 0;
            while (dir[k] != '\0')
            {
                path[k] = dir[k];
                ++k;
            }
            path[k] = '\0';
            cat(path, "/");
            cat(path, filename);

            stat(path, &s_file);
            long long int mem_blocks = blocks;
            blocks = mem_blocks + s_file.st_blocks;
        }
        printf("total %lld\n", blocks / 2);
        char *temp = malloc(1000 * sizeof(char));

        ll i = 0;
        while (i < sz)
        // for (int i = 0; i < sz; i++)
        {
            ll j = i + 1;
            while (j < sz)
            // for (int j = i + 1; j < sz; j++)
            {
                if (strcmp(sorted[i], sorted[j]) > 0)
                {
                    ll k = 0;
                    // strcpy(temp, sorted[i]);
                    while (sorted[i][k] != '\0')
                    {
                        temp[k] = sorted[i][k];
                        ++k;
                    }
                    temp[k] = '\0';

                    // strcpy(sorted[i], sorted[j]);
                    k = 0;
                    while (sorted[j][k] != '\0')
                    {
                        sorted[i][k] = sorted[j][k];
                        ++k;
                    }
                    sorted[i][k] = '\0';

                    // strcpy(sorted[j], temp);
                    k = 0;
                    while (temp[k] != '\0')
                    {
                        sorted[j][k] = temp[k];
                        ++k;
                    }
                    sorted[j][k] = '\0';
                }
                j++;
            }
            i++;
        }
        i = 0;
        while (i < sz)
        // for (int i = 0; i < sz; i++)
        {
            char *filename = sorted[i];
            char *filepath;
            filepath = malloc(100 * sizeof(char));
            // strcpy(filepath, dir);
            ll k = 0;
            while (dir[k] != '\0')
            {
                filepath[k] = dir[k];
                ++k;
            }
            filepath[k] = '\0';
            cat(filepath, "/");
            cat(filepath, filename);
            struct stat s_file;
            stat(filepath, &s_file);
            // printf((s_file.st_mode & S_IRUSR) ? "r" : "-");
            if (s_file.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");
            // printf((s_file.st_mode & S_IWUSR) ? "w" : "-");
            if (s_file.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");
            // printf((s_file.st_mode & S_IXUSR) ? "x" : "-");
            if (s_file.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");
            // printf((s_file.st_mode & S_IRGRP) ? "r" : "-");
            if (s_file.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");
            // printf((s_file.st_mode & S_IWGRP) ? "w" : "-");
            if (s_file.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");
            // printf((s_file.st_mode & S_IXGRP) ? "x" : "-");
            if (s_file.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");
            // printf((s_file.st_mode & S_IROTH) ? "r" : "-");
            if (s_file.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");
            // printf((s_file.st_mode & S_IWOTH) ? "w" : "-");
            if (s_file.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");
            // printf((s_file.st_mode & S_IXOTH) ? "x" : "-");
            if (s_file.st_mode & S_IXOTH)
                printf("x");
            else
                printf("-");

            struct tm *time;
            char date[1000];

            time = localtime(&s_file.st_mtime);
            strftime(date, 1000, "%b  %d %H:%M", time);

            // printf(" %3d", (int)s_file.st_nlink);
            // printf(" %8.8s", getpwuid(s_file.st_uid)->pw_name);
            // printf("%8.8s", getgrgid(s_file.st_gid)->gr_name);
            // printf(" %6lld", (long long int)s_file.st_size);
            // printf(" %s ", date);
            printf(" %3d %8.8s %8.8s %6lld %s ", (int)s_file.st_nlink, getpwuid(s_file.st_uid)->pw_name, getgrgid(s_file.st_gid)->gr_name, (long long int)s_file.st_size, date);
            if (S_ISDIR(s_file.st_mode))
            {
                printf(RED "%s\n" RESET, filename);
            }
            else
            {
                if (s_file.st_mode & S_IXUSR)
                    printf(boldYELLOW "%s\n" RESET, filename);
                else
                    printf(brightBLUE "%s\n" RESET, filename);
            }
            i++;
        }
    }
    closedir(p);
}

void lsprev()
{
    char *cmd = "ls";
    char dir[1000];
    getcwd(dir, 1000);
    if (!isDirectory(dir))
    {
        FILE *f = fopen(dir, "r");
        if (!f)
        {
            printf("%s: cannot access '%s': No such file or directory\n", cmd, dir);
            return;
        }
        printf("%s\n", dir);
        return;
    }
    if (strlen(dir) > 0 && strlen(dir) < 2)
    {
        return;
    }
    else
    {
        ll len = strlen(dir);
        ll pos = -1;
        ll i = 0;
        while (i < len)
        // for (ll i = 0; i < len; i++)
        {
            if (dir[i] == '/')
                pos = i;
            i++;
        }
        char *curr = malloc(1000 * sizeof(char));
        ll sz = 0;
        for (ll i = 0; i < pos; i++)
        {
            curr[sz] = dir[i];
            sz++;
        }
        curr[sz++] = '\0';
        ls_normal(curr);
    }
}

void ls(char **s, int num)
{
    if (num == 1)
    {
        char dir[1000];
        getcwd(dir, 1000);
        ls_normal(dir);
    }
    else if (num == 2)
    {
        char dir[1000];
        getcwd(dir, 1000);

        if (strncmp(s[1], ".", strlen(s[1])) == 0)
        {
            ls_normal(dir);
        }
        else if (strncmp(s[1], "-a", strlen(s[1])) == 0)
        {
            lsa(dir);
        }
        else if (strncmp(s[1], "-l", strlen(s[1])) == 0)
        {
            lsl(dir);
        }
        else if (strncmp(s[1], "..", strlen(s[1])) == 0)
        {
            lsprev();
        }
        else if (strncmp(s[1], "~", strlen(s[1])) == 0)
        {
            ls_normal(home_dir);
        }
        else if (strncmp(s[1], "-la", strlen(s[1])) == 0 || strncmp(s[1], "-al", strlen(s[1])) == 0)
        {
            lsla(dir);
        }
        else
        {
            ll f = 0;
            f = 1;
            ll i = 1;
            ll a_flag = 0, l_flag = 0;
            if (s[i][0] == '~')
            {
                char *path = malloc(1000 * sizeof(char));
                int j = 0, k = 1;
                // for (j = 0; j < strlen(home_dir); j++)
                while(j < strlen(home_dir))
                {
                    path[j] = home_dir[j];
                    j++;
                }
                // for (int k = 1; k <= strlen(s[i]); k++)
                while(k <= strlen(s[i]))
                {
                    path[j] = s[i][k];
                    j++;
                    k++;
                }
                if (a_flag != 0 && l_flag != 0)
                    lsla(path);
                else if (a_flag != 0)
                    lsa(path);
                else if (l_flag != 0)
                    lsl(path);
                else
                    ls_normal(path);
            }
            else
            {
                if (a_flag != 0 && l_flag != 0)
                    lsla(s[i]);
                else if (a_flag != 0)
                    lsa(s[i]);
                else if (l_flag != 0)
                    lsl(s[i]);
                else
                    ls_normal(s[i]);
            }
        }
    }
    else
    {
        ll a_flag = 0, l_flag = 0;
        ll f = 0;
        ll i = 1;
        // for (int i = 1; i < num; i++)
        while (i < num)
        {
            if (strncmp(s[i], "-l", strlen(s[i])) == 0)
                l_flag = 1;
            else if (strncmp(s[i], "-a", strlen(s[i])) == 0)
                a_flag = 1;
            else if (strncmp(s[i], "-al", strlen(s[i])) == 0 || strncmp(s[i], "-la", strlen(s[i])) == 0)
            {
                a_flag = 1, l_flag = 1;
            }
            i++;
        }
        ll cnt = 0;
        for (ll i = 1; i < num; i++)
        {
            if (!strncmp(s[i], "-l", strlen(s[i])) == 0)
                continue;
            else if (!strncmp(s[i], "-a", strlen(s[i])) == 0)
                continue;
            else if (!strcmp(s[i], "-al") || !strcmp(s[i], "-la"))
                continue;
            else
            {
                cnt++;
            }
        }
        if (cnt == 1)
        {
            for (ll i = 1; i < num; i++)
            {
                if (!strncmp(s[i], "-l", strlen(s[i])) == 0)
                    continue;
                else if (!strncmp(s[i], "-a", strlen(s[i])) == 0)
                    continue;
                else if (!strcmp(s[i], "-al") || !strcmp(s[i], "-la"))
                    continue;
                else
                {
                    f = 1;
                    if (s[i][0] == '~')
                    {
                        char *path = malloc(1000 * sizeof(char));
                        int j = 0, k = 1;
                        // for (j = 0; j < strlen(home_dir); j++)
                        while (j < strlen(home_dir))
                        {
                            path[j] = home_dir[j];
                            j++;
                        }
                        // for (int k = 1; k <= strlen(s[i]); k++)
                        while (k <= strlen(s[i]))
                        {
                            path[j++] = s[i][k];
                            k++;
                        }
                        if (a_flag != 0 && l_flag != 0)
                            lsla(path);
                        else if (a_flag != 0)
                            lsa(path);
                        else if (l_flag != 0)
                            lsl(path);
                        else
                            ls_normal(path);
                    }
                    else
                    {
                        if (a_flag != 0 && l_flag != 0)
                            lsla(s[i]);
                        else if (a_flag != 0)
                            lsa(s[i]);
                        else if (l_flag != 0)
                            lsl(s[i]);
                        else
                            ls_normal(s[i]);
                    }
                }
            }
        }
        else
        {
            for (ll i = 1; i < num; i++)
            {
                if (strncmp(s[i], "-l", strlen(s[i])) == 0)
                    continue;
                else if (strncmp(s[i], "-a", strlen(s[i])) == 0)
                    continue;
                else if (!strcmp(s[i], "-al") || !strcmp(s[i], "-la"))
                    continue;
                else
                {
                    f = 1;
                    if (s[i][0] == '~')
                    {
                        char *path = malloc(1000 * sizeof(char));
                        ll j = 0, k = 1;
                        // for (j = 0; j < strlen(home_dir); j++)
                        while (j < strlen(home_dir))
                        {
                            path[j] = home_dir[j];
                            j++;
                        }
                        // for (int k = 1; k <= strlen(s[i]); k++)
                        while (k <= strlen(s[i]))
                        {
                            path[j++] = s[i][k];
                            k++;
                        }
                        printf(RED "%s :\n" RESET, path);
                        // printf("%s :\n",s[i]);
                        if (a_flag != 0 && l_flag != 0)
                            lsla(path);
                        else if (a_flag != 0)
                            lsa(path);
                        else if (l_flag != 0)
                            lsl(path);
                        else
                            ls_normal(path);
                    }
                    else
                    {
                        printf(RED "%s :\n" RESET, s[i]);
                        // printf("%s :\n",s[i]);
                        if (a_flag != 0 && l_flag != 0)
                            lsla(s[i]);
                        else if (a_flag != 0)
                            lsa(s[i]);
                        else if (l_flag != 0)
                            lsl(s[i]);
                        else
                            ls_normal(s[i]);
                    }
                }
            }
        }

        char dir[1500];
        getcwd(dir, 1500);
        if (!f)
        {
            if (a_flag != 0 && l_flag != 0)
                lsla(dir);
            else if (a_flag != 0)
                lsa(dir);
            else if (l_flag != 0)
                lsl(dir);
            else
                ls_normal(dir);
        }
    }
}