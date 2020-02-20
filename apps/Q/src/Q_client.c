#include "cmsis_os.h"
#include <string.h>
#include "Q_client.h"
#include "net.h"

static Q_client_s this = Q_CLIENT_DEFAULT;
static void Q_client_task(void);


void Q_client_init(Q_client_groupcfg_e groupcfg, Q_client_frwrd_pckt_f frwrd_pckt_callback){

	this.frwrd_pckt = frwrd_pckt_callback;
	this.conn = netconn_new(NETCONN_UDP);

	IP4_ADDR(&this.mcast_addr, 224, 0, 0, groupcfg);
	IP4_ADDR(&this.remote_addr, 224, 0, 0, Q_CLIENT_GROUPCFG_CTRL);

	osThreadDef(QClientTask, Q_client_task, osPriorityNormal, 0, 1024);
	osThreadCreate(osThread(QClientTask), NULL);

}

void Q_client_send(char *raw_data){
	this.send_buf = netbuf_new();
	netbuf_ref(this.send_buf, raw_data, strlen(raw_data));
	netconn_send(this.conn, this.send_buf);
	netbuf_delete(this.send_buf);
}

static void Q_client_task(void){
	for(;;){
		if(net_is_link_up() && net_is_bound()){
			switch(this.state){
				case Q_CLIENT_UNBOUND:
					this.ip_addr = net_get_ip_addr();
					this.remote_addr = net_get_gateway();
					netconn_bind(this.conn, IP4_ADDR_ANY, Q_CLIENT_DEFAULT_PORT);
					netconn_join_leave_group(this.conn, &this.mcast_addr, &this.ip_addr, NETCONN_JOIN);
					this.state = Q_CLIENT_BOUND;
					break;
				case Q_CLIENT_BOUND:
					netconn_recv(this.conn, &this.recv_buf);
					Q_packet_s *packet = Q_packet_parse(this.recv_buf->p->payload, this.recv_buf->p->len);
					this.frwrd_pckt(packet);
					Q_packet_free(packet);
					netbuf_delete(this.recv_buf);
			}
		}
		osDelay(200);
	}
}

