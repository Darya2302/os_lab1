#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) {
        std::cerr << "Failed to fork process." << std::endl;
        return 1;
    } else if (pid == 0) {
        // Дочерний процесс
        std::cout << "Child process created." << std::endl;
        sleep(5); // Для демонстрации, что основной процесс ждет сигнала о завершении
        exit(0); // Завершаем дочерний процесс
    } else {
        // Основной процесс
        std::cout << "Main process created." << std::endl;

        // Ждем завершения дочернего процесса
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            std::cout << "Child process exited normally with exit code: " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cout << "Child process exited due to signal: " << WTERMSIG(status) << std::endl;
        } else if (WIFSTOPPED(status)) {
            std::cout << "Child process stopped due to signal: " << WSTOPSIG(status) << std::endl;
        } else {
            std::cout << "Child process exited abnormally." << std::endl;
        }
    }

    return 0;
}