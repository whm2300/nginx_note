
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

/*
ngx_hash_t��nginx�Լ���hash���ʵ�֡������ʵ��λ��src/core/ngx_hash.h|c�С�ngx_hash_t��ʵ��Ҳ�����ݽṹ�̿�������������hash���ʵ���Ǵ�ͬС�졣���ڳ��õĽ����ͻ�ķ���������̽�⣬����̽��Ϳ������ȡ�ngx_hash_tʹ�õ�����õ�һ�֣�Ҳ���ǿ���������Ҳ��STL�е�hash��ʹ�õķ�����

����ngx_hash_t��ʵ�������伸�����ŵ��ص�:

ngx_hash_t����������hash���ʵ�֣����Բ���ɾ��Ԫ�أ���ֻ��һ�γ�ʼ�����͹���������hash���Ժ󣬼Ȳ�����ɾ����Ҳ�����ڲ���Ԫ���ˡ�
ngx_hash_t�Ŀ�����������Ŀ���һ������ʵ�����ǿ���һ�������Ĵ洢�ռ䣬�������Կ�����һ�����顣������Ϊngx_hash_t�ڳ�ʼ����ʱ�򣬻ᾭ��һ��Ԥ����Ĺ��̣���ǰ��ÿ��Ͱ������ж���Ԫ�طŽ�ȥ�������������������ǰ֪��ÿ��Ͱ�Ĵ�С�ˡ���ô�Ͳ���Ҫʹ������һ�������Ĵ洢�ռ���㹻�ˡ���Ҳ��һ���̶��Ͻ�ʡ���ڴ��ʹ�á�
����������������ǿ��Կ����������ֵԽ��Խ����ڴ���˷ѡ��������������ǳ�ʼ����Ȼ��Ϳ�����������в����ˡ�����������ϸ����һ�¡�
 */


//�ο���
//1. http://blog.csdn.net/livelylittlefish/article/details/6636229
//2. http://blog.csdn.net/chen19870707/article/details/40794285
//3. http://my.oschina.net/chenzhuo/blog/177866
//4. http://tengine.taobao.org/book/chapter_02.html#ngx-hash-t-100
//hashԪ�ؽṹ
typedef struct {
    void             *value;    //value����ĳ��key��Ӧ��ֵ����<key,value>�е�value
    u_short           len;      //name����
    u_char            name[1];  //ĳ��Ҫhash������(��nginx�б���Ϊ�ַ���)����<key,value>�е�key
    // �������鳤��Ϊ1����һ��С���ɡ�ʵ��ʱ���ھ������ngx_hash_elt_t�Ĵ�Сʱʹ�ú�NGX_HASH_ELT_SIZE��ȷ��(�������ڴ�����)��
    // #define NGX_HASH_ELT_SIZE(name) (sizeof(void *) + ngx_align((name)->key.len + 2, sizeof(void *)))
} ngx_hash_elt_t;


//hash�ṹ
typedef struct {
    ngx_hash_elt_t  **buckets; //hashͰ(��size��Ͱ)
    ngx_uint_t        size;    //hashͰ����
} ngx_hash_t;

/*
nginxΪ�˴������ͨ�����������ƥ�����⣬ʵ����ngx_hash_wildcard_t������hash��
������֧���������͵Ĵ���ͨ�����������
һ����ͨ�����ǰ�ģ����磺��*.abc.com����Ҳ����ʡ�Ե��Ǻţ�ֱ��д�ɡ�.abc.com����
������key������ƥ��www.abc.com��qqq.www.abc.com֮��ġ�
����һ����ͨ�����ĩβ�ģ����磺��mail.xxx.*�������ر�ע��ͨ�����ĩβ�Ĳ���λ�ڿ�ʼ��ͨ������Ա�ʡ�Ե���
������ͨ���������ƥ��mail.xxx.com��mail.xxx.com.cn��mail.xxx.net֮���������

��һ�����˵��������һ��ngx_hash_wildcard_t���͵�hash��ֻ�ܰ���ͨ�����ǰ��key������ͨ����ں��key��
����ͬʱ�����������͵�ͨ�����key��ngx_hash_wildcard_t���ͱ����Ĺ�����ͨ������ngx_hash_wildcard_init��ɵģ�
����ѯ��ͨ������ngx_hash_find_wc_head����ngx_hash_find_wc_tail�����ġ�
ngx_hash_find_wc_head�ǲ�ѯ����ͨ�����ǰ��key��hash��ģ���ngx_hash_find_wc_tail�ǲ�ѯ����ͨ����ں��key��hash��ġ�
 */
typedef struct {
    // ����ɢ�б�
    ngx_hash_t        hash;
    // ��ʹ�����ngx_hash_wildcard_tͨ���ɢ�б���Ϊĳ������Ԫ��ʱ������ʹ�����valueָ��ָ���û�����
    void             *value;
} ngx_hash_wildcard_t;


// <key,value> �ṹ
typedef struct {
    ngx_str_t         key;      //key��Ϊnginx���ַ����ṹ
    ngx_uint_t        key_hash; //�ɸ�key�������hashֵ(ͨ��hash������ngx_hash_key_lc())
    void             *value;    //��key��Ӧ��ֵ�����һ����-ֵ��<key,value>
} ngx_hash_key_t;


