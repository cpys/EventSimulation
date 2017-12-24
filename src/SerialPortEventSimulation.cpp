//
// Created by yingzi on 2017/12/6.
//

#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <csignal>
#include "SerialPortClient/SerialPortClient.h"

using namespace std;

const string DEFAULT_PORT_PATH = "/dev/ttyS0";

const uint SLEEP_TIME = 1024000;

const uint EVENT_ALL = 10000;
const uint EVENT_LENGTH = 100;
const double IMPORTANT_EVENT_PRO = 0.0;

vector<string> IMPORTANT_EVENT_NAME_LIST = {
        "increase"
};

string portPath;
SerialPortClient serialPortClient;

uint sendNum;
uint sendFailedNum;

char events[EVENT_ALL + 1][EVENT_LENGTH + 1];

void quit(int);

string getEventName() {
    if (rand() > RAND_MAX * IMPORTANT_EVENT_PRO) {
        return "notImportantEventName";
    }
    else {
        return IMPORTANT_EVENT_NAME_LIST[rand() % IMPORTANT_EVENT_NAME_LIST.size()];
    }
}

void constructStaticEvent() {
    string eventHead, eventTail, event;
    for (uint i = 1; i <= EVENT_ALL; ++i) {
        eventHead = "<xml type=\"event\" name=\"" + getEventName() + "\" num=\"" + to_string(i) + "\" attr=\"";
        eventTail = "\"><x>" + to_string(i % 100) + "</x><y>" + to_string(i % 50) + "</y></xml>";
        event = eventHead + string(EVENT_LENGTH - eventHead.size() - eventTail.size(), '*') + eventTail;
        strcpy(events[i], event.c_str());
    }
}

void sendStaticEvent() {
    sendFailedNum = 0;
    sendNum = 0;
    printf("start send events\n");

    for (uint eventNum = 1; eventNum <= EVENT_ALL; ++eventNum) {
        if (!serialPortClient.sendMessage(events[eventNum])) {
            ++sendFailedNum;
            printf("send event failed:%s\n", events[eventNum]);
        }
        else {
            printf("send event success:%s\n", events[eventNum]);
        }
        ++sendNum;
        usleep(SLEEP_TIME);
    }
    cout << "success send event num:" << sendNum - sendFailedNum << "/" << sendNum << endl;
}

void quit(int signum) {
    cout << "success send event num:" << sendNum - sendFailedNum << "/" << sendNum << endl;
    serialPortClient.closePort();
    exit(0);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        portPath = DEFAULT_PORT_PATH;
    }
    else if (argc == 2) {
        portPath = argv[1];
    }
    else {
        cerr << "Usage: ./EventSimulation" << " [port_path]" << endl;
        return -1;
    }
    cout << "port path is " << portPath << endl;

    serialPortClient.setPort(portPath);
    if (!serialPortClient.init()) {
        cerr << "init serial port failed!" << endl;
        return -1;
    }

    signal(SIGINT, quit);

    constructStaticEvent();
    sendStaticEvent();

    return 0;

}

