#include "headers.h"
#define ll long long int

char *err = "too many arguments ";
void call_command(char *cmd, bool append_to_hist)
{
    if (!cmd)
        return;
    ll nCmds = 1;
    char *curr_cmd = strdup(cmd);
    char **store_cmd = (char **)calloc(nCmds, sizeof(char *));
    cmd = strtok(cmd, ";");
    if (!cmd)
        return;
    if (append_to_hist)
    {
        append_history(curr_cmd);
        // printf("curr_cmd = %s\n", curr_cmd);
        // for (char *ptr = cmd; *ptr; ++ptr)
        //     if (*ptr == ';')
        //         ++nCmds;
        char *ptr = cmd;
        while (*ptr)
        {
            if (*ptr == ';')
                ++nCmds;
            ptr++;
        }
        // printf("nCmds = %d\n", nCmds);
    }
    // printf("nCmds = %d\n", nCmds);
    sFree(curr_cmd);
    int z = 0;
    while (cmd != NULL)
    {
        store_cmd[z] = strdup(cmd);
        z++;
        cmd = strtok(NULL, ";");
    }
    // for (int i = 0; i < z; ++i)
    // {
    //     if (!isPipeOrRedirect(store_cmd[i]))
    //     {
    //         execute(store_cmd[i]);
    //     }
    //     sFree(store_cmd[i]);
    // }
    ll i = 0;
    while (i < z)
    {
        if (!isPipeOrRedirect(store_cmd[i]))
        {
            execute(store_cmd[i]);
        }
        sFree(store_cmd[i]);
        i++;
    }
    sFree(store_cmd);
}

void execute(char *cmd)
{
    char *token = strtok(cmd, delim);
    if (token == NULL)
    {
        return;
    }
    if (strncmp("exit", token, strlen(token)) == 0 || strncmp("quit", token, strlen(token)) == 0)
    {
        exit(0);
    }
    else if (strncmp("echo", token, strlen(token)) == 0)
    {
        call_echo(token);
    }
    else if (strncmp("pwd", token, strlen(token)) == 0)
    {
        present_working_directory();
    }
    else if (strncmp("cd", token, strlen(token)) == 0)
    {
        call_CD(token);
    }
    else if (strncmp("ls", token, strlen(token)) == 0)
    {
        char *instruction = token;
        ll num = 0;
        char **args = (char **)malloc(100 * sizeof(char *));
        while (instruction != NULL)
        {
            args[num] = instruction;
            ++num;
            instruction = strtok(NULL, delim);
        }
        ls(args, num);
    }
    else if (strncmp("history", token, strlen(token)) == 0)
    {
        token = strtok(NULL, delim);
        ll argc = 0, a = 0;
        while (token != NULL)
        {
            argc++;
            ll m = 1, digs = strlen(token);
            // for (int i = digs - 1; i >= 0; --i) {
            //     if (i == 0 && token[0] == '-') {
            //         a = -a;
            //     } else {
            //         a += m * (token[i] - '0');
            //         m *= 10;
            //     }
            // }
            a = atoi(token);
            token = strtok(NULL, delim);
        }
        char *cmd = "history";
        if (argc > 1)
        {
            printf("%s: too many arguments\n", cmd);
        }
        else
        {
            if (argc == 0)
                history(-1);
            else
            {
                if (a >= 0)
                {
                    history(a);
                }
                else
                {
                    printf("%s: invalid arguments\n", cmd);
                }
            }
        }
    }
    else if (strncmp("pinfo", token, strlen(token)) == 0)
    {
        char *num;
        num = strtok(NULL, delim);
        // printf("%s %s\n", instruction, num);
        pinfo(token, num);
    }
    else if (strncmp("discover", token, strlen(token)) == 0)
    {

        char *instruction = token;
        ll num = 0;
        char **args = (char **)malloc(100 * sizeof(char *));
        while (instruction != NULL)
        {
            args[num] = instruction;
            ++num;
            instruction = strtok(NULL, delim);
        }
        discover(args, num);
    }
    else if (strncmp("jobs", token, strlen(token)) == 0)
    {
        bool procStopped = false, procRunning = false, valid = true;
        while (token != NULL)
        {
            if (token[0] == '-')
            {
                // for (int i = 1; token[i] != '\0'; ++i)
                int i = 1;
                while(token[i] != '\0')
                {
                    if (token[i] == 's' && token[i] != 'r')
                    {
                        procStopped = true;
                    }
                    else if (token[i] == 'r' && token[i] != 's')
                    {
                        procRunning = true;
                    }
                    else
                    {
                        valid = false;
                        printf("Pash: jobs: %c: invalid option\n", token[i]);
                        break;
                    }
                    i++;
                }
            }
            token = strtok(NULL, delim);
        }
        if (valid != false)
        {
            jobs(procRunning, procStopped);
        }
    }
    else if (strncmp("sig", token, strlen(token)) == 0)
    {
        char *usage = "Usage: sig <job number> <signal number>";
        token = strtok(NULL, delim);
        if (!token)
        {
            printf("sig: %s\n", usage);
            return;
        }
        int jobNumber = atoi(token);
        token = strtok(NULL, delim);
        if (!token)
        {
            printf("sig: %s\n", usage);
            return;
        }
        int sigNumber = atoi(token);
        token = strtok(NULL, token);
        if (!token)
        {
            sig_sendSignal(jobNumber, sigNumber);
        }
        else
        {
            printf("Pash: sig: too many arguments\n");
        }
    }
    else if (strncmp("fg", token, strlen(token)) == 0)
    {
        token = strtok(NULL, delim);
        ll jNo = 0, args = 0, counter = 0;
        while (token != NULL)
        {
            ++args;
            jNo = atoi(token);
            token = strtok(NULL, delim);
            counter++;
        }
        if (args > 0)
        {
            if (args >= 2)
            {
                printf("Pash: fg: %s\n", err);
                return;
            }
            else if (jNo < 0 && jNo != 0)
            {
                printf("Pash: fg: invalid job number\n");
                return;
            }
            else
            {
                _foreground(jNo);
            }
        }
        else
        {
            _foreground(0);
        }
    }
    else if (strncmp("bg", token, strlen(token)) == 0)
    {
        token = strtok(NULL, delim);
        ll jNo = 0, args = 0;
        while (token != NULL)
        {
            ++args;
            jNo = atoi(token);
            token = strtok(NULL, delim);
        }
        if (args > 0)
        {
            if (args >= 2)
            {
                printf("Pash: bg: %s\n", err);
                return;
            }
            else if (jNo < 0 && jNo != 0)
            {
                printf("Pash: bg: invalid job number\n");
                return;
            }
            else
            {
                _background(jNo);
            }
        }
        else
        {
            _background(0);
        }
    }
    else
    {
        fgbg(token);
    }
}