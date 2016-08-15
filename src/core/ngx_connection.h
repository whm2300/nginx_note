
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CONNECTION_H_INCLUDED_
#define _NGX_CONNECTION_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_listening_s  ngx_listening_t;

struct ngx_listening_s {
    // socket�׽��־��
    ngx_socket_t        fd;

    // ����socketaddr��ַ
    struct sockaddr    *sockaddr;	
    // socketaddr��ַ����
    socklen_t           socklen;    /* size of sockaddr */
    // �洢IP��ַ���ַ���addr_text��󳤶ȣ�����ָ����addr_text ��������ڴ��С
    size_t              addr_text_max_len;
    // ���ַ�����ʽ�洢IP��ַ
    ngx_str_t           addr_text;

    // �׽������ͣ����磬��type��SOCK_STREAM ʱ����ʾTCP
    int                 type;

    /* TCPʵ�ּ���ʱ��backlog���У�����ʾ��������ͨ���������ֽ���TCP���ӵ���û���κν��̿�ʼ��������������� */
    int                 backlog;
    // �ں��ж�������׽��ֵĽ��ջ�������С
    int                 rcvbuf;
    // �ں��ж�������׽��ֵķ��ͻ�������С
    int                 sndbuf;

    /* handler of accepted connection */
    // ���µ�TCP���ӳɹ�������Ĵ�����
    ngx_connection_handler_pt   handler;

    /*
    ʵ���Ͽ�ܲ�������servers ָ�룬����������Ϊһ������ָ�룬Ŀǰ��Ҫ����HTTP����mail��ģ�飬�û����浱ǰ�����˿ڶ�Ӧ�ŵ�����������
    */
    void               *servers;  /* array of ngx_http_in_addr_t, for example */

    // log��logp���ǿ��õ���־�����ָ��
    ngx_log_t           log;
    ngx_log_t          *logp;

    // ���Ϊ�µ�TCP���Ӵ����ڴ�أ����ڴ�صĳ�ʼ��СӦ����pool_size
    size_t              pool_size;

    size_t              post_accept_buffer_size;
    /* should be here because of the deferred accept */
    /* should be here because of the AcceptEx() preread */
    /* TCP_DEFER_ACCEPT ѡ��ڽ���TCP���ӳɹ��ҽ��յ��û����������ݺ󣬲���Լ����׽��ָ���Ȥ�Ľ��̷����¼�֪ͨ�������ӽ����ɹ���
    ���post_accept_timeout �����Ȼû���յ����û����ݣ����ں�ֱ�Ӷ�������
    */
    ngx_msec_t          post_accept_timeout;

    // ǰһ��ngx_listening_t�ṹ�����ngx_listening_t�ṹ��֮����previousָ����ɵ�����
    ngx_listening_t    *previous;  
    // ��ǰ���������Ӧ�ŵ�ngx_connection_t�ṹ��
    ngx_connection_t   *connection;

    /*
    ��־λ��Ϊ1���ʾ�ڵ�ǰ���������Ч����ִ��ngx_init_cycleʱ���رռ����˿ڣ�Ϊ0ʱ�������رա��ı�־λ��ܴ�����Զ����á�
    */
    unsigned            open:1;
    /*
    ��־λ��Ϊ1��ʾʹ���Ѿ��е�ngx_cycle_t����ʼ���µ�ngx_cycle_t�ṹ��ʱ�����ر�ԭ�ȴ򿪵ļ����˿ڣ����������������������ã�
    remainΪ0ʱ����ʾ�����ر������򿪵ļ����˿ڡ��ñ�־λ��ܴ�����Զ����ã��μ�ngx_init_cycle������
    */
    unsigned            remain:1;
    /*
    ��־λ��Ϊ1��ʾ�������õ�ǰngx_listening_t�ṹ���е��׽��֣�Ϊ0ʱ������ʼ���׽��֣��ñ�־λ��ܴ�����Զ�����
    */
    unsigned            ignore:1;

    // ��ʾ�Ƿ��Ѿ��󶨣�ʵ����Ŀǰ�ñ�־λû��ʹ��
    unsigned            bound:1;       /* already bound */
    // ��ʾ��ǰ��������Ƿ�����ǰһ�����̣�������nginx����
    // ���Ϊ1�� ���ʾ����ǰһ�����̣�һ��ᱣ��֮ǰ�Ѿ����úõ��׽��֣������ı�
    unsigned            inherited:1;   /* inherited from previous process */
    // Ŀǰδʹ��
    unsigned            nonblocking_accept:1;
    // ��־λ��Ϊ1ʱ��ʾ��ǰ�ṹ���Ӧ���׽����Ѿ�����
    unsigned            listen:1;
    // Ŀǰδʹ��
    unsigned            nonblocking:1;
    // Ŀǰ�ñ�־λû������
    unsigned            shared:1;    /* shared between threads or processes */
    // ��־λ��Ϊ1ʱ��ʾnginx�Ὣ�����ַת��Ϊ�ַ�����ʽ�ĵ�ַ
    unsigned            addr_ntop:1;

#if (NGX_HAVE_INET6 && defined IPV6_V6ONLY)
    unsigned            ipv6only:2;
#endif

#if (NGX_HAVE_DEFERRED_ACCEPT)
    unsigned            deferred_accept:1;
    unsigned            delete_deferred:1;
    unsigned            add_deferred:1;
#ifdef SO_ACCEPTFILTER
    char               *accept_filter;
#endif
#endif
#if (NGX_HAVE_SETFIB)
    int                 setfib;
#endif

};


