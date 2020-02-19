#include "Q_client.h"
#include "cmsis_os.h"
#include "net.h"
#include "api.h"
#include <string.h>

static Q_client_s this = Q_CLIENT_DEFAULT;
static void Q_client_task(void);


void Q_client_init(Q_client_groupcfg_e groupcfg, Q_client_recv_f recv_clbck){

	this.conn 		= netconn_new(NETCONN_UDP);
	this.recv_clbck = recv_clbck;

	IP4_ADDR(&this.mcast_addr, 224, 0, 0, groupcfg);
	IP4_ADDR(&this.remote_addr, 224, 0, 0, Q_CLIENT_GROUPCFG_CTRL);

	osThreadDef(QClientTask, Q_client_task, osPriorityNormal, 0, 1024);
	osThreadCreate(osThread(QClientTask), NULL);

}


static void Q_client_task(void){
	uint8_t *data = pvPortMalloc(sizeof(uint8_t));
	for(;;){
		if(net_is_link_up() && net_is_bound()){
			switch(this.state){
				case Q_CLIENT_UNBOUND:
					this.ip_addr = net_get_ip_addr();
					netconn_bind(this.conn, IP4_ADDR_ANY, 7000);
					netconn_join_leave_group(this.conn, &this.mcast_addr, &this.ip_addr, NETCONN_JOIN);
					this.state = Q_CLIENT_BOUND;
					break;
				case Q_CLIENT_BOUND:
					netconn_recv(this.conn, &this.buf);
					data = pvPortRealloc(data, sizeof(uint8_t) * this.buf->p->len);
					memcpy(data, this.buf->p->payload, this.buf->p->len);
					netbuf_delete(this.buf);
					this.recv_clbck(data);
			}
		}
	}
}