typedef ngx_uint_t (*ngx_hash_key_pt) (u_char *data, size_t len);


typedef struct {
    // ���ھ�ȷƥ��Ļ���ɢ�б�
    ngx_hash_t            hash;
    // ���ڲ�ѯǰ��ͨ�����ɢ�б�
    ngx_hash_wildcard_t  *wc_head;
    // ���ڲ�ѯ����ͨ�����ɢ�б�
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;


/*
hash:	���ֶ����ΪNULL����ô�������ʼ�������󣬸��ֶ�ָ���´���������hash��������ֶβ�ΪNULL����ô�ڳ�ʼ��ʱ�����е����ݱ�����������ֶ���ָ��hash���С�
key:	ָ����ַ�������hashֵ��hash������nginx��Դ�������ṩ��Ĭ�ϵ�ʵ�ֺ���ngx_hash_key_lc��
max_size:	hash���е�Ͱ�ĸ��������ֶ�Խ��Ԫ�ش洢ʱ��ͻ�Ŀ�����ԽС��ÿ��Ͱ�д洢��Ԫ�ػ���٣����ѯ�������ٶȸ��졣��Ȼ�����ֵԽ��Խ����ڴ���˷�ҲԽ��(ʵ����Ҳ�˷Ѳ��˶���)��
bucket_size:	ÿ��Ͱ��������ƴ�С����λ���ֽڡ�����ڳ�ʼ��һ��hash���ʱ�򣬷���ĳ��Ͱ�����޷�������������ڸ�Ͱ��Ԫ�أ���hash���ʼ��ʧ�ܡ�
name:	��hash������֡�
pool:	��hash������ڴ�ʹ�õ�pool��
temp_pool:	��hash��ʹ�õ���ʱpool���ڳ�ʼ������Ժ󣬸�pool���Ա��ͷź����ٵ���
 */
//hash��ʼ���ṹ����������������ݷ�װ������Ϊ�������ݸ�ngx_hash_init()��ngx_hash_wildcard_init()����
typedef struct {
    ngx_hash_t       *hash;         //ָ�����ʼ����hash�ṹ��
    ngx_hash_key_pt   key;          //hash����ָ��

    // ɢ�б��в۵������Ŀ
    ngx_uint_t        max_size;     //bucket��������

    // ɢ�б���һ���۵Ŀռ��С����������ÿ��ɢ�б�Ԫ�عؼ��ֵ���󳤶ȣ�ͨ��NGX_HASH_ELT_SIZE(name)����ÿ��element�Ĵ�С��
    // ������bucket_size���ýϴ���ô�����ܹ����ɶ��element������һ��bucket���Ŷ��element���������²����ٶ��½���
    // Ϊ�˸��õĲ����ٶȣ��뽫bucket_size����Ϊ����element���������Ǹ���
    ngx_uint_t        bucket_size;

    // ɢ�б������
    char             *name;         //��hash�ṹ������(���ڴ�����־��ʹ��)
    // �ڴ�أ�������ɢ�б����3��������1����ͨɢ�б�1��ǰ��ͨ���ɢ�б�1������ͨ���ɢ�б��е����в�
    ngx_pool_t       *pool;         //��hash�ṹ��poolָ����ڴ���з���
    // ��ʱ�ڴ�أ����������ڳ�ʼ��ɢ�б�֮ǰ������Ҫ���ڷ���һЩ��ʱ�Ķ�̬���飬��ͨ�����Ԫ���ڳ�ʼ��ʱ��Ҫ�õ���Щ���顣
    ngx_pool_t       *temp_pool;    //������ʱ���ݿռ���ڴ��
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_KEY     1
#define NGX_HASH_READONLY_KEY     2


typedef struct {
    // �����keys_hash, dns_wc_head_hash,dns_wc_tail_hash���Ǽ���ɢ�б���hsizeָ����ɢ�б�Ĳ۸����������ɢ�з���Ҳ��Ҫ��hsize����
    ngx_uint_t        hsize;

    // �ڴ�أ����ڷ����������ڴ棬��Ŀǰ��nginx�汾Ϊֹ����pool��Աû���κ�����
    ngx_pool_t       *pool;
    // ��ʱ�ڴ�أ�����Ķ�̬������Ҫ���ڴ涼��temp_pool�ڴ�ط���
    ngx_pool_t       *temp_pool;

    // �ö�̬������ngx_hash_key_t�ṹ�屣���Ų�����ͨ����ؼ��ֵ�Ԫ��
    ngx_array_t       keys;
    /* һ��������׵�ɢ�б������������ʽ������hsize��Ԫ�أ�ÿ��Ԫ�ض���ngx_array_t��̬���飬���û���ӵ�Ԫ�ع����У�����ݹؼ���
    ���û���ngx_str_t���͵Ĺؼ�����ӵ�ngx_array_t ��̬�����У��������е��û�Ԫ�صĹؼ��ֶ������Դ�ͨ�������ʾ��ȷƥ�� */
    ngx_array_t      *keys_hash;

    // �ö�̬������ngx_hash_key_t �ṹ�屣���ź���ǰ��ͨ����ؼ��ֵ�Ԫ�����ɵ��м�ؼ���
    ngx_array_t       dns_wc_head;
    // һ��������׵�ɢ�б������������ʽ������hsize��Ԫ�أ�ÿ��Ԫ�ض���ngx_array_t ��̬���顣���û����Ԫ�ع����У�����ݹؼ��뽫�û���
    // ngx_str_t���͵Ĺؼ�����ӵ�ngx_array_t ��̬�����С��������е��û�Ԫ�صĹؼ��ֶ���ǰ��ͨ�����
    ngx_array_t      *dns_wc_head_hash;

    // �ö�̬������ngx_hash_key_t �ṹ�屣���ź���ǰ��ͨ����ؼ��ֵ�Ԫ�����ɵ��м�ؼ���
    ngx_array_t       dns_wc_tail;
    /*
    һ�����佨���ɢ�б������������ʽ������hsize��Ԫ�أ�ÿ��Ԫ�ض���ngx_array_t��̬���顣���û����Ԫ�ع����У�����ݹؼ��뽫�û�
    ��ngx_str_t ���͵Ĺؼ�����ӵ�ngx_array_t ��̬�����У��������е��û�Ԫ�صĹؼ��ֶ�������ͨ�����
    */
    ngx_array_t      *dns_wc_tail_hash;
} ngx_hash_keys_arrays_t;


// ngx_table_elt_t��һ��key/value�ԣ�ngx_str_t���͵�key��value
//�ýṹ��Ҫ������ʾHTTPͷ����Ϣ������"server:nginx/1.8.0",����key=server,value="nginx/1.8.0"
typedef struct {
    ngx_uint_t        hash; //������ngx_hash_t��ĳ�Ա��ʱ�����ڿ��ټ���ͷ��
    ngx_str_t         key;  //�����ַ���
    ngx_str_t         value; //ֵ�ַ���
    u_char           *lowcase_key; //ȫСд��key�ַ���
} ngx_table_elt_t;


//hash����
void *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t key, u_char *name, size_t len);


//�ú�����ѯ����ͨ�����ǰ��key��hash��ġ�
//hwc:	hash������ָ�롣
//name:	��Ҫ��ѯ������������: www.abc.com��
//len:	name�ĳ��ȡ�
//�ú�������ƥ���ͨ�����Ӧvalue�����û�в鵽������NULL��
void *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);

//�ú�����ѯ����ͨ�����ĩβ��key��hash��ġ� ����������ֵ��μ��ϸ�������˵����
void *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t key,
    u_char *name, size_t len);


ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);

/*
�ú���ӭ������һ�����԰���ͨ���key��hash��
hinit:	����һ��ͨ���hash���һЩ������һ�����ϡ����ڸò�����Ӧ�����͵�˵������μ�ngx_hash_t������ngx_hash_init������˵����
names:	�����hash������е�ͨ���key�����顣
	�ر�Ҫע����������key�Ѿ����Ǳ�Ԥ������ġ�
	���磺��*.abc.com�����ߡ�.abc.com����Ԥ��������Ժ󣬱���ˡ�com.abc.����
	����mail.xxx.*����Ԥ����Ϊ��mail.xxx.����Ϊʲô�ᱻ�������������ﲻ�ò��򵥵�����һ��ͨ���hash���ʵ��ԭ��
	����������͵�hash���ʱ��ʵ�����ǹ�����һ��hash���һ������������ͨ��hash���е�key�����ӡ������ġ�
	���磺���ڡ�*.abc.com�����ṹ���2��hash����һ��hash������һ��keyΪcom�ı���ñ����value������ָ��ڶ���hash���ָ�룬
	���ڶ���hash������һ������abc���ñ����value������ָ��*.abc.com��Ӧ��value��ָ�롣
	��ô��ѯ��ʱ�򣬱����ѯwww.abc.com��ʱ���Ȳ�com��ͨ����com�����ҵ��ڶ�����hash��
	�ڵڶ���hash���У��ٲ���abc���������ƣ�ֱ����ĳһ����hash���в鵽�ı����Ӧ��value��Ӧһ��������ֵ����һ��ָ����һ��hash���ָ���ʱ�򣬲�ѯ���̽�����
	������һ����Ҫ�ر�ע��ģ�����names������Ԫ�ص�valueֵ����λbit����Ϊ0����������;����
	���������������������hash���ѯ������ȷ�����
nelts:	names����Ԫ�صĸ�����
�ú���ִ�гɹ�����NGX_OK������NGX_ERROR��
 */
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);

#define ngx_hash(key, c)   ((ngx_uint_t) key * 31 + c)
ngx_uint_t ngx_hash_key(u_char *data, size_t len);
//lc��ʾlower case�����ַ���ת��ΪСд���ټ���hashֵ
ngx_uint_t ngx_hash_key_lc(u_char *data, size_t len);  
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_keys_array_init(ngx_hash_keys_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_key(ngx_hash_keys_arrays_t *ha, ngx_str_t *key,
    void *value, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
