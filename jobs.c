#include "headers.h"

#define ll long long int

// const char *run = "Running", *stop = "Stopped";
processJobState JOBS[1500];
ll nJobs;

int compare_jobs(const void *a, const void *b)
{
    int val1 = 0;
    val1 = strncmp((const char *)((processJobState *)a)->command_name, (const char *)((processJobState *)b)->command_name, strlen((const char *)((processJobState *)a)->command_name));
    return val1;
}

int processExists(pid_t pid)
{
    return (kill(pid, 0) == 0);
}

void checkJobs()
{
    ll z = 0;
    nJobs = 0;
    process *curr = process_ptr;
    char process_stat_path[4000] = {};
    // for (process* curr = process_ptr; curr != NULL; curr = curr->next)
    while (curr != NULL)
    {
        if (processExists(curr->pid))
        {
            z = max(z, curr->JobNo);
        }
        else
        {
            deleteProcess(curr->pid);
        }
        curr = curr->next;
    }
    z++;
    curr = process_ptr;
    // for (process* curr = process_ptr; curr != NULL; curr = curr->next)
    while (curr != NULL)
    {
        sprintf(process_stat_path, "/proc/%d/stat", curr->pid);
        FILE *file = fopen(process_stat_path, "r");
        if (!file)
            continue;
        char executable[300] = {};
        ll counter = 0;
        fscanf(file, "%d %s %c", &JOBS[nJobs].pid, executable, &JOBS[nJobs].status);
        counter++;
        fclose(file);
        if (curr->JobNo < 0)
        {
            curr->JobNo = JOBS[nJobs].jobNo = z;
            z++;
        }
        else
        {
            JOBS[nJobs].jobNo = curr->JobNo;
        }
        JOBS[nJobs].command_name = strdup(curr->name);
        ++nJobs;
        curr = curr->next;
    }
}

void jobs(bool running, bool stopped)
{
    checkJobs();
    qsort(JOBS, nJobs, sizeof(processJobState), compare_jobs);
    for (ll i = 0; i < nJobs; i++)
    {
        if ((running && JOBS[i].status == 'T') || (stopped && JOBS[i].status != 'T'))
            continue;
        // if (stopped && JOBS[i].status != 'T') continue;
        // printf("[%d] %s %s [%d]\n", (JOBS[i].jobNo), (JOBS[i].status == 'T' ? "Stopped" : "Running"), JOBS[i].command_name, JOBS[i].pid);
        printf("[%d] ", (JOBS[i].jobNo));
        if (JOBS[i].status == 'T')
            printf("Stopped ");
        else
            printf("Running ");
        printf("%s ", JOBS[i].command_name);
        printf("[%d]\n", JOBS[i].pid);
        free(JOBS[i].command_name);
    }
}

void sig_sendSignal(int jobNumber, int signalNumber)
{
    checkJobs();
    int validJobNo = 0;

    // for (process *curr = process_ptr; curr != NULL; curr = curr->next)
    process *curr = process_ptr;
    while (curr != NULL)
    {
        if (curr->JobNo == jobNumber)
        {
            int ret = kill(curr->pid, signalNumber);
            if (ret <= -1)
            {
                perror("Pash: sig");
            }
            validJobNo = 1;
            break;
        }
        curr = curr->next;
    }
    // for (process *curr = process_ptr; curr != NULL; curr = curr->next)
    curr = process_ptr;
    while (curr != NULL)
    {
        if (!processExists(curr->pid))
        {
            deleteProcess(curr->pid);
        }
        curr = curr->next;
    }
    if (!validJobNo)
    {
        printf("Pash: sig: %d", jobNumber);
        printf(": No such job\n");
    }
}