#include <stdint.h>

#define MEM_ALIGNMENT 4
#define MEM_SIZE 4000
#define MEMP_NUM_TCP_SEG 32
#define MEMP_NUM_ARP_QUEUE 10
#define PBUF_POOL_SIZE 24
#define LWIP_ARP 1
#define LWIP_ETHERNET 1
#define LWIP_ICMP 1
#define LWIP_RAW 1
#define TCP_WND (16 * TCP_MSS)
#define TCP_MSS 1460
#define TCP_SND_BUF (8 * TCP_MSS)
#define TCP_SND_QUEUELEN ((4 * (TCP_SND_BUF) + (TCP_MSS - 1)) / (TCP_MSS))

#define TCPIP_THREAD_STACKSIZE 1024
#define DEFAULT_THREAD_STACKSIZE 1024
#define DEFAULT_RAW_RECVMBOX_SIZE 8
#define TCPIP_MBOX_SIZE 8
#define LWIP_TIMEVAL_PRIVATE 0
#define LWIP_TCPIP_CORE_LOCKING_INPUT 1

#define LWIP_ALTCP 1
#define LWIP_ALTCP_TLS 1
#define LWIP_ALTCP_TLS_MBEDTLS 1

#define LWIP_IPV6 1

#define LWIP_NETIF_HOSTNAME 1

#define SNTP_SUPPORT 1
#define SNTP_SERVER_DNS 1
#define SNTP_UPDATE_DELAY 86400

void time_set_sntp(uint32_t sec, uint32_t us);
#define SNTP_SET_SYSTEM_TIME(sec, us) time_set_sntp(sec, us);

// We get a redefinition error if we don't do this
#define LWIP_TIMEVAL_PRIVATE 0
