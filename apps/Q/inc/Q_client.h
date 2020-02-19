
#ifndef _Q_CLIENT_H_
#define _Q_CLIENT_H_

#include "lwip/api.h"
#include "Q_defs.h"
#include "net.h"

#define Q_PORT 5214
#define Q_CLIENT_DEFAULT {NULL, NULL, {}, {}, Q_CLIENT_UNBOUND}

typedef enum{
	Q_CLIENT_UNBOUND,
	Q_CLIENT_BOUND
}Q_client_state_e;

typedef struct{
	struct netconn *conn;
	struct netbuf *buf;
	ip4_addr_t ip_addr;
	ip4_addr_t mcast_addr;
	ip4_addr_t remote_addr;
	Q_client_state_e state;
}Q_client_s;

Q_client_err_e Q_client_init(void);

#endif
