#include <stdio.h>  
#include <string.h>   
#include <event2/event.h>  
 
int main() {  
    puts("init a event_base!");  
    struct event_base *base; //定义一个event_base  
    base = event_base_new(); //初始化一个event_base  

    const char *x =  event_base_get_method(base); //查看用了哪个IO多路复用模型，linux一下用epoll  
    printf("METHOD:%s\n", x);  

    int y = event_base_dispatch(base); //事件循环。因为我们这边没有注册事件，所以会直接退出  
    event_base_free(base);  //销毁libevent  

    return 0;
}  
