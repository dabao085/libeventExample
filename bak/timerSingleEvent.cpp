#include <iostream>
#include <event.h>
using namespace std;

void onTime(int sock, short event, void *arg)
{
    cout << "3 seconds elapse" << endl;
    cout << "Ding..." << endl;
}

int main(int argc, char **argv)
{
    struct event timer;
    struct timeval tv;

    //3秒后调用回调函数
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    event_init();   //废弃版本
    evtimer_set(&timer, onTime, (void*)&timer);
    event_add(&timer, &tv);

    event_dispatch();//废弃版本
    return 0;
}