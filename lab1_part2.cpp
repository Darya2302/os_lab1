﻿#include <iostream>
#include <Windows.h>
#include <signal.h>
#include <csignal>

int main()
{
    HANDLE hChildProcess;
    PROCESS_INFORMATION pi;
    DWORD exitCode;

    // Создание процесса
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(L"C:/Users/Dasha/data/os/Child/Child/Child.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))
    {
        std::cerr << "Failed to create process" << std::endl;
        return 1;
    }

    // Ожидание завершения процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Получение кода завершения процесса
    GetExitCodeProcess(pi.hProcess, &exitCode);
    std::cout << "The process terminated with the following code: " << exitCode << std::endl;

    // Закрытие дескрипторов процесса и потока
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
