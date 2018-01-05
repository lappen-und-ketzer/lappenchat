#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
#include <string.h> // strlen
#include <stdio.h> // stderr, sscanf, fputs, puts
#include <inttypes.h> // SCNoLEAST16
#include "config/networkapi.h"

#ifdef USE_networkapi_winsock2
#include "winsock2.h"
#elif defined(USE_networkapi_unix)
#error Networking using Unix sockets is not implemented
#endif
#include "networking.h"

static int
parse_port
(
 const char * const s,
 Port * const port
)
{
	return sscanf(s, "%"SCNuLEAST16, port) == 1;
}

int main
(
 int argc,
 char * * argv
)
{
	if ( argc == 2 )
	{
		Port port;
		
		if ( parse_port(argv[1], &port) )
		{
			//char ip_address_pretty[16];
			nw_stuff stuff;
			
			//prettyfy_ip_address(ip_address, ip_address_pretty);
			
			fprintf(stderr, "info: will try to listen on port number %u\n", port);
			//fprintf(stderr, "info: will try to connect to %s, port number %u\n", ip_address_pretty, port);
			
			if ( nw_listen(&stuff, port) )
			{
				while ( 1 )
				{
					char message[2048];
					size_t message_size;
					
					message_size = nw_recv(&stuff, message, sizeof(message)-1);
					if ( message_size )
					{
						message[message_size] = 0;
						
						puts(message);
					}
					else
					{
						break;
					}
				}
				
				nw_close(&stuff);
			}
			else
			{
				fputs("error: couldn't connect\n", stderr);
			}
		}
		else
		{
			fputs("error: address and/or port specified are invalid\n", stderr);
		}
	}
	else
	{
		fputs("error: less or more than the required three arguments were specified", stderr);
	}
	
	return EXIT_FAILURE;
}
