#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <jmorecfg.h>


void signalHandler(int signal) {
    if (signal == SIGINT) {
        printf("\nSIGINT %d", signal);
    }
    else if(signal == SIGTERM)
    {
        printf("\nSIGTERM %d", signal);
    }
    else{
        printf("\n%d", signal);
    }
    exit(0);
}

void* printRandomNumber(void*) {
    while (TRUE) {
        printf("%f\n", (double)rand() / RAND_MAX);
        sleep(3);
    }
}

void* reversString(void*) {
    while (TRUE) {
        char str[250];
        scanf("%s", str);
        for (int i = strlen(str) - 1; i >= 0; --i) {
            printf("%c", str[i]);
        }
        printf("\n");
    }
}

int main() {
    pthread_t thread1;
    pthread_t thread2;
    int first;
    int second;
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    first = pthread_create(&thread1, NULL, printRandomNumber, NULL);
    second = pthread_create(&thread2, NULL, reversString, NULL);
    while (TRUE){}
    return 0;
}