typedef enum {
     NGX_ERROR_ALERT = 0,
     NGX_ERROR_ERR,
     NGX_ERROR_INFO,
     NGX_ERROR_IGNORE_ECONNRESET,
     NGX_ERROR_IGNORE_EINVAL
} ngx_connection_log_error_e;


typedef enum {
     NGX_TCP_NODELAY_UNSET = 0,
     NGX_TCP_NODELAY_SET,
     NGX_TCP_NODELAY_DISABLED
} ngx_connection_tcp_nodelay_e;


typedef enum {
     NGX_TCP_NOPUSH_UNSET = 0,
     NGX_TCP_NOPUSH_SET,
     NGX_TCP_NOPUSH_DISABLED
} ngx_connection_tcp_nopush_e;


#define NGX_LOWLEVEL_BUFFERED  0x0f
#define NGX_SSL_BUFFERED       0x01


struct ngx_connection_s {
    /*
    ����δʹ��ʱ��data��Ա���ڳ䵱���ӳ��п������������е�nextָ�롣�����ӱ�ʹ��ʱ��data��������ʹ������nginxģ�������
    ����HTTP����У�dataָ��ngx_http_request_t����
    */
    void               *data;

    // ���Ӷ�Ӧ�Ķ��¼�
    ngx_event_t        *read;
    // ���Ӷ�Ӧ��д�¼�
    ngx_event_t        *write;

    // �׽��־��
    ngx_socket_t        fd;

    // ֱ�ӽ��������ַ����ķ���
    ngx_recv_pt         recv;
    // ֱ�ӷ��������ַ����ķ���
    ngx_send_pt         send;
    // ��ngx_chain_t����Ϊ���������������ַ����ķ���
    ngx_recv_chain_pt   recv_chain;
    // ��ngx_chain_t����Ϊ���������������ַ����ķ���
    ngx_send_chain_pt   send_chain;

    // ������Ӷ�Ӧ��ngx_listening_t�������󣬴�������listening �����˿ڵ��¼�����
    ngx_listening_t    *listening;

    // ����������Ѿ����ͳ�ȥ���ֽ���
    off_t               sent;

    // ���Լ�¼��־��ngx_log_t����
    ngx_log_t          *log;

    /*
    �ڴ�أ�һ����acceptһ��������ʱ���ᴴ��һ���ڴ�أ�����������ӽ���ʱ�������ڴ�ء�
    */
    ngx_pool_t         *pool;

    //���ӿͻ��˵�socketaddr�ṹ��
    struct sockaddr    *sockaddr;
    // socketaddr�ṹ��ĳ���
    socklen_t           socklen;
    // ���ӿͻ����ַ�����ʽ��IP��ַ
    ngx_str_t           addr_text;

#if (NGX_SSL)
    ngx_ssl_connection_t  *ssl;
#endif

    // �����ļ����˿ڶ�Ӧ��socketaddr�ṹ�壬Ҳ����listening���������е�sockaddr��Ա
    struct sockaddr    *local_sockaddr;

    /* 
    ���ڽ��ա�����ͻ��˷������ַ�����ÿ���¼�����ģ������ɾ��������ӳ��з�����Ŀռ�� buffer������ջ����ֶΡ�
    ���磬��HTTPģ���У����Ĵ�С������client_header_buffer_size������
    */ 
    ngx_buf_t          *buffer;

    /*
    ���ֶ����ڽ���ǰ������˫������Ԫ�ص���ʽ��ӵ�ngx_cycle_t���Ľṹ���reusable_connections_queue˫�������У���ʾ�����õ�����
    */
    ngx_queue_t         queue;

    /*
    ����ʹ�ô�����ngx_connection_t�ṹ��ÿ�ν���һ�����Կͻ��˵����ӣ����������������˷�������������ʱ ��ngx_peer_connection_tҲʹ������
    number�����1
    */
    ngx_atomic_uint_t   number;

    // ������������
    ngx_uint_t          requests;

