//
// Created by yingzi on 2017/12/6.
//

#include <netinet/in.h>
#include <string>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>
#include <vector>

using namespace std;

const string DEFAULT_SERVER_IP = "10.108.164.222";
const uint DEFAULT_SERVER_PORT = 5556;

const uint SLEEP_TIME = 1024000;
const uint SLEEP_EVENT_NUM = 1;

const uint EVENT_ALL = 1;
const uint EVENT_LENGTH = 100;
const double IMPORTANT_EVENT_PRO = 0.0;

vector<string> IMPORTANT_EVENT_NAME_LIST = {
        "increase"
};

string serverIp;
uint serverPort;

int clientSocket;
struct sockaddr_in serverAddr;

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

    for (uint eventNum = 1; eventNum <= EVENT_ALL ; ++eventNum) {
        if (sendto(clientSocket, events[eventNum], EVENT_LENGTH, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
            ++sendFailedNum;
            printf("send event %s failed\n", events[eventNum]);
        }
        //else {
//            printf("send event %s success\n", events[eventNum]);
        //}
        ++sendNum;
        if (eventNum % SLEEP_EVENT_NUM == 0) {
            usleep(SLEEP_TIME * SLEEP_EVENT_NUM);
        }
    }
    cout << "success send event num:" << sendNum - sendFailedNum << "/" << sendNum << endl;
}

void quit(int signum) {
    cout << "success send event num:" << sendNum - sendFailedNum << "/" << sendNum << endl;
    exit(0);
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

    srand((unsigned)time(NULL));

    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        cerr << "create client socket failed!" << endl;
        return -1;
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    serverAddr.sin_port = htons(serverPort);

    signal(SIGINT, quit);

    constructStaticEvent();
    sendStaticEvent();

    return 0;

}

