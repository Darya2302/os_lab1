#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include<processthreadsapi.h>
#include <Windows.h>

// Флаг для завершения работы потоков
bool exitFlag = false;

// Функция потока, которая пишет случайное число через определенный интервал
void* threadFunc(void* arg) {
    double interval = *(double*)arg;

    while (!exitFlag) {
        double randomNum = (double)rand() / RAND_MAX;
        std::cout << randomNum << std::endl;
        sleep(1300);
    }

    pthread_exit(NULL);
}

// Обработчик сигнала SIGINT
void sigintHandler(int signum) {
    std::cout << "Received SIGINT signal. Exiting..." << std::endl;
    exitFlag = true;
}

void reversString() {
    while (TRUE) {

        char str[250];
        scanf_s("%s", str, 250);
        printf("%s\n", _strrev(str));
    }
}

int main() {

    // Создаем поток
    pthread_t thread;
    pthread_create(&thread, NULL, threadFunc, NULL, 0, NULL);
    pthread_create(&thread, NULL, reversString, NULL, 0, NULL);

    // Устанавливаем обработчик сигнала SIGINT
    signal(SIGINT, sigintHandler);

    // Ожидаем завершение работы потока
    pthread_join(thread, NULL);

    return 0;
}
