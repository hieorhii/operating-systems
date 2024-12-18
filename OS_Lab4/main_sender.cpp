#include <iostream>
#include <fstream>
#include <string>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

#define MAX_MESSAGE_LENGTH 20

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    string filename = argv[1];

    kill(getppid(), SIGUSR1);

    string command;
    while (true) {
        cout << "Enter command (send/exit): ";
        cin >> command;

        if (command == "send") {
            cout << "Enter message (max " << MAX_MESSAGE_LENGTH - 1 << " chars): ";
            string message;
            cin.ignore();
            getline(cin, message);

            if (message.size() >= MAX_MESSAGE_LENGTH) {
                cerr << "Message too long!" << endl;
                continue;
            }

            fstream file(filename.c_str(), ios::binary | ios::in | ios::out);
            if (!file) {
                cerr << "Failed to open file." << endl;
                continue;
            }

            char buffer[MAX_MESSAGE_LENGTH] = { 0 };
            bool written = false;

            for (int i = 0; !file.eof(); ++i) {
                file.read(buffer, MAX_MESSAGE_LENGTH);
                if (buffer[0] == 0) {
                    file.seekp(i * MAX_MESSAGE_LENGTH, ios::beg);
                    file.write(message.c_str(), message.size());
                    written = true;
                    break;
                }
            }

            file.close();

            if (written) {
                cout << "Message sent successfully." << endl;
            }
            else {
                cout << "File is full, waiting..." << endl;
                sleep(1);
            }
        }
        else if (command == "exit") {
            break;
        }
        else {
            cout << "Unknown command." << endl;
        }
    }

    return 0;
}
