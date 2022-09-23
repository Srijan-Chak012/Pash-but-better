#include "headers.h"

char *action = "exited";

void child_process_handler(int signum, siginfo_t *info, void *ucontext) {
	int wstatus;
    pid_t id;
	while (1) {
		id = waitpid(-1, &wstatus, WNOHANG | WUNTRACED);
		if (id < 1) return;
		process *currPtr = process_ptr;
		// for (process* currPtr = process_ptr; currPtr != NULL; currPtr = currPtr->next) 
		while (currPtr != NULL){
			if (id != currPtr->pid) 
			{
				currPtr = currPtr->next;
				continue;
			}
			if (WIFEXITED(wstatus) != 0) {
				fprintf(stderr, "%s with pid %d %s normally\n", currPtr->name, id, action);
				deleteProcess(id);
			} else {
				if (WIFSTOPPED(wstatus) == 0 && WIFCONTINUED(wstatus) == 0) {
					fprintf(stderr, "%s with pid %d %s abnormally\n", currPtr->name, id, action);	
					deleteProcess(id);
				}
			}
			break;
			currPtr = currPtr->next;
		}
		prompt_done = true;
		prompt(); 
		fflush(stdout);
		fflush(stderr);
	}
}

void signal_handler(int signum, siginfo_t *info, void *ucontext) {
	// 
	checkJobs();
}

// void initSignal() {
// 	static struct sigaction sigAct, childSig;
//     sigAct.sa_sigaction = signal_handler;
//     childSig.sa_sigaction = child_process_handler;
//     sigAct.sa_flags = SA_RESTART | SA_SIGINFO;
//     childSig.sa_flags = SA_RESTART | SA_SIGINFO;
//     sigemptyset(&sigAct.sa_mask);
// 	sigemptyset(&childSig.sa_mask);
//     sigaction(SIGINT, &sigAct, NULL);
//     sigaction(SIGTSTP, &sigAct, NULL);
// 	sigaction(SIGCHLD, &childSig, NULL);
// }

