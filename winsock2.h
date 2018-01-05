#include <winsock2.h>

typedef struct
{
	WSADATA wsadata;
	SOCKET socket;
} nw_stuff;
