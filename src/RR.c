/*
 * programed by gaoyu
 * this program is to set test process scheduler to RR
*/

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
	if (!buf)
	{
		printf("the allocation of memory is failed!\n");
		return -1;
	}
	//syscall
	realnr = syscall(356, buf, &nr);
	if (realnr >= 0) printf("success! the real NR is %d\n", realnr);
	else
	{
		printf("the system call return error!\n");
		return -1;
	}
	//find the test process
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
	}
	//set the test process scheduler to RR
	param.sched_priority=1;
	printf("%d\n",testPid);
	if (sched_setscheduler(testPid, SCHED_RR, &param) == -1) //设置优先级
	{
		perror("sched_setscheduler() failed");
	}
	return 0;
}
