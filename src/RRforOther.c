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



		if (strCompare(buf[i].comm, processName) == 1)
		{
			testPid = buf[i].pid;
			printf("%d\n", testPid);
			struct sched_param param;
			sched_getparam(testPid, &param);
			if (sched_setscheduler(testPid, SCHED_RR, &param) == -1) //设置优先级
			{
				perror("sched_setscheduler() failed");
			}
			break;
		}
		//for(j=0;j<buf[i].depth;j++) printf("\t");
		printf("%s,%d,%ld,%d,%d,%d,%ld\n", buf[i].comm, buf[i].pid, buf[i].state, buf[i].parent_pid, buf[i].first_child_pid, buf[i].next_sibling_pid, buf[i].uid);
	}
	for (i++; i < realnr; i++)
	{
		if (buf[testPid].depth < buf[i].depth)
		{
			if (strCompare(buf[i].comm, exceptionProcess) == 0)
			{
				struct sched_param param;
				sched_getparam(buf[i].pid, &param);
				if (sched_setscheduler(buf[i].pid, SCHED_RR, &param) == -1) //设置优先级
				{
					perror("sched_setscheduler() failed");
				}
			}
			else
			{
				struct sched_param param;
				sched_getparam(buf[i].pid, &param);
				if (sched_setscheduler(buf[i].pid, SCHED_NORMAL, &param) == -1) //设置优先级
				{
					perror("sched_setscheduler() failed");
				}
			}
		}
		else	break;
	}



	return 0;
}

//est.processtest