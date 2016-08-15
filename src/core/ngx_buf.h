
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */
 //���������ϣ�
 //http://hi.baidu.com/langwan/blog/item/822b758d5d1d9a1ab31bbaf8.html


#ifndef _NGX_BUF_H_INCLUDED_
#define _NGX_BUF_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef void *            ngx_buf_tag_t;

typedef struct ngx_buf_s  ngx_buf_t;

// ngx_buf_s��nginx���ڴ�������ݵĹؼ����ݽṹ
// ����Ӧ�����ڴ����ݣ�ҲӦ���ڴ������ݡ�
struct ngx_buf_s {
    // �����ڴ�����
    u_char          *pos;       //��֪��Ҫ������ڴ����ݵ���ʼλ��
    u_char          *last;      //��֪��Ҫ������ڴ����ݵĽ���λ�ã���ϣ�����������Ϊ[pos,last)

    // �����ļ�����
    off_t            file_pos;  //��֪��Ҫ������ļ����ݵ���ʼλ��
    off_t            file_last; //��֪��Ҫ������ļ����ݵĽ���λ��

    // �����ڴ�����
    u_char          *start;      //��һ�����ڴ汻�����ڶ��buf�е�ʱ����ô��Щbuf�����start��end��ָ������ڴ����ʼλ�ú���ֹλ�ã���pos��ͬ��pos����ڵ���start
    u_char          *end;        //��start��������last��ͬ��last��С�ڵ���end

    ngx_buf_tag_t    tag;        //��ǰ�����������͡��������ĸ�ģ��ʹ�ã���ָ�����ģ��ngx_module_t�����ĵ�ַ
    ngx_file_t      *file;       //�ļ����������õ��ļ�

    // ��ǰ��������Ӱ�ӻ������������Ա����ʹ�õ���
	//��һ��buf������copy��һbuf�������ֶε�ʱ����ô������bufָ���ʵ������ͬһ���ڴ����ͬһ���ļ���
	//��ʱ������buf��shadow���໥ָ��Է��ģ���ô��������������buf���ͷŵ�ʱ����Ҫ�ر�С�ġ�
    ngx_buf_t       *shadow;

    /* the buf's content could be changed */
    //��ʱ�ڴ��־λ��1��ʾ��������ʱ�ڴ��У���������ݿ����޸�
    unsigned         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    //�ڴ��־λ��1��ʾ�������ڴ��У���������ݲ��ܱ��޸�
    unsigned         memory:1;

    /* the buf's content is mmap()ed and must not be changed */
    // ��־λ��1��ʾ����ڴ�����mmapϵͳ����ӳ������ģ������Ա��޸�
    unsigned         mmap:1; 
	
	//��־λ��1��ʾ���Ա����գ�ͨ�����shadow�ֶ�һ��ʹ�ã�
	//��ʹ��ngx_create_temp_buf����������bufͬʱҲ����һ��buf��shadow��ʱ�򣬱�ʾ���buf�ǿ��ͷŵ�
    unsigned         recycled:1;  

	
    unsigned         in_file:1;     //��־λ��1��ʾ�Ǵ����ļ����ݣ��������ڴ�����
	
	
	//������flush�ֶα�����Ϊ1��buf��chain�����chain�����ݼ�ʹ����������������Ҳ����������
	//������postpone_output���õ����ơ�
    unsigned         flush:1;

    //��־λ�����ڲ������������ʱ�Ƿ�ʹ��ͬ����ʽ����Ҫ�������ǡ�
    //���п��ܻ�����nginx���̣�nginx�����в������������첽�ġ�
    unsigned         sync:1; 

    // ��־λ���Ƿ������һ�黺������nginx_buf_t������ngx_chain_t����������
    // 1���������һ�������Ļ�����
    unsigned         last_buf:1; 

    //��־λ���Ƿ���ngx_chain_t�е����һ�黺����
    unsigned         last_in_chain:1; 

    //��־λ���Ƿ������һ��Ӱ�ӻ���������shadow���ʹ�ã�ͨ��������ʹ�ã�
	//�ڴ���һ��buf��shadow��ʱ��ͨ�����´�����һ��buf��last_shadow����Ϊ1.
    unsigned         last_shadow:1;

    //��־λ���Ƿ�������ʱ�ļ�
	//�����ܵ��ڴ�ʹ�õ����ƣ���ʱ��һЩbuf��������Ҫ��д�������ϵ���ʱ�ļ���
    unsigned         temp_file:1;

    /* STUB */ int   num;
};

// nginx�������ݽṹ�����統�û�����HTTP�����ʱ�򣬾���Ҫ����ngx_chain_t�������
// ��������һ��ngx_chain_t����next����Ϊnull��������Զ���ᷢ�ͳɹ�
struct ngx_chain_s {
    ngx_buf_t    *buf; //�����Ӧ��buffer
    ngx_chain_t  *next; //������һ��Ԫ��
};


typedef struct {
    ngx_int_t    num;   //������������
    size_t       size;  //�������Ĵ�С
} ngx_bufs_t; //��������Ĳ����ṹ������һ�δ������������


typedef struct ngx_output_chain_ctx_s  ngx_output_chain_ctx_t;