    /*
    �����е�ҵ�����͡��κ��¼�����ģ�鶼�����Զ�����Ҫ�ı�־λ��
    ���buffered�ֶ���8λ��������ͬʱ��ʾ8����ͬ��ҵ�񡣵�����ģ�����Զ���buffered��־λʱע�ⲻҪ�����ʹ�õ�ģ�鶨��ı�־λ��ͻ��
    */
    unsigned            buffered:8;

    /*
    �����Ӽ�¼��־ʱ�ļ�����ռ����3λ��ȡֵ��Χ��0��7����ʵ����Ŀǰֻ������5��ֵ����ngx_connection_log_error_eö�ٱ�ʾ�����£�
    typedef enum{
        NGX_ERROR_ALERT = 0,
        NGX_ERROR_ERR,
        NGX_ERROR_INFO,
        NGX_ERROR_IGNORE_ECONNRESET,
        NGX_ERROR_IGNORE_EINVAL,
    } ngx_connection_log_error_e;
    */
    unsigned            log_error:3;     /* ngx_connection_log_error_e */

    /*
    ��־λ��Ϊ1��ʾ���������ӣ���ӿͻ��˷�������ӣ�
    Ϊ0��ʾ�����������ӵ���Ϊ�����������ķǶ������ӣ���ʹ��upstream�������˷�������������������
    */
    unsigned            single_connection:1;
    // ��־λ��Ϊ1��ʾ���ڴ��ַ���������Ŀǰ������
    unsigned            unexpected_eof:1;
    // ��־λ��Ϊ1��ʾ�����ѳ�ʱ
    unsigned            timedout:1;
    // ��־λ��Ϊ1��ʾ���Ӵ�������г��ִ���
    unsigned            error:1;
    // ��־λ��Ϊ1��ʾ�����Ѿ����١����������ָ����TCP���ӣ�������ngx_connection_t�ṹ�塣
    // ��destroyΪ1ʱ��ngx_connection_t�ṹ����Ȼ���ڣ������Ӧ���׽��֣��ڴ���Ѿ������á�
    unsigned            destroyed:1;

    // ��־λ��Ϊ1��ʾ���Ӵ��ڿ���״̬����keepalive��������������֮���״̬
    unsigned            idle:1;
    // ��־λ��Ϊ1��ʾ���ӿ����ã����������queue�ֶ��Ƕ�Ӧʹ�õ�
    unsigned            reusable:1;
    // ��־λ��Ϊ1��ʾ���ӹر�
    unsigned            close:1;

    // ��־λ��Ϊ1��ʾ���ڽ��ļ��е����ݷ������ӵ���һ��
    unsigned            sendfile:1;
    /*
    ��־λ�����Ϊ1�� ���ʾֻ���������׽��ֶ�Ӧ�ķ��ͻ�������������������õĴ�С��ֵ���¼�����ģ�ͲŻ�ַ����¼���
    */ 
    unsigned            sndlowat:1;
    // ��־λ����ʾ���ʹ��TCP��nodelay���ԡ�����ȡֵ��Χ��ngx_connection_tcp_nodelay_e
    unsigned            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    // ��־λ����ʾ���ʹ��TCP��nopush���ԣ�����ȡֵ��Χ��ngx_connection_tcp_nopush_e
    unsigned            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

#if (NGX_HAVE_IOCP)
    unsigned            accept_context_updated:1;
#endif

#if (NGX_HAVE_AIO_SENDFILE)
    // ��־λ��Ϊ1ʱ��ʾʹ���첽I/O�ķ�ʽ���������ļ����͸��������ӵ���һ��
    unsigned            aio_sendfile:1;
    // ʹ���첽 I/O ��ʽ���͵��ļ���busy_sendfile����������������ļ�����Ϣ
    ngx_buf_t          *busy_sendfile;
#endif

#if (NGX_THREADS)
    ngx_atomic_t        lock;
#endif
};


ngx_listening_t *ngx_create_listening(ngx_conf_t *cf, void *sockaddr,
    socklen_t socklen);
ngx_int_t ngx_set_inherited_sockets(ngx_cycle_t *cycle);
ngx_int_t ngx_open_listening_sockets(ngx_cycle_t *cycle);
void ngx_configure_listening_sockets(ngx_cycle_t *cycle);
void ngx_close_listening_sockets(ngx_cycle_t *cycle);
void ngx_close_connection(ngx_connection_t *c);
ngx_int_t ngx_connection_local_sockaddr(ngx_connection_t *c, ngx_str_t *s,
    ngx_uint_t port);
ngx_int_t ngx_connection_error(ngx_connection_t *c, ngx_err_t err, char *text);

ngx_connection_t *ngx_get_connection(ngx_socket_t s, ngx_log_t *log);
void ngx_free_connection(ngx_connection_t *c);

void ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable);

#endif /* _NGX_CONNECTION_H_INCLUDED_ */
