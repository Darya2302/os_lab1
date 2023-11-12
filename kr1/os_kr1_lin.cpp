#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class BankAccount {
private:
    double balance;
    mutex mtx;
    condition_variable cv;
    bool isAvailable;

public:
    BankAccount(double initialBalance) : balance(initialBalance), isAvailable(true) {}

    double getBalance() {
        unique_lock<mutex> lock(mtx);
        return balance;
    }

    void changeBalance(double amount) {
        unique_lock<mutex> lock(mtx);
        while (!isAvailable) {
            cv.wait(lock);
        }

        isAvailable = false;
        this_thread::sleep_for(chrono::milliseconds(50)); // Simulate delay

        int maxAttempts = 3;
        int attempts = 0;

        while (attempts < maxAttempts) {
            if (balance + amount >= 0) {
                double currentBalance = balance;
                balance += amount;
                std::cout << "was=" << currentBalance << ", operation " << (amount > 0 ? '+' : '-') << abs(amount) << ", now=" << balance << endl;
                break;
            }
            else {
                attempts++;
                std::cout << "Operation " << (amount > 0 ? "+" : "") << amount << " temporarily not possible. Attempt " << attempts << " from " << maxAttempts << std::endl;
            }
        }

        isAvailable = true;
        cv.notify_all();
    }
};

void modifyBalance(BankAccount& account, const vector<double>& changes) {
    for (double change : changes) {
        account.changeBalance(change);
    }
}

int main(int argc, char** argv) {
    // Контроль количества операций и их значений через флаги командной строки
    vector<double> changes;
    if (argc > 1) {
        int numChanges = atoi(argv[1]);
        for (int i = 2; i < argc; i++) {
            double change = atof(argv[i]);
            changes.push_back(change);
        }

        if (changes.size() < numChanges) {
            cerr << "Quantity of operations is less" << endl;
            return 1;
        }
        else if (changes.size() > numChanges) {
            cerr << "Quantity of operations is more" << endl;
            return 1;
        }
    }
    else {
        changes = { -120, 10, 15, -40, -30, 90 }; // Пример сценария операций
    }

    BankAccount account(100);
    thread t(modifyBalance, ref(account), ref(changes));

    // Проверка счета пользователя
    this_thread::sleep_for(chrono::seconds(1)); // Задержка перед выводом состояния счета
    cout << "Balance: " << account.getBalance() << endl;

    t.join();

    return 0;
}
