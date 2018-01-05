#include "winsock2.h"
#include "networking.h"

#include <stdio.h> // stderr, fputs, fprintf
#include <winsock2.h>


int nw_connect
(
 nw_stuff * const stuff,
 IPv4address ip_address,
 Port port
)
{
	if ( WSAStartup(MAKEWORD(2,2), &(stuff->wsadata)) == 0 )
	{
		fputs("info: successfully initialized Winsock DLL\n", stderr);
		
		stuff->socket = socket(AF_INET, SOCK_STREAM, 0);
		if ( stuff->socket != INVALID_SOCKET )
		{
			fputs("info: successfully created socket\n", stderr);
			
			struct sockaddr_in server_address;
			
			server_address.sin_addr.s_addr = htonl(ip_address);
			server_address.sin_family = AF_INET;
			server_address.sin_port = htons(port);
			
			if ( connect(stuff->socket, (struct sockaddr *)&server_address, sizeof(server_address)) >= 0 )
			{
				fputs("info: successfully connected\n", stderr);
				
				return 1;
			}
			else
			{
				int error = WSAGetLastError();
				fputs("error: couldn't establish connection", stderr);
				switch ( error )
				{
					case WSAENETUNREACH:
						fputs(": network is unreachable", stderr);
						break;
					case WSAETIMEDOUT:
						fputs(": host didn't respond within timeout", stderr);
						break;
					default:
						fprintf(stderr, "; error code: %i", error);
				}
				fputc('\n', stderr);
			}
		}
		else
		{
			fprintf(stderr, "error: couldn't create socket; error code: %i\n", WSAGetLastError());
		}
	}
	else
	{
		fprintf(stderr, "error: couldn't initialize Winsock DLL; error code: %i\n", WSAGetLastError());
	}
	
	return 0;
}

int nw_listen
(
 nw_stuff * const stuff,
 Port port
)
{
	if ( WSAStartup(MAKEWORD(2,2), &(stuff->wsadata)) == 0 )
	{
		SOCKET server_socket;
		
		fputs("info: successfully initialized Winsock DLL\n", stderr);
		
		server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if ( server_socket != INVALID_SOCKET )
		{
			fputs("info: successfully created socket\n", stderr);
			
			struct sockaddr_in address;
			
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_family = AF_INET;
			address.sin_port = htons(port);
			
			if ( bind(server_socket, (struct sockaddr *)&address, sizeof(address)) == 0 )
			{
				fprintf(stderr, "info: successfully bound socket to local IP and port number %u\n", port);
				
				if ( listen(server_socket, SOMAXCONN) == 0 )
				{
					stuff->socket = accept(server_socket, NULL, NULL);
					if ( stuff->socket != INVALID_SOCKET )
					{
						closesocket(server_socket);
						
						return 1;
					}
				}
			}
			else
			{
				int error = WSAGetLastError();
				fputs("error: couldn't establish connection", stderr);
				switch ( error )
				{
					case WSAENETUNREACH:
						fputs(": network is unreachable", stderr);
						break;
					case WSAETIMEDOUT:
						fputs(": host didn't respond within timeout", stderr);
						break;
					default:
						fprintf(stderr, "; error code: %i", error);
				}
				fputc('\n', stderr);
			}
		}
		else
		{
			fprintf(stderr, "error: couldn't create socket; error code: %i\n", WSAGetLastError());
		}
	}
	else
	{
		fprintf(stderr, "error: couldn't initialize Winsock DLL; error code: %i\n", WSAGetLastError());
	}
	
	return 0;
}

int nw_send
(
 const nw_stuff * const stuff,
 const char * const msg,
 const size_t msg_size
)
{
	if ( send(stuff->socket, msg, msg_size, 0) >= 0 )
	{
		fputs("info: message sent succesfully\n", stderr);
		
		return 1;
	}
	else
	{
		fprintf(stderr, "error: couldn't send message; error code: %i\n", WSAGetLastError());
		
		return 0;
	}
}

size_t nw_recv
(
 const nw_stuff * const stuff,
 char * const buffer,
 const size_t size
)
{
	int recv_size = recv(stuff->socket, buffer, size, 0);
	if ( recv_size != SOCKET_ERROR )
	{
		return recv_size;
	}
	else
	{
		int error = WSAGetLastError();
		fputs("error: couldn't receive message", stderr);
		switch ( error )
		{
			case WSAECONNABORTED:
				fputs(": connection was aborted", stderr);
				break;
			default:
				fprintf(stderr, "; error code: %i", error);
		}
		fputc('\n', stderr);
		
		return 0;
	}
}

void nw_close
(
 const nw_stuff * const stuff
)
{
	closesocket(stuff->socket);
	
	if ( WSACleanup() == 0 )
		fputs("info: successfully terminated use of the Winsock DLL\n", stderr);
	else
		fprintf(stderr, "error: couldn't terminate the use of the Winsock DLL; error code: %i\n", WSAGetLastError());
}
