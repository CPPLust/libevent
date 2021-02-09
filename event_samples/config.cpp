

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



int main(int argc, char * argv[])
{
	//创建一个上下文


	event_config *conf = event_config_new();

	const char ** methods = event_get_supported_methods();
	printf("supported_methods:\n");
	for (int i = 0; methods[i] != NULL; ++i)
	{
		printf("methods[%d] = %s\n", i + 1, methods[i]);
	}
	event_base * base = event_base_new_with_config(conf);
	if (NULL != base)
	{
		printf("event_base_new success...\n");
	}

	event_config_free(conf);

	if (base)
	{
		event_base_free(base);
	}

	
	

	getchar();
	return 0;
}
