#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_MESSAGE_LENGTH 20

using namespace std;

volatile sig_atomic_t sender_ready = 0;

void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        sender_ready++;
    }
}

void wait_for_senders(int count) {
    while (sender_ready < count) {
        pause();
    }
}

int main() {
    string filename;
    int record_count;

    cout << "Enter binary file name: ";
    cin >> filename;

    cout << "Enter number of records: ";
    cin >> record_count;

    ofstream file(filename.c_str(), ios::binary | ios::trunc);
    if (!file) {
        cerr << "Failed to create file." << endl;
        return 1;
    }

    for (int i = 0; i < record_count; ++i) {
        char empty[MAX_MESSAGE_LENGTH] = { 0 };
        file.write(empty, MAX_MESSAGE_LENGTH);
    }
    file.close();

    int sender_count;
    cout << "Enter number of Sender processes: ";
    cin >> sender_count;

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    vector<pid_t> senders;

    for (int i = 0; i < sender_count; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./sender", "./sender", filename.c_str(), NULL);
            perror("execl failed");
            return 1;
        }
        else if (pid > 0) {
            senders.push_back(pid);
        }
        else {
            cerr << "Failed to fork." << endl;
            return 1;
        }
    }

    wait_for_senders(sender_count);
    cout << "All Sender processes are ready." << endl;

    string command;
    while (true) {
        cout << "Enter command (read/exit): ";
        cin >> command;

        if (command == "read") {
            ifstream infile(filename.c_str(), ios::binary);
            if (!infile) {
                cerr << "Failed to open file." << endl;
                continue;
            }

            char message[MAX_MESSAGE_LENGTH] = { 0 };
            bool found = false;

            for (int i = 0; i < record_count; ++i) {
                infile.read(message, MAX_MESSAGE_LENGTH);
                if (message[0] != 0) {
                    cout << "Message: " << message << endl;
                    found = true;

                    ofstream outfile(filename.c_str(), ios::binary | ios::in);
                    outfile.seekp(i * MAX_MESSAGE_LENGTH, ios::beg);
                    char empty[MAX_MESSAGE_LENGTH] = { 0 };
                    outfile.write(empty, MAX_MESSAGE_LENGTH);
                    outfile.close();

                    break;
                }
            }

            if (!found) {
                cout << "No new messages." << endl;
            }

            infile.close();
        }
        else if (command == "exit") {
            break;
        }
        else {
            cout << "Unknown command." << endl;
        }
    }

    for (pid_t pid : senders) {
        kill(pid, SIGTERM);
        waitpid(pid, NULL, 0);
    }

    return 0;
}
