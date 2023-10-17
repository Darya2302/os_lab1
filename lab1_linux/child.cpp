#include <iostream>
#include <csignal>
#include <unistd.h>

// Функция-обработчик сигнала
void signalHandler(int signal) {
    std::cout << "Received signal: " << signal << std::endl;
    exit(signal);
}

int main() {
    // Установка обработчика сигнала
    signal(SIGINT, signalHandler);

    std::cout << "Waiting for signal..." << std::endl;

    while (true) {
        // Бесконечный цикл ожидания сигнала
        sleep(1);
    }

    return 0;
}
