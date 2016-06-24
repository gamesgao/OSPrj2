/*
 * programed by gaoyu
 * this program is to set all the descendants of zygote scheduler to RR
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

//may cause problem!
//a function to compare the string equation or not
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

//set the process to sched_RR
int setProcessToRR(pid_t pid,int prio)
{
	struct sched_param param;
	printf("%d\n", pid);
	param.sched_priority=prio;
	if (sched_setscheduler(pid, SCHED_RR, &param) == -1) //设置优先级
	{
		perror("sched_setscheduler() failed");
	}
	return 0;
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
	//find the zygote process
	for (i = 0; i < realnr; i++)
	{
		if (strCompare(buf[i].comm, processName) == 1)
		{
			testPid = buf[i].pid;
			printf("%d\n", testPid);
			break;
		}
	}
	//set the descendent's scheduler to RR
	for (i=i+1; i < realnr; i++)
	{
		if (1 < buf[i].depth)
		{

			if (strCompare(buf[i].comm, exceptionProcess) == 0)
			{
				setProcessToRR(buf[i].pid,99);
			}
			else
			{	
				setProcessToRR(buf[i].pid,1);
			}
		}
		else	break;
	}
	return 0;
}