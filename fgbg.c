#include "headers.h"

int argc, bg_no;
char *argv[265];
int background;
char *actions = "command not found";

void get_args(char *token)
{
    argc = 0;
    background = 0;
    while (token != NULL)
    {
        argv[argc] = token;
        token = strtok(NULL, delim);
        ++argc;
    }
    argv[argc] = NULL;
    if (strncmp(argv[argc - 1], "&", strlen(argv[argc - 1])) == 0)
    {
        background = 1;
        argv[argc - 1] = NULL;
        argc--;
    }
}

void fgbg(char *tkn)
{
    get_args(tkn);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Pash: fork");
    }
    else if (pid < 1 && pid > -1)
    {
        setpgid(0, 0);
        if (execvp(argv[0], argv) < 0)
        {
            printf("%s: %s\n", argv[0], actions);
        }
        _exit(0);
    }
    else
    {
        insertProcess(pid, argv[0]);
        setpgid(pid, pid);
        int counter = 0;
        if (!background)
        {
            int status;
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            if (counter == 0)
            {
                tcsetpgrp(STDIN_FILENO, getpgid(pid));
                tcsetpgrp(STDOUT_FILENO, getpgid(pid));
                // printf("counter: %d", counter);
            }

            counter++;

            waitpid(pid, &status, WUNTRACED);

            if (counter == 1)
            {
                tcsetpgrp(STDIN_FILENO, getpgrp());
                tcsetpgrp(STDOUT_FILENO, getpgrp());
            }
            // printf("Counter: ", counter);

            counter++;
            if (counter == 2)
            {
                signal(SIGTTIN, SIG_DFL);
                signal(SIGTTOU, SIG_DFL);
            }
            // printf("Counter: ", counter);
            if (WIFSTOPPED(status) == 0)
            {
                deleteProcess(pid);
            }
        }
        else
        {
            printf("%d\n", pid);
        }
    }
    for (int i = 0; i < argc; ++i)
    {
        argv[i] = NULL;
    }
}

void _foreground(int jobNo)
{
    checkJobs();
    int jobNoSpecified = 1;
    if (jobNo == 0)
    {
        jobNoSpecified = 0;
        // for (process *curr = process_ptr; curr != NULL; curr = curr->next)
        // {
        //     jobNo = max(jobNo, curr->JobNo);
        // }
        process *curr = process_ptr;
        while (curr != NULL)
        {
            jobNo = max(jobNo, curr->JobNo);
            curr = curr->next;
        }
    }
    pid_t childProcessPID = -1;
    int validJob = 0;
    char *name;
    // for (process *curr = process_ptr; curr != NULL; curr = curr->next)
    // {
    //     if (curr->JobNo == jobNo)
    //     {
    //         validJob = 1;
    //         childProcessPID = curr->pid;
    //         name = curr->name;
    //         break;
    //     }
    // }
    process *curr = process_ptr;
    while (curr != NULL)
    {
        if (curr->JobNo == jobNo)
        {
            validJob = 1;
            childProcessPID = curr->pid;
            name = curr->name;
            break;
        }
        curr = curr->next;
    }
    if (validJob == 0)
    {
        printf("Pash: fg: ");
        if (!jobNoSpecified)
        {
            printf("current: ");
        }
        else
        {
            printf("%d: ", jobNo);
        }
        printf("No such job\n");
        return;
    }

    int counter = 0;

    if (counter == 0)
    {
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
    }

    counter++;

    if (counter == 1)
    {
        tcsetpgrp(STDIN_FILENO, getpgid(childProcessPID));
        tcsetpgrp(STDOUT_FILENO, getpgid(childProcessPID));
    }
    // printf("counter: %d", counter);
    int status, err = 0;
    if (kill(childProcessPID, SIGCONT) < 0)
    {
        perror("Pash: fg");
        err = 1;
    }
    else
    {
        printf("%s\n", name);
        waitpid(childProcessPID, &status, WUNTRACED);
    }

    if (counter == 1)
    {
        tcsetpgrp(STDIN_FILENO, getpgrp());
        tcsetpgrp(STDOUT_FILENO, getpgrp());
    }
    // printf("Counter: ", counter);

    counter++;
    if (counter == 2)
    {
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
    }
    // printf("Counter: ", counter);

    if (WIFSTOPPED(status) == 0 && err)
    {
        deleteProcess(childProcessPID);
    }
}

void _background(int jobNo)
{
    checkJobs();
    int jobNoSpecified = 1;
    if (jobNo == 0)
    {
        jobNoSpecified = 0;
        // for (process *curr = process_ptr; curr != NULL; curr = curr->next)
        // {
        //     jobNo = max(jobNo, curr->JobNo);
        // }
        process *curr = process_ptr;
        while (curr != NULL)
        {
            jobNo = max(jobNo, curr->JobNo);
            curr = curr->next;
        }
    }
    pid_t childProcessPID = -1;
    int validJob = 0;
    // for (process *curr = process_ptr; curr != NULL; curr = curr->next)
    // {
    //     if (curr->JobNo == jobNo)
    //     {
    //         validJob = 1;
    //         childProcessPID = curr->pid;
    //         break;
    //     }
    // }
    process *curr = process_ptr;
    while (curr != NULL)
    {
        if (curr->JobNo == jobNo)
        {
            validJob = 1;
            childProcessPID = curr->pid;
            break;
        }
        curr = curr->next;
    }
    if (!validJob)
    {
        printf("Pash: bg: ");
        if (jobNoSpecified)
        {
            printf("%d: ", jobNo);
        }
        else
        {
            printf("current: ");
        }
        printf("No such job\n");
        return;
    }
    if (kill(childProcessPID, SIGCONT) < 0)
    {
        perror("Pash: bg");
    }
}
