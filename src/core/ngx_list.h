
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;

// �ο���nginxԴ�����������ṹngx_list_t http://blog.csdn.net/livelylittlefish/article/details/6599065

/*
ngx_list_t��nginx��ʹ�õ�����ṹ���������ǳ�˵������ṹ(����`std::list`)��̫һ����
������list�������ݽṹ��һЩ�ص㣬����������Ԫ�أ�ʵ�ֶ�̬���������������������͵����ݽṹ���ܵ���ʼ�趨���������������ƣ�
��ͬ����������Ľڵ㣬`std::list`ÿ���ڵ�ֻ�ܴ��һ��Ԫ�أ�ngx_list_t�Ľڵ�ȴ��һ���̶���С�����飬���Դ�Ŷ��Ԫ�ء�

�ڳ�ʼ����ʱ��������Ҫ�趨����Ԫ������Ҫռ�õ��ڴ�ռ��С���Լ�ÿ���ڵ������������С��
�����Ԫ�ص����list�����ʱ�򣬻�����β���Ľڵ�������������Ԫ�أ��������ڵ����������ˣ���������һ���µĽڵ㵽���list����ȥ��
 */

// ngx_list_part_s�Ǵ���ngx_list_t�����һ���ڵ㡣
// �����������һ�����飬����������յ�Ԫ��
struct ngx_list_part_s {
    void             *elts; //����Ԫ��elts����,��������Ŀռ��СΪsize*nalloc
    ngx_uint_t        nelts; //��ǰ��ʹ�õ�elts������һ��ҪС�ڵ���nalloc
    ngx_list_part_t  *next; //ָ��ngx_list_t�е��¸�����part
};

// ngx_list_t�ṹ��һ������������ÿ���ڵ���ngx_list_part_t�ṹ��
// ��ngx_list_part_t���и�elts��һ�����飬�����������С�̶���Ԫ�أ�������ngx_pool_t����������ռ�
typedef struct {
    ngx_list_part_t  *last; //ָ�����������һ��Ԫ�أ��������൱��βָ�롣�����µĽڵ�ʱ���Ӵ˿�ʼ��
    ngx_list_part_t   part; //�����е�һ��Ԫ�أ��������൱��ͷָ�롣����ʱ���Ӵ˿�ʼ��
    size_t            size; //������ÿ��Ԫ�صĴ�С
    ngx_uint_t        nalloc; //�����ÿ��ngx_list_part_t��elts������������ɵ����Ԫ�ظ���
    ngx_pool_t       *pool; //��ǰlist���ݴ�ŵ��ڴ��
} ngx_list_t;

//ngx_list_create��ngx_list_init������һ���Ķ��Ǵ���һ��list��ֻ�Ƿ���ֵ��һ��...
ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

// ngx_list_init�ǳ�ʼ����һ�����е�����
static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size); //���ڴ������ռ����eltsָ����ÿռ�
    if (list->part.elts == NULL) {
        return NGX_ERROR;
    }

    list->part.nelts = 0; //�շ�����������ûʹ�ã�����Ϊ0
    list->part.next = NULL;
    list->last = &list->part; //last��ʼ��ʱ��ָ���׽ڵ�
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return NGX_OK;
}


/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) {
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          part = part->next;
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */


void *ngx_list_push(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
