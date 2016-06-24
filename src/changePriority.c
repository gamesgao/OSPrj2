/*
 * programed by gaoyu
 * this program is to set test process scheduler to RR with certain prority
 * usage:changePriority <number>
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
//string to number
int charToNum(char *str)
{
	int i=0;
	int result=0;
	while(str[i]!='\0')
	{
		result*=10;
		result+=str[i]-'0';
		i++;
	}
	return result;
}

int main(int argc, char * argv[])
{

	struct sched_param param;
	int maxpri;
	int myPrio=0;
	struct prinfo buf[100];
	int nr = 100;
	int realnr = 0;
	int i = 0;
	int j = 0;
	pid_t testPid;
	char processName[16] = "est.processtest";

	myPrio=charToNum(argv[1]);
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
	//set the test process scheduler to RR with priority myPrio
	param.sched_priority=myPrio;
	printf("%d\n",testPid);
	if (sched_setscheduler(testPid, SCHED_RR, &param) == -1) //设置优先级
	{
		perror("sched_setscheduler() failed");
	}
	return 0;
}