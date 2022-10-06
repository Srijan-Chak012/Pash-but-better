#include "headers.h"

struct passwd *PWD;
uid_t uid;
char user_name[300], host_name[300];
char current_work_dir[4000], home_dir[4000];
char *buff, *delim;
char buff2[4000], buff3[4000];
size_t home_dir_len, buff_len;
long cmd_len;
bool prompt_done;
int STD_INP_FD, STD_OUT_FD;

void sFree(void *__ptr)
{
	if (__ptr != NULL)
		free(__ptr);
}

int main()
{
	// initSignal();
	static struct sigaction sigAct, childSig;
	sigAct.sa_sigaction = signal_handler;
	childSig.sa_sigaction = child_process_handler;
	sigAct.sa_flags = SA_RESTART | SA_SIGINFO;
	childSig.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sigAct.sa_mask);
	sigemptyset(&childSig.sa_mask);
	sigaction(SIGINT, &sigAct, NULL);
	sigaction(SIGTSTP, &sigAct, NULL);
	sigaction(SIGCHLD, &childSig, NULL);
	STD_INP_FD = dup(STDIN_FILENO);
	STD_OUT_FD = dup(STDOUT_FILENO);
	// User_and_System_Name();
	delim = " \t\n";
	getcwd(home_dir, 4000);
	home_dir_len = strlen(home_dir);
	buff_len = 100000UL;

	call_command("clear", false);
	fflush(stdout);
	initialize_history();

	printf("________________________________________________________________________________________\n________________________________________________________________________________________\n________________________██████________██████████__________██████________________________\n______________________████████████████__________██████████████████______________________\n____________________████████████________________________████████████____________________\n__________________████████████____________________________████████████__________________\n__________________██████████________________________________██████████__________________\n__________________████████____________________________________████████__________________\n____________________████________██████____________██████________████____________________\n______________________██______████████____________████████______██______________________\n______________________██____██████__██____________██__██████____██______________________\n______________________██____██████████____________██████████____██______________________\n______________________██____██████████__████████__██████████____██______________________\n________________________██__████████____████████____████████__██________________________\n________________________██____████____________________████____██________________________\n__________________________██________________________________██__________________________\n");
	printf(""RED"                         $> Welcome to PASH, the Panda Shell!<$\n" RESET);
	printf("\n\n");

	while (true)
	{
		if (prompt_done == false)
			prompt();
		prompt_done = false;
		buff = NULL;
		cmd_len = getline(&buff, &buff_len, stdin);
		if ((cmd_len < 0 || strlen(buff) == 0) && feof(stdin))
		{
			puts("");
			if (errno == EINTR)
			{
				continue;
			}
			sFree(buff);
			return 0;
		}
		if (cmd_len < 0)
		{
			perror("");
			exit(0);
		}
		call_command(buff, true);
		sFree(buff);
	}
	return 0;
}
