
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

//�ú�������һ��ngx_list_t���͵Ķ���,���Ը�list�ĵ�һ���ڵ������Ԫ�ص��ڴ�ռ䡣
//
//pool:	�����ڴ�ʹ�õ�pool��
//n:	ÿ���ڵ�(ngx_list_part_t)�����Դ�ŵ�Ԫ�ظ���
//size:	ÿ��Ԫ����ռ�õ��ڴ��С
ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    // �ȴ���һ��ngx_list_tָ��
    list = ngx_palloc(pool, sizeof(ngx_list_t));
    if (list == NULL) {
        return NULL;
    }

    list->part.elts = ngx_palloc(pool, n * size);
    if (list->part.elts == NULL) {
        return NULL;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return list;
}

/*
ngx_list_create��ʹ��ʾ����

ngx_list_t *testlist = ngx_list_create(r->pool, 4, sizeof(ngx_str_t));
if (testlist == NULL) {
    return NGX_ERROR;
} 
*/


// ������l������µ�Ԫ�أ����ص����·����Ԫ�ص��׵�ַ
void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    if (last->nelts == l->nalloc) {
        // ������last�ڵ��Ѿ�ʹ����nlloc��elts�����Ա����½�һ��last�ڵ�
        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, sizeof(ngx_list_part_t));
        if (last == NULL) {
            return NULL;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        if (last->elts == NULL) {
            return NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        l->last->next = last;
        l->last = last;
    }

    elt = (char *) last->elts + l->size * last->nelts;
    last->nelts++;

    return elt;
}

/*
ngx_list_push��ʹ��ʵ����

ngx_str_t *str = ngx_list_push(testlist);
if (str == NULL) {
    return NGX_ERROR;
}

str->len = sizeof("Hello World");
str->value = "Hello World";
*/



/*���������ʵ��
//part����ָ�������е�ÿһ��ngx_list_part_t����
ngx_list_part_t* part = &testlist.part;
//���������е��������ͣ����������eltsת��Ϊ������ʹ��
ngx_str_t* str = part->elts;
//i��ʾԪ���������ÿ��ngx_list_part_t����������
for (i = 0;;i++)
{
	if (i>=part->next)
	{
		if (part->next == NULL)
		{
			//˵��ĳ��ngx_list_part_t�����nextָ��Ϊ�գ���˵���Ѿ�����������
			break;
		}
		//������һ��ngx_list_part_t
		part = part->next;
		header = part->elts;
		//��i���������Ϊ0��׼�����·�����һ������
		i = 0;
	}
	//������Ժܷ����ȡ����ǰ������������Ԫ�ء�
	printf("List Element:%*s\n", str[i].len, str[i].data);
}
*/