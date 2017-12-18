//
// Created by yingzi on 2017/12/6.
//

#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "SerialPortClient/SerialPortClient.h"

using namespace std;

const string DEFAULT_PORT_PATH = "/dev/ttyS0";

const uint SLEEP_TIME = 1024000;
const uint TIME_ALL = 50000000;

const uint EVENT_ALL = 10000;
const uint EVENT_LENGTH = 100;
const double IMPORTANT_EVENT_PRO = 0.5;

string portPath;

SerialPortClient serialPortClient;

char eventBuffer[10000];
char eventName[100];
char eventImportant[10];

clock_t startTime, endTime;
uint usedTime;

char events[EVENT_ALL + 1][EVENT_LENGTH + 1];

string getEventImportant() {
    if (rand() > RAND_MAX * IMPORTANT_EVENT_PRO) return "0";
    else return "1";
}

void constructStaticEvent() {
    string eventHead, eventTail, event;
    for (uint i = 1; i <= EVENT_ALL; ++i) {
        eventHead = "<xml type=\"event\" name=\"increase\" important=\"" + getEventImportant() + "\" num=\"" + to_string(i) + "\" attr=\"";
        eventTail = "\"><x>" + to_string(i % 100) + "</x></xml>";
        event = eventHead + string(EVENT_LENGTH - eventHead.size() - eventTail.size(), '*') + eventTail;
        strcpy(events[i], event.c_str());
    }
}

void sendStaticEvent() {
    uint sendFailedNum = 0;
    uint sendNum = 0;
    usedTime = 0;
    for (uint eventNum = 1; ; ++eventNum) {
        if (usedTime > TIME_ALL) {
            cout << "time out" << endl;
            break;
        }
        if (eventNum > EVENT_ALL) {
            cout << "event send over" << endl;
            break;
        }

        if (!serialPortClient.sendMessage(events[eventNum])) {
            ++sendFailedNum;
            printf("send event %s failed\n", events[eventNum]);
        }
        else {
//            printf("send event %s success\n", events[eventNum]);
        }
        ++sendNum;
        usleep(SLEEP_TIME);
        usedTime = (uint)((double)(clock() - startTime) / CLOCKS_PER_SEC * 1000000) + SLEEP_TIME * eventNum;
    }
    cout << "success send event num:" << sendNum - sendFailedNum << "/" << sendNum << endl;
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

    srand((unsigned)time(NULL));

    serialPortClient.setPort(portPath);
    if (!serialPortClient.init()) {
        cerr << "init serial port failed!" << endl;
        return -1;
    }

    constructStaticEvent();
    sendStaticEvent();

//    usedTime = 0;
//    startTime = clock();
//    for (uint eventNum = 1; ; ++eventNum) {
//        if (usedTime > TIME_ALL) break;
//        if (eventNum > EVENT_ALL) break;
//
//        strcpy(eventBuffer, R"border(<xml type="event" ")border");
//        getEventName();
//        strcat(eventBuffer, eventName);
//        getEventImportant();
//        strcat(eventBuffer, eventImportant);
//
//        strcat(eventBuffer, R"border(eventNum=")border");
//        strcat(eventBuffer, to_string(eventNum).c_str());
//
//
//
//        usleep(SLEEP_TIME);
//
//        usedTime = (uint)((double)(clock() - startTime) / CLOCKS_PER_SEC * 1000000) + SLEEP_TIME * eventNum;
//    }

    return 0;

}

