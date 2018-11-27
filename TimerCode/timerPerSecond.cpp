/*
* 运行程序后, 每隔1秒, 调用onTime函数,打印Ding...
*/

#include <iostream>
#include <event.h>
using namespace std;
static struct event_base *base;
void onTime(int sock, short event, void *arg)
{
    cout << "Ding..." << endl;
    struct timeval *tmp = (struct timeval*)arg;
    struct event *tmpTimer = evtimer_new(base, onTime, arg);
    event_add(tmpTimer, tmp);
}

int main(int argc, char **argv)
{
    struct event *timer;
    struct timeval tv;

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    base = event_base_new();
    //传入tv做为参数
    timer = evtimer_new(base, onTime, (void*)&tv);

    event_add(timer, &tv);
    event_base_dispatch(base);
    return 0;
}