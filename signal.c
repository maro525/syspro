#include <stdio.h>
#include <signal.h>

void func(int signo)
{
    printf("signo = %d\n", signo);
}

void func1(int signo)
{
    printf("hello this is func 1. signo = %d\n", signo);
}

void func2(int signo)
{
    printf("hello this is func 2. signo = %d\n", signo);
}

int main(int argc, char **argv)
{
    signal(SIGINT, func);
    signal(SIGUSR1, func1);
    signal(SIGUSR2, func2);
    while(1);
} 