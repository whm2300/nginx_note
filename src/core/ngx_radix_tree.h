
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_RADIX_TREE_H_INCLUDED_
#define _NGX_RADIX_TREE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


#define NGX_RADIX_NO_VALUE   (uintptr_t) -1

typedef struct ngx_radix_node_s  ngx_radix_node_t;

// ������
struct ngx_radix_node_s {
    // ָ�������������û������������ֵΪnull��ָ��
    ngx_radix_node_t  *right;
    // ָ�������������û������������ֵΪnull��ָ��
    ngx_radix_node_t  *left;
    // ָ�򸸽ڵ㣬���û�и��ڵ㣬��ֵΪnull��ָ��
    ngx_radix_node_t  *parent;
    // value�洢����ָ���ֵ����ָ���û���������ݽṹ���������ڵ㻹δʹ�ã�value��ֵ����NGX_RADIX_NO_VALUE
    uintptr_t          value;
};


typedef struct {
    // ָ����ڵ�
    ngx_radix_node_t  *root;
    // �ڴ�أ���������������Ľڵ�����ڴ�
    ngx_pool_t        *pool;
    // �����Ѿ����䵫��ʱδʹ�õĽڵ㣬freeʵ���������в������нڵ�ĵ�����
    ngx_radix_node_t  *free;
    // �ѷ����ڴ��л�δʹ���ڴ���׵�ַ
    char              *start;
    // �ѷ����ڴ��л�δʹ�õ��ڴ��С
    size_t             size;
} ngx_radix_tree_t;


ngx_radix_tree_t *ngx_radix_tree_create(ngx_pool_t *pool,
    ngx_int_t preallocate);
ngx_int_t ngx_radix32tree_insert(ngx_radix_tree_t *tree,
    uint32_t key, uint32_t mask, uintptr_t value);
ngx_int_t ngx_radix32tree_delete(ngx_radix_tree_t *tree,
    uint32_t key, uint32_t mask);
uintptr_t ngx_radix32tree_find(ngx_radix_tree_t *tree, uint32_t key);


#endif /* _NGX_RADIX_TREE_H_INCLUDED_ */
