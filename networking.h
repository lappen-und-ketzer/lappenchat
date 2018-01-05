#include <stddef.h> // size_t
#include <stdint.h> // uint_least16_t

typedef uint_least32_t IPv4address;
typedef uint_least16_t Port;

int nw_connect
(
 nw_stuff * const,
 IPv4address ip_address,
 Port port
);

int nw_listen
(
 nw_stuff * const,
 Port port
);

int nw_send
(
 const nw_stuff * const,
 const char * const message,
 const size_t message_size
);

size_t nw_recv
(
 const nw_stuff * const,
 char * const buffer,
 const size_t max_to_read
);

void nw_close
(
 const nw_stuff * const
);
