#include "headers.h"

#define ll long long int

void child_process_handler(int signum, siginfo_t *info, void *ucontext)
{
	int wstatus;
	pid_t id;
	while (1)
	{
		id = waitpid(-1, &wstatus, WNOHANG | WUNTRACED);
		if (id < 1)
			return;
		// for (process *currPtr = process_ptr; currPtr != NULL; currPtr = currPtr->next)
		char *action = "exited";
		process *currPtr = process_ptr;
		while (currPtr != NULL)
		{
			if (id != currPtr->pid)
				continue;
			if (WIFEXITED(wstatus) != 0)
			{
				fprintf(stderr, "%s with pid %d %s normally\n", currPtr->name, id, action);
				deleteProcess(id);
			}
			else
			{
				if (WIFSTOPPED(wstatus) == 0 && WIFCONTINUED(wstatus) == 0)
				{
					fprintf(stderr, "%s with pid %d %s abnormally\n", currPtr->name, id, action);
					deleteProcess(id);
				}
			}
			break;
			currPtr = currPtr->next;
		}
		prompt();
		prompt_done = true;

		fflush(stdout);
		fflush(stderr);
	}
}

void signal_handler(int signum, siginfo_t *info, void *ucontext)
{
	// ll counter = 0;
	checkJobs();
	// counter++;
	// printf("counter: %lld\n", counter);
}
