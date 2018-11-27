/*
* 运行程序后, 等待3秒, 调用onTime函数
*/

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
    struct event *timer;
    struct event_base *base;
    struct timeval tv;

    //3秒后调用回调函数
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    base = event_base_new();
    timer = evtimer_new(base, onTime, NULL);

    event_add(timer, &tv);
    event_base_dispatch(base);
    return 0;
}