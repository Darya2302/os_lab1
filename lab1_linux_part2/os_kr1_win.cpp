#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool accountAvailable = true;
std::vector<double> balanceChangeScenario = { 40, -30, 90, -150, 30, -100 };

double accountBalance = 100;
std::vector<double> operations;

void changeBalance(double amount) {//изменяет баланс
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::lock_guard<std::mutex> lock(mtx);
    accountBalance += amount;
    operations.push_back(amount);
}

bool isValidChange(double amount) { // проверяет возможность проведения операции
    std::lock_guard<std::mutex> lock(mtx);
    return accountBalance + amount >= 0;
}

void performOperation(double amount) {
    int maxAttempts = 3;
    int attempts = 0;
    while (attempts < maxAttempts) {
        if (isValidChange(amount)) {
            changeBalance(amount);
            std::cout << "was=" << accountBalance - amount << ", operation " << (amount > 0 ? "+" : "") << amount << ", now=" << accountBalance << std::endl;
            break;
        }
        else {
            attempts++;
            std::cout << "Operation " << (amount > 0 ? "+" : "") << amount << " temporarily not possible. Attempt " << attempts << " from " << maxAttempts << std::endl;
            //

            /*int k = size(balanceChangeScenario);
            double t = balanceChangeScenario[0];
            for (int i = 0; i < k - 1; i++) {
                balanceChangeScenario[i] = balanceChangeScenario[i + 1];
            }
            balanceChangeScenario[k - 1] = t;
            */

        }
    }
}

void runScenario(const std::vector<double>& scenario) {//блокирует счет при операции
    for (double amount : scenario) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return accountAvailable; });

        accountAvailable = false;
        lock.unlock();

        performOperation(amount);

        lock.lock();
        accountAvailable = true;
        lock.unlock();
        cv.notify_all();
    }
}

void printAccountBalance() {
    //while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::lock_guard<std::mutex> lock(mtx);
        double currentBalance = accountBalance;
        std::cout << "Current account balance: " << currentBalance << std::endl;
    //}
}

int main() {
    std::thread scenarioThread(runScenario, balanceChangeScenario);
    std::thread balanceThread(printAccountBalance);

    scenarioThread.join();
    balanceThread.join();

    return 0;
}
