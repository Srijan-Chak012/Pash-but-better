#include "headers.h"

#define ll long long int

char *getFileName(int start, int *end, char *cmd)
{
    if (start < 0)
        return NULL;
    char *File = NULL;
    ll l, r;
    l = start + 1;
    // for (l = start + 1; cmd[l] == ' ' || cmd[l] == '\n' || cmd[l] == '\t'; ++l);
    while (cmd[l] == ' ' || cmd[l] == '\n' || cmd[l] == '\t')
        l++;
    r = l;
    // for (r = l + 1; cmd[r] && cmd[r] != ' ' && cmd[r] != '\n' && cmd[r] != '\t'; ++r);
    while (cmd[r] && cmd[r] != ' ' && cmd[r] != '\n' && cmd[r] != '\t')
        r++;
    File = (char *)calloc((r - l + 1), sizeof(char));
    ll i = l;
    // for (int i = l; i < r; ++i)
    while (i < r)
    {
        File[i - l] = cmd[i];
        i++;
    }
    *end = r;
    return File;
}

int isPipeOrRedirect(char *cmd)
{
    int append = 0;
    int inpPos = -1, outPos = -1, nPipes = 0, inpPos2 = -1, outPos2 = -1;
    ll i = 0;
    // for (int i = 0; cmd[i]; ++i)
    while(cmd[i])
    {
        if (cmd[i] == '<')
        {
            inpPos = i;
        }
        else if (cmd[i] == '>')
        {
            outPos = i;
            if (i >= 1 && cmd[i - 1] == '>')
                append = 1;
        }
        else if (cmd[i] == '|')
        {
            nPipes++;
        }
        i++;
    }
    if (inpPos < 0 && outPos < 0 && nPipes == 0)
    {
        return 0;
    }
    char *inputFile = getFileName(inpPos, &inpPos2, cmd);
    char *outputFile = getFileName(outPos, &outPos2, cmd);
    int FDin, FDout;

    if (outputFile != NULL)
    {
        FDout = open(outputFile, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
        close(FDout);
    }

    if (nPipes < 1)
    {
        if (outputFile != NULL)
        {
            FDout = open(outputFile, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
        }
        else
        {
            FDout = dup(STD_OUT_FD);
        }
        if (inputFile != NULL)
        {
            FDin = open(inputFile, O_RDONLY);
            if (FDin < 0)
            {
                char msg[1500] = {};
                sprintf(msg, "Pash: %s: No such file or directory", inputFile);
                perror(msg);
                close(FDout);
                dup2(STD_INP_FD, STDIN_FILENO);
                dup2(STD_OUT_FD, STDOUT_FILENO);
                return 1;
            }
        }
        else
        {
            FDin = dup(STD_INP_FD);
        }
        cmd = strtok(cmd, "><");
        dup2(FDout, STDOUT_FILENO);
        dup2(FDin, STDIN_FILENO);
        close(FDout);
        close(FDin);
        execute(cmd);
        dup2(STD_INP_FD, STDIN_FILENO);
        dup2(STD_OUT_FD, STDOUT_FILENO);
        return 1;
    }
    char *tkn = strtok(cmd, "|");
    FDin = dup(STD_INP_FD);
    for (int cmdNo = 0; tkn != NULL; cmdNo++)
    {
        int LESS = 0, GREAT = 0;
        int len = strlen(tkn);
        for (char *c = tkn; *c; ++c)
        {
            if (*c == '<')
            {
                LESS = 1;
                *c = '\0';
                len = c - tkn;
            }
            if (*c == '>')
            {
                GREAT = 1;
                *c = '\0';
                len = c - tkn;
            }
        }
        char *curr_cmd = (char *)calloc(len + 1, sizeof(char));
        strncpy(curr_cmd, tkn, strlen(tkn));
        tkn = strtok(NULL, "|");

        dup2(FDin, STDIN_FILENO);
        close(FDin);
        if (LESS)
        {
            FDin = open(inputFile, O_RDONLY);
            if (FDin < 0)
            {
                char msg[1500];
                sprintf(msg, "Pash: %s: No such file or directory", inputFile);
                perror(msg);
            }
            else
            {
                dup2(FDin, STDIN_FILENO);
                close(FDin);
            }
        }
        if (!tkn)
        {
            if (outputFile && GREAT)
            {
                FDout = open(outputFile, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
            }
            else
            {
                FDout = dup(STD_OUT_FD);
            }
        }
        else
        {
            int fdPipe[2] = {};
            pipe(fdPipe);
            FDout = fdPipe[1];
            FDin = fdPipe[0];
        }
        dup2(FDout, STDOUT_FILENO);
        close(FDout);

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Pash: fork done goofed");
        }
        else if (pid == 0)
        {
            execute(curr_cmd);
            _exit(EXIT_SUCCESS);
        }
        waitpid(pid, NULL, WUNTRACED);
        dup2(STD_INP_FD, STDIN_FILENO);
        dup2(STD_OUT_FD, STDOUT_FILENO);
    }
    return 1;
}
