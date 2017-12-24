//
// Created by chenkuan on 17-12-24.
//

#include <iostream>
#include <csignal>
#include "SerialPortClient/SerialPortClient.h"
using namespace std;

const string DEFAULT_PORT_PATH = "/dev/ttyUSB0";

SerialPortClient serialPortClient;
int recvLen;

void quit(int signum) {
    serialPortClient.closePort();
    cout << "recv len = " << recvLen << endl;
    exit(0);
}

int main() {
    serialPortClient.setPort(DEFAULT_PORT_PATH);
    if (!serialPortClient.init()) {
        cerr << "init serial port failed!" << endl;
        return -1;
    }

    signal(SIGINT, quit);

    recvLen = 0;
    while (true) {
        serialPortClient.getMessage();
        string &messageQueue = serialPortClient.getMessageQueue();
        recvLen += messageQueue.size();
        cout << messageQueue << endl;
        messageQueue.clear();
    }

    return 0;
}