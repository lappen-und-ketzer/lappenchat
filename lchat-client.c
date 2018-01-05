#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
#include <string.h> // strlen
#include <stdio.h> // stdin, stdout, stderr, sscanf, fputs, puts
#include <inttypes.h> // SCNoLEAST16
#include "config/networkapi.h"

#ifdef USE_networkapi_winsock2
#include "winsock2.h"
#elif defined(USE_networkapi_unix)
#error Networking using Unix sockets is not implemented
#endif
#include "networking.h"

static int
parse_ipv4_address
(
 const char * const s,
 IPv4address * const address,
 Port * const port
)
{
	int rv;
	unsigned int c1, c2, c3, c4;
	rv = sscanf(s, "%u.%u.%u.%u:%"SCNuLEAST16, &c1, &c2, &c3, &c4, port);
	*address = (IPv4address)c4 + (((IPv4address)c3) << 8) + (((IPv4address)c2) << 16) + (((IPv4address)c1) << 24);
	
	return rv == 5 && c1 < 256 && c2 < 256 && c3 < 256 && c4 < 256;
}

static void
prettyfy_ip_address
(
 IPv4address address,
 char buffer[16]
)
{
	unsigned char c1, c2, c3, c4;
	
	c1 = (unsigned char)(address / (256u * 256u * 256u));
	c2 = (unsigned char)(address / (256u * 256u));
	c3 = (unsigned char)(address / 256u);
	c4 = (unsigned char)(address);
	
	sprintf(buffer, "%u.%u.%u.%u", c1, c2, c3, c4);
}

int main
(
 int argc,
 char * * argv
)
{
	if ( argc == 2 )
	{
		IPv4address ip_address;
		Port port;
		
		if ( parse_ipv4_address(argv[1], &ip_address, &port) )
		{
			char ip_address_pretty[16];
			nw_stuff stuff;
			
			prettyfy_ip_address(ip_address, ip_address_pretty);
			
			fprintf(stderr, "info: will try to connect to %s, port number %u\n", ip_address_pretty, port);
			
			if ( nw_connect(&stuff, ip_address, port) )
			{
				while ( 1 )
				{
					char message[256];
					
					puts("Enter a message to send");
					
					fgets(message, sizeof(message), stdin);
					
					if ( *message == '\n' )
					{
						// User entered an empty line: exit
						break;
					}
					else
					{
						const size_t message_length = strlen(message);
						
						// Remove last newline
						message[message_length-1] = 0;
						
						if ( !nw_send(&stuff, message, message_length) )
						{
							break;
						}
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
