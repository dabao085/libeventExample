#include <iostream>
#include <signal.h>
#include <event.h>
using namespace std;

int called = 0;

static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
    struct event* tmp = (struct event*)arg;
    cout << "called: " << called << endl;
    if(called > 2){
        cout << "called times is greater than 2, excute event_del..." << endl;
        event_del(tmp);
    }
    ++called;
}

int main(int argc, char** argv)
{
    struct event* evSignal;
    struct event_base* base;

    base = event_base_new();
    evSignal = evsignal_new(base, SIGINT, signal_cb, event_self_cbarg());
    event_add(evSignal, NULL);
    event_base_dispatch(base);

    event_free(evSignal);
    event_base_free(base);
    return 0;
}