typedef ngx_int_t (*ngx_output_chain_filter_pt)(void *ctx, ngx_chain_t *in);

#if (NGX_HAVE_FILE_AIO)
typedef void (*ngx_output_chain_aio_pt)(ngx_output_chain_ctx_t *ctx,
    ngx_file_t *file);
#endif

//�ο���
//http://simohayha.iteye.com/blog/662327
struct ngx_output_chain_ctx_s {
    ngx_buf_t                   *buf;           //������ʱ��buf
    ngx_chain_t                 *in;            //�����˽�Ҫ���͵�chain
    ngx_chain_t                 *free;          //�������Ѿ�������ϵ�chain���Ա����ظ�����
    ngx_chain_t                 *busy;          //�����˻�δ���͵�chain

    unsigned                     sendfile:1;    //sendfile���
    unsigned                     directio:1;    //directio���
#if (NGX_HAVE_ALIGNED_DIRECTIO)
    unsigned                     unaligned:1;
#endif
    unsigned                     need_in_memory:1;  //�Ƿ���Ҫ���ڴ��б���һ��(ʹ��sendfile�Ļ����ڴ���û���ļ��Ŀ����ģ���������ʱ��Ҫ�����ļ�����ʱ����Ҫ����������)
    unsigned                     need_in_temp:1;    //�Ƿ���ڵ�buf����һ�ݣ����ﲻ���Ǵ������ڴ滹���ļ�
#if (NGX_HAVE_FILE_AIO)
    unsigned                     aio:1;

    ngx_output_chain_aio_pt      aio_handler;
#endif

    off_t                        alignment;

    ngx_pool_t                  *pool;              //�����
    ngx_int_t                    allocated;         //�Ѿ�allocated�Ĵ�С
    ngx_bufs_t                   bufs;              //��Ӧ��bufs�Ĵ�С�����ֵ��������loc conf�����õ�bufs
    ngx_buf_tag_t                tag;               //��ʾ���ڴ����Ǹ�ģ�飨��ΪupstreamҲ�����output_chain)

    ngx_output_chain_filter_pt   output_filter;     //���ֵһ����ngx_http_next_filter,Ҳ���Ǽ�������filter��
    void                        *filter_ctx;        //��ǰfilter�������ģ�����Ҳ������upstreamҲ�����output_chain
};


typedef struct {
    ngx_chain_t                 *out;
    ngx_chain_t                **last;
    ngx_connection_t            *connection;
    ngx_pool_t                  *pool;
    off_t                        limit;
} ngx_chain_writer_ctx_t;


#define NGX_CHAIN_ERROR     (ngx_chain_t *) NGX_ERROR

//����־λ��ȷ���������Ƿ����ڴ���
#define ngx_buf_in_memory(b)        (b->temporary || b->memory || b->mmap)
#define ngx_buf_in_memory_only(b)   (ngx_buf_in_memory(b) && !b->in_file)
//����Ƿ�����������õı�־λ
#define ngx_buf_special(b)                                                   \
    ((b->flush || b->last_buf || b->sync)                                    \
     && !ngx_buf_in_memory(b) && !b->in_file)

#define ngx_buf_sync_only(b)                                                 \
    (b->sync                                                                 \
     && !ngx_buf_in_memory(b) && !b->in_file && !b->flush && !b->last_buf)
//���㻺�����Ĵ�С
#define ngx_buf_size(b)                                                      \
    (ngx_buf_in_memory(b) ? (off_t) (b->last - b->pos):                      \
                            (b->file_last - b->file_pos))
//���ڴ���з���size��С�Ļ�����
ngx_buf_t *ngx_create_temp_buf(ngx_pool_t *pool, size_t size);
ngx_chain_t *ngx_create_chain_of_bufs(ngx_pool_t *pool, ngx_bufs_t *bufs);

//ֱ�Ӵ��ڴ���д���һ��ngx_buf_t�ṹ��Ȼ���ֹ�ָ����Ա
#define ngx_alloc_buf(pool)  ngx_palloc(pool, sizeof(ngx_buf_t))
#define ngx_calloc_buf(pool) ngx_pcalloc(pool, sizeof(ngx_buf_t))

ngx_chain_t *ngx_alloc_chain_link(ngx_pool_t *pool);
//���ڴ�����ͷ�ngx_chinat_t����
#define ngx_free_chain(pool, cl)                                             \
    cl->next = pool->chain;                                                  \
    pool->chain = cl



ngx_int_t ngx_output_chain(ngx_output_chain_ctx_t *ctx, ngx_chain_t *in);
ngx_int_t ngx_chain_writer(void *ctx, ngx_chain_t *in);

ngx_int_t ngx_chain_add_copy(ngx_pool_t *pool, ngx_chain_t **chain,
    ngx_chain_t *in);
ngx_chain_t *ngx_chain_get_free_buf(ngx_pool_t *p, ngx_chain_t **free);
void ngx_chain_update_chains(ngx_chain_t **free, ngx_chain_t **busy,
    ngx_chain_t **out, ngx_buf_tag_t tag);


#endif /* _NGX_BUF_H_INCLUDED_ */
