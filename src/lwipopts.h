#define TCP_WND 16384
#define TCP_MSS 1460
#define PBUF_POOL_SIZE 24

#define LWIP_ALTCP 1
#define LWIP_ALTCP_TLS 1
#define LWIP_ALTCP_TLS_MBEDTLS 1

#define LWIP_IPV6 1

// We get a redefinition error if we don't do this
#define LWIP_TIMEVAL_PRIVATE 0
