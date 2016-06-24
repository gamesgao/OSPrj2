#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct prinfo {
	pid_t	parent_pid;
	pid_t	pid;
	pid_t	first_child_pid;
	pid_t	next_sibling_pid;
	long	state;
	long	uid;
	int depth;
	char 	comm[64];
};

//may cause problem!
int strCompare(char* src, char* dst)
{
	int j = 0;
	int flag = 0;
	while (dst[j] != '\0' && src[j] != '\0')
	{
		if (dst[j] == src[j]) flag = 1;
		else
		{
			flag = 0;
			break;
		}
		j++;
	}
	return flag;
}

int main()
{


	int maxpri;
	struct prinfo buf[100];
	int nr = 100;
	int realnr = 0;
	int i = 0;
	int j = 0;
	pid_t testPid;
	char processName[5] = "main";
	char exceptionProcess[16] = "est.processtest";
	/*
		maxpri=sched_get_priority_max(SCHED_FIFO);

		if(maxpri == -1)
		{
			perror("sched_get_priority_max() failed");
			exit(1);
		}
		param.sched_priority = maxpri;
	*/
	if (!buf)
	{
		printf("the allocation of memory is failed!\n");
		return -1;
	}
	realnr = syscall(356, buf, &nr);
	if (realnr >= 0) printf("success! the real NR is %d\n", realnr);
	else
	{
		printf("the system call return error!\n");
		return -1;
	}
	for (i = 0; i < realnr; i++)
	{
		struct sched_param param;
		sched_getparam(buf[i].pid, &param);
		printf("the current algorithm for %d is %d prio=%d\n",buf[i].pid, sched_getscheduler(buf[i].pid),param.sched_priority );	
	}
	


	return 0;
}

//est.processtest