
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_
#define _NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

//�ýṹ����ÿ���������ľ���IPһһ��Ӧ
typedef struct {
    struct sockaddr                *sockaddr;//һ�������ӵ�IP��ַ
    socklen_t                       socklen;//sockaddr�ṹ��ĳ���
    ngx_str_t                       name;//��ַ������

    ngx_int_t                       current_weight;
    ngx_int_t                       weight;

    ngx_uint_t                      fails;
    time_t                          accessed;

    ngx_uint_t                      max_fails;
    time_t                          fail_timeout;

    ngx_uint_t                      down;          /* unsigned  down:1; */

#if (NGX_HTTP_SSL)
    ngx_ssl_session_t              *ssl_session;   /* local to a process */
#endif
} ngx_http_upstream_rr_peer_t;


typedef struct ngx_http_upstream_rr_peers_s  ngx_http_upstream_rr_peers_t;
//����IP��ַ�б�
struct ngx_http_upstream_rr_peers_s {
    ngx_uint_t                      single;        /* unsigned  single:1; */
    ngx_uint_t                      number;//��������������peer�ĳ���
    ngx_uint_t                      last_cached;

 /* ngx_mutex_t                    *mutex; */
    ngx_connection_t              **cached;

    ngx_str_t                      *name;

    ngx_http_upstream_rr_peers_t   *next;//backup������IP�б�

    ngx_http_upstream_rr_peer_t     peer[1];//��backup������IP�б�
};

//�Ǹ��ؾ�����ʹ�õ����ݽṹ
typedef struct {
    ngx_http_upstream_rr_peers_t   *peers;//IP��ַ�б�
    ngx_uint_t                      current;//round robin�㷨����
    uintptr_t                      *tried;//����bit����
    uintptr_t                       data;
} ngx_http_upstream_rr_peer_data_t;


ngx_int_t ngx_http_upstream_init_round_robin(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
ngx_int_t ngx_http_upstream_init_round_robin_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
ngx_int_t ngx_http_upstream_create_round_robin_peer(ngx_http_request_t *r,
    ngx_http_upstream_resolved_t *ur);
ngx_int_t ngx_http_upstream_get_round_robin_peer(ngx_peer_connection_t *pc,
    void *data);
void ngx_http_upstream_free_round_robin_peer(ngx_peer_connection_t *pc,
    void *data, ngx_uint_t state);

#if (NGX_HTTP_SSL)
ngx_int_t
    ngx_http_upstream_set_round_robin_peer_session(ngx_peer_connection_t *pc,
    void *data);
void ngx_http_upstream_save_round_robin_peer_session(ngx_peer_connection_t *pc,
    void *data);
#endif


#endif /* _NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_ */
