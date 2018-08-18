#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>			 /* For O_* constants */
#include <sys/stat.h> 	   /* For mode constants */
#include <semaphore.h>



#define SEM_FLAG O_CREAT
#define SEM_MODE 00600
#define SEM_1 "sem_1"
#define SEM_2 "sem_2"

sem_t *create_semaphore(char *sem_name, unsigned int value);
int main() 
{
	sem_t *sem1 = create_semaphore(SEM_1, 1);
	sem_t *sem2 = create_semaphore(SEM_2, 0);
	
	int pipefd[2];
	
	pid_t cpid;
	char buf[20] = {0};

	if((pipe(pipefd)) == -1) {
		perror("pipe");
		return -1;
	}

	cpid = fork();
	if(cpid == -1) {
		perror("fork");
		return -1;
	}else if(cpid == 0) {
		printf("00000\n");
		sem_wait(sem1);
		char *str = "hello word";
		memcpy(buf, str, strlen(str));
		printf("11111111\n");
		close(pipefd[0]);
		write(pipefd[1], buf, strlen(buf));
		sem_post(sem2);
		while(1);
		
	}else {
		sem_wait(sem2);
		printf("22222222\n");
		memset(buf, 0, sizeof(buf));
		read(pipefd[0], buf, 5);
		printf("%s \n", buf);
		sem_post(sem1);
		wait(NULL);
	}
	return 0;
}


sem_t *create_semaphore(char *sem_name, unsigned int value)
{
	sem_unlink(sem_name);
	sem_t * sem = sem_open(sem_name, SEM_FLAG, SEM_MODE, value);
    if(SEM_FAILED == sem) {
		perror("sem_open");
		return NULL;
	}
	return sem;

}
