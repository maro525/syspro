#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define MAXLOOP 200

void *thread_func(int *i)
{
    int pid;
    pthread_t thread_id;

    while(*i <= MAXLOOP)
    {
        *i += 1;
        fprintf(stderr, "%d ", *i);
        // sleep(1);
    }
}

int main(int argc, char **argv)
{
    pthread_t thread;
    int pid;
    int i;

    pid = getpid();
    printf("main pid : %d\n", pid);
    if (pthread_create(&thread, NULL, thread_func, &i) != 0)
        perror("pthread_create()");
    
    i = 0;
    while(i <= MAXLOOP)
    {
        i++;
        fprintf(stderr, "%d ", i);\
        // sleep(1);
    }
    
    pthread_join(thread_func, NULL);
    return 0;
}