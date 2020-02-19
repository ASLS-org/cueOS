#include "Q_client.h"
#include "cmsis_os.h"
#include "net.h"
#include "api.h"
#include <string.h>

static Q_client_s this = Q_CLIENT_DEFAULT;
static void Q_client_task(void);


Q_client_err_e Q_client_init(void){

	Q_client_err_e err = Q_CLIENT_OK;

	this.conn = netconn_new(NETCONN_UDP);

	if(this.conn != NULL){
		osThreadDef(QClientTask, Q_client_task, osPriorityNormal, 0, 1024);
		osThreadCreate(osThread(QClientTask), NULL);
	}else{
		err = Q_CLIENT_UNABLE_TO_CREATE_CON;
	}

	return err;

}


static void Q_client_task(void){
	char msg[] = "OUI!\n";
	char *data;
	err_t err;
	struct netbuf *recv_netbuf;
	for(;;){
		if(net_is_link_up() && net_is_bound()){
			switch(this.state){
				case Q_CLIENT_UNBOUND:
					this.ip_addr	 = net_get_ip_addr();
					this.remote_addr = net_get_gateway();
					IP4_ADDR(&this.mcast_addr, 239, 1, 1, 234);
					netconn_bind(this.conn, IP4_ADDR_ANY, 7000);
					err = netconn_join_leave_group(this.conn, &this.mcast_addr, &this.ip_addr, NETCONN_JOIN);
					//netconn_connect(this.conn, IP4_ADDR_BROADCAST, 7000);
					this.state = Q_CLIENT_BOUND;
					break;
				case Q_CLIENT_BOUND:
					netconn_recv(this.conn, &recv_netbuf);
					break;
			}
		}
		osDelay(200);
	}
}
