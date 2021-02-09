

#include "event2/event.h"
#include "event2/listener.h"

#include <stdio.h>
#include <signal.h>


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

void EventCtrlC_cb(evutil_socket_t sock, short which, void *ptr)
{
	printf("EventCtrlC_cb.....\n");
}

int main(int argc, char * argv[])
{
	//创建一个上下文
	event_base* base = event_base_new();
	
	//1. add ctrl +c
	event * ctrl_c_event = evsignal_new(base, SIGINT, EventCtrlC_cb, base);
	if (NULL == ctrl_c_event)
	{
		printf("ctrl_c_event is NULL,evsignal_new failed\n ");
		return 0;
	}
	
	//2. add event

	if (event_add(ctrl_c_event, NULL) != 0)
	{
		printf("event_add is ERR\n ");
		return 0;
	}


	event_base_dispatch(base);


	if (ctrl_c_event)
	{
		event_free(ctrl_c_event);
		ctrl_c_event = NULL;
	}
	
	if (base)
	{
		event_base_free(base);
	}

	getchar();
	return 0;
}
