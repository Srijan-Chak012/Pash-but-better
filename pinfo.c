#include "headers.h"
#include "pinfo.h"

#define ll long long int

void pinfo(char *string, char *num)
{
    // printf("%s %s\n", string, num);
    pid_t process_id;
    // if (num == NULL)
    // {
    //     process_id = getpid();
    // }
    // else
    // {
    //     process_id = atoi(num);
    // }
    (num == NULL) ? (process_id = getpid()) : (process_id = atoi(num));
    // printf("pid -- %d\n", process_id);
    char *path = malloc(1000 * sizeof(char));
    sprintf(path, "/proc/%d/stat", process_id);
    FILE *status = fopen(path, "r");
    // if(status == NULL) {
    //     perror("Error: no process\n");
    //     return;
    // }
    (status == NULL) ? (perror("Error: no process\n")) : (0);
    if (!status)
    {
        return;
    }
    // printf("Here\n");
    ll storepid;
    char r_s = '-';
    ll par_group;
    ll set_group;
    ll me;

    fscanf(status, "%lld %*s %c %*s %lld %*s %*s %lld %*s %*s %*s %*s %*s %*s %*s %*s %*s " "%*s %*s %*s %*s %*s %lld", &storepid, &r_s, &par_group, &set_group, &me);

    char fgorbg = '-';
    // if(par_group==set_group) fgorbg='+';
    // else fgorbg=' ';
    (par_group == set_group) ? (fgorbg = '+') : (fgorbg = ' ');
    char *info = malloc(1000 * sizeof(char));
    info = "pid";
    printf("%s -- %d\n", info, process_id);
    info = "Process Status";
    printf("%s -- %c%c\n", info, r_s, fgorbg);
    info = "Memory";
    char *memorytype = malloc(1000 * sizeof(char));
    memorytype = "Virtual Memory";
    printf("%s -- %lld {%s}\n", info, me, memorytype);
    fclose(status);
    char *execpath = malloc(1000 * sizeof(char));
    sprintf(execpath, "/proc/%d/exe", process_id);
    char exepath[1500];
    int temp = readlink(execpath, exepath, sizeof(exepath));
    exepath[strlen(exepath)] = '\0';

    char display[1500];

    if (strlen(exepath) < strlen(home_dir))
    {
        ll size = 0, i = 0;
        // for (int i = 0; i <= n; i++)
        //     display[size++] = exepath[i];
        while (i <= strlen(exepath))
        {
            display[size++] = exepath[i];
            i++;
        }
    }
    else
    {
        char compare[1500];
        int szz = 0, i = 0;
        // for (int i = 0; i < len; i++)
        // {
        //     compare[szz++] = exepath[i];
        // }
        while (i < strlen(home_dir))
        {
            compare[szz++] = exepath[i];
            i++;
        }
        compare[szz++] = '\0';
        if (strncmp(compare, home_dir, strlen(home_dir)) == 0)
        {
            display[0] = '~';
            ll size = 1, i = strlen(home_dir);
            // for (int i = len; i <= n; i++)
            // {
            //     display[size++] = exepath[i];
            // }
            while (i <= strlen(exepath))
            {
                display[size++] = exepath[i];
                i++;
            }
        }
        else
        {
            // strcpy(display, exepath);
            for (int i = 0; i < strlen(exepath); i++)
            {
                display[i] = exepath[i];
            }
            display[strlen(exepath)] = '\0';
        }
    }
    info = "Executable Path";
    printf("%s -- %s\n", info, display);
}