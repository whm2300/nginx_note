
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

//�ο�	http://blog.csdn.net/sunhappy2200/article/details/5915189
// 		http://tengine.taobao.org/book/chapter_02.html#ngx-array-t-100
#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

// ��̬����
struct ngx_array_s {
    // eltsָ��������׵�ַ
    void        *elts; 
    // nelts���������Ѿ�ʹ�õ�Ԫ�ظ���
    ngx_uint_t   nelts; 
    // ÿ������Ԫ��ռ�õ��ڴ��С
    size_t       size;  
    // ��ǰ�������ܹ�����Ԫ�ظ������ܴ�С
    ngx_uint_t   nalloc; 
    // �ڴ�ض���
    ngx_pool_t  *pool;  
};

/*
���ڴ���д���n��Ԫ�ص����飬Ԫ�ش�СΪsize
����һ���µ�������󣬲������������

p:	��������ڴ�ʹ�õ��ڴ�أ�
n:	����ĳ�ʼ������С�����ڲ����ݵ���������������ɵ�Ԫ�ظ�����
size:	����Ԫ�صĴ�С����λ���ֽڡ�

ע������: ����ʹ��ngx_palloc�����ڴ棬����������ʱ���ɵ��ڴ治�ᱻ�ͷţ�������ڴ���˷ѡ�
��ˣ��������ǰ�滮��������������ڴ������߳�ʼ����ʱ��һ�θ㶨�����������ݣ�����ڴ��˷ѡ�
 */
ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);

// ���ٸ�������󣬲��ͷ��������ڴ���ڴ�ء�
void ngx_array_destroy(ngx_array_t *a);

// ������a����׷��һ��Ԫ�أ�������ָ����Ԫ�ص�ָ�롣
// ��Ҫ�ѷ��ص�ָ��ʹ������ת����ת��Ϊ��������ͣ�Ȼ���ٸ���Ԫ�ر�������Ǹ��ֶΣ���������Ԫ���Ǹ������ͣ���ֵ��
// ������������������·���������nalloc*size)���ڴ�ռ䣬��nalloc����Ϊ2*nalloc
void *ngx_array_push(ngx_array_t *a);

// ���ؽ�Ҫ���n��Ԫ�ص����������׸�Ԫ�صĵ�ַ
void *ngx_array_push_n(ngx_array_t *a, ngx_uint_t n);


// ���һ����������Ǳ������ڶ��ϵģ���ô������ngx_array_destroy�����Ժ�������ٴ�ʹ�ã��Ϳ��Ե��ô˺�����
// ���һ����������Ǳ�������ջ�ϵģ���ô����Ҫ���ô˺��������г�ʼ���Ĺ����Ժ󣬲ſ���ʹ�á�
static ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * size);
    if (array->elts == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
