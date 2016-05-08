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

int main()
{

	struct sched_param param;
	int maxpri;
	struct prinfo buf[100];
	int nr = 100;
	int realnr = 0;
	int i = 0;
	int j = 0;
	pid_t testPid;
	char processName[16] = "est.processtest";
	printf("max prio for RR is %d\n"	,sched_get_priority_max(SCHED_RR));
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

		j = 0;
		int flag = 0;
		while (buf[i].comm[j] != '\0')
		{
			if (buf[i].comm[j] == processName[j]) flag = 1;
			else
			{
				flag = 0;
				break;
			}
			j++;
		}
		if (flag == 1)
		{
			testPid = buf[i].pid;
			break;
		}
		//for(j=0;j<buf[i].depth;j++) printf("\t");
		printf("%s,%d,%ld,%d,%d,%d,%ld\n", buf[i].comm, buf[i].pid, buf[i].state, buf[i].parent_pid, buf[i].first_child_pid, buf[i].next_sibling_pid, buf[i].uid);
	}

	//fork();
	printf("the current pid is %d\n", testPid);
	printf("the return value of getparam is %d\n",sched_getparam(testPid, &param));
	printf("the param is %d\n", param.sched_priority);
	param.sched_priority=58;
	printf("the current prio is %d\n", param.sched_priority);
	printf("the current algorithm is %d\n", sched_getscheduler(testPid));
	if (sched_setscheduler(testPid, SCHED_RR, &param) == -1) //设置优先级
	{
		perror("sched_setscheduler() failed");
	}

	return 0;
}

//est.processtest