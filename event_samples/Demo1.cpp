

#include "event2/event.h"
#include "event2/listener.h"

#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
class CWinSockInitializer
{
public:
	CWinSockInitializer()
	{
#if defined(_WIN32)
		WORD wVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		::WSAStartup(wVersion, &wsaData);
#endif
	}
	~CWinSockInitializer()
	{
#if defined(_WIN32)
		::WSACleanup();
#endif
	}
};
CWinSockInitializer WinSockInitializer;


 void listener_ck(struct evconnlistener *e, evutil_socket_t s, struct sockaddr *addr, int socklen, void *ptr)
{
	printf("Comming a client..\n");
	closesocket(s);
}


int main(int argc, char * argv[])
{
	//创建一个上下文
	event_base* base = event_base_new();
	if (NULL != base)
	{
		printf("event_base_new success...\n");
	}


	//创建socket 绑定监听

	sockaddr_in  addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET,
		addr.sin_port = htons(8000);

	evconnlistener *evListener = evconnlistener_new_bind(
		base,
		listener_ck, 
		NULL,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
		10,
		(const struct sockaddr *)&addr,
		sizeof(addr)	
		);

	//LEV_OPT_CLOSE_ON_FREE释放掉 evListener会关闭socket
	//LEV_OPT_REUSEABLE `socket`重用

	if (NULL == evListener)
	{
		printf("evconnlistener_new_bind error..\n");
	}

	event_base_dispatch(base);

	if (evListener)
	{
		evconnlistener_free(evListener);
		evListener = NULL;
	}


	if (base)
	{
		event_base_free(base);
		base = NULL;
	}
	

	getchar();
	return 0;
}
