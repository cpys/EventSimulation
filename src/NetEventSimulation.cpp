//
// Created by yingzi on 2017/12/6.
//

#include <netinet/in.h>
#include <string>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

const string DEFAULT_SERVER_IP = "10.205.41.48";
const uint DEFAULT_SERVER_PORT = 5556;

const uint SLEEP_TIME = 10240;
const uint TIME_ALL = 5000000;

const uint EVENT_ALL = 100;
const uint EVENT_LENGTH = 100;
const double IMPORTANT_EVENT_PRO = 0.0;

string serverIp;
uint serverPort;

int clientSocket;
struct sockaddr_in serverAddr;

char eventBuffer[10000];
char eventName[100];
char eventImportant[10];

clock_t startTime, endTime;
uint usedTime;

char events[EVENT_ALL + 1][EVENT_LENGTH + 1];

void constructStaticEvent() {
    string eventHead, eventTail, event;
    for (uint i = 1; i <= EVENT_ALL; ++i) {
        eventHead = "<xml type=\"event\" name=\"increase\" important=\"0\" num=\"" + to_string(i) + "\" attr=\"";
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

        if (sendto(clientSocket, events[eventNum], EVENT_LENGTH, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
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
        serverIp = DEFAULT_SERVER_IP;
        serverPort = DEFAULT_SERVER_PORT;
    }
    else if (argc == 2) {
        serverIp = argv[1];
        serverPort = DEFAULT_SERVER_PORT;
    }
    else if (argc == 3) {
        serverIp = argv[1];
        serverPort = stoi(argv[2]);
    }
    else {
        cerr << "Usage: ./EventSimulation" << " [server_ip] [server_port]" << endl;
        return -1;
    }
    cout << "server ip is " << serverIp << ", port is " << serverPort << endl;

    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        cerr << "create client socket failed!" << endl;
        return -1;
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    serverAddr.sin_port = htons(serverPort);

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

