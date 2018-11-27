#include <iostream>
#include <string>
#include <cassert>
#include <string>
#include <string.h>
#include <event.h>
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>    
#include <unistd.h>

using namespace std;
struct event_base *base;
char buf[1024];

//向客户端发回接收到的数据
void onWrite(int iCliFd, short iEvent, void *arg)
{
    cout << "ready to write back..." << endl;
    size_t count = write(iCliFd, buf, strlen(buf));
    if(count <= 0){
        cout << "write error..." << endl;
    }else{
        cout << "write " << count << " characters..." << endl;
        memset(buf, 0, sizeof(buf));
    }
    struct event *tmp = (struct event*)arg;
    event_del(tmp);
    cout << endl;
}

//读取客户端发送的数据
void onRead(int iCliFd, short iEvent, void *arg)
{
    size_t count;
    assert(arg != NULL);
    memset(buf, 0, 1024);
    count = read(iCliFd, buf, 1024);
    if(count == 0){
        cout << "Client [" << iCliFd << "] close..." << endl;
        struct event *tmp = (struct event*)arg;
        event_del(tmp);
        return ;
    }else{
        buf[count] = '\0';
        cout << "Client[" << iCliFd << "]: " << buf;
    }

    //添加写事件
    struct event *evWrite = event_new(base, iCliFd, EV_WRITE|EV_PERSIST, onWrite, event_self_cbarg());
    event_add(evWrite, NULL);
}

void onAccept(int iCliFd, short iEvent, void *arg)
{
    evutil_socket_t sockConn;
    struct sockaddr_in addr;
    socklen_t sockLength = sizeof(addr);
    sockConn = accept(iCliFd, (struct sockaddr*)&addr, &sockLength);
    
    if(sockConn < 0){
        cout << "accept error..." << endl;
        return ;
    }

    //有新的连接, 对其进行读操作
    struct event *evRead = event_new(base, sockConn, EV_READ|EV_PERSIST, onRead, event_self_cbarg());
    event_add(evRead, NULL);
}

int main(int argc, char **argv)
{
    evutil_socket_t sockfd;
    struct sockaddr_in servAddr;

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(8888);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr));
    listen(sockfd, 10);

    base = event_base_new();
    struct event *evAccept = event_new(base, sockfd, EV_READ|EV_PERSIST, onAccept, NULL);
    event_add(evAccept, NULL);
    event_base_dispatch(base);

    event_free(evAccept);
    event_base_free(base);

    return 0;
}