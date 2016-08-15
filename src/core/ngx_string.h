
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_STRING_H_INCLUDED_
#define _NGX_STRING_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


// ngx_str_t����ʾ�ַ������мǲ��ܰ�data�����ַ���������data��û�й涨��\0��β
// data+len �Ŵ����ַ��������������data�����ַ����������п��ܵ����ڴ�Խ�硣
// ��ʹ���ַ�������Ч�����ڴ�ʹ������
typedef struct {
    size_t      len;  //�ַ�������Ч����
    u_char     *data; //�ַ��������ݣ�ָ���ַ�������ʼλ��
} ngx_str_t;


typedef struct {
    ngx_str_t   key;  //key-value�ṹ,��
    ngx_str_t   value; //key-value�ṹ��ֵ
} ngx_keyval_t;

//�ο����ϣ�
//http://blog.csdn.net/dingyujie/article/details/7515904
typedef struct {
    unsigned    len:28;             //����ֵ�ĳ���

    unsigned    valid:1;            //�����Ƿ���Ч
    unsigned    no_cacheable:1;     /* �����Ƿ��ǿɻ���ģ�һ����˵��ĳЩ�����ڵ�һ�εõ�����ֵ�󣬺����ٴ��õ�ʱ������ֱ��ʹ����             
                                    * �ε�ֵ��������һЩ��ν��no_cacheable�ı���������Ҫ��ÿ��ʹ�õ�ʱ�򣬶�Ҫͨ��get_handler֮  
                                    * ��������ٴλ�ȡ  
                                    */
    unsigned    not_found:1;        //����û���ҵ���һ����ָĳ������û���ܹ�ͨ��get��ȡ�������ֵ
    unsigned    escape:1;           //����ֵ�Ƿ���Ҫ��ת�崦��

    u_char     *data;               //����ֵ
} ngx_variable_value_t;

//ͨ��һ���ԡ�0����β����ͨ�ַ���str����һ��nginx���ַ�����
//����api�в���sizeof�����������ַ������ȣ���˸�api�Ĳ���������һ�������ַ�����
#define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }

//��������ʱ����ʼ���ַ���Ϊ���ַ����������ĳ���Ϊ0��dataΪNULL��
#define ngx_null_string     { 0, NULL }

//�����ַ���strΪtext��text����Ϊ�����ַ�����
#define ngx_str_set(str, text)                                               \
    (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text

//�����ַ���strΪ�մ�������Ϊ0��dataΪNULL��
#define ngx_str_null(str)   (str)->len = 0; (str)->data = NULL

/*
ngx_string��ngx_null_stringֻ�����ڸ�ֵʱ��ʼ��
ngx_str_t str = ngx_string("hello world");
ngx_str_t str1 = ngx_null_string();

�������ʹ�ã��ͻ������⡣
ngx_str_t str, str1;
str = ngx_string("hello world");    // �������
str1 = ngx_null_string;                // �������

������������Ե���ngx_str_set��ngx_str_null��������������:
ngx_str_t str, str1;
ngx_str_set(&str, "hello world");
ngx_str_null(&str1);

����Ҫע����ǣ�ngx_string��ngx_str_set�ڵ���ʱ������ȥ���ַ���һ���ǳ����ַ����������õ����벻���Ĵ���(��Ϊngx_str_set�ڲ�ʹ����sizeof()������������u_char*����ô����������ָ��ĳ��ȣ��������ַ����ĳ���)���磺
ngx_str_t str;
u_char *a = "hello world";
ngx_str_set(&str, a);    // �������
*/

#define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

//��src��ǰn���ַ�ת����Сд�����dst�ַ�������
void ngx_strlow(u_char *dst, u_char *src, size_t n);
/*
��������Ҫ��֤dstָ��Ŀռ���ڵ���n�����������ԭ�ַ��������䶯����Ҫ����ԭ�ַ��������ԣ�

ngx_str_t str = ngx_string("hello world");
ngx_strlow(str->data, str->data, str->len);
*/

//���ִ�Сд���ַ����Ƚϣ�ֻ�Ƚ�ǰn���ַ���
#define ngx_strncmp(s1, s2, n)  strncmp((const char *) s1, (const char *) s2, n)


/* msvc and icc7 compile strcmp() to inline loop */
#define ngx_strcmp(s1, s2)  strcmp((const char *) s1, (const char *) s2)


#define ngx_strstr(s1, s2)  strstr((const char *) s1, (const char *) s2)
#define ngx_strlen(s)       strlen((const char *) s)

#define ngx_strchr(s1, c)   strchr((const char *) s1, (int) c)

static ngx_inline u_char *
ngx_strlchr(u_char *p, u_char *last, u_char c)
{
    while (p < last) {

        if (*p == c) {
            return p;
        }

        p++;
    }

    return NULL;
}


/*
 * msvc and icc7 compile memset() to the inline "rep stos"
 * while ZeroMemory() and bzero() are the calls.
 * icc7 may also inline several mov's of a zeroed register for small blocks.
 */
#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)  //ngx_memzeroʹ�õ���memsetԭ�ͣ�memsetʹ�û����б�д
#define ngx_memset(buf, c, n)     (void) memset(buf, c, n)


#if (NGX_MEMCPY_LIMIT)

void *ngx_memcpy(void *dst, void *src, size_t n);
#define ngx_cpymem(dst, src, n)   (((u_char *) ngx_memcpy(dst, src, n)) + (n))

#else

/*
 * gcc3, msvc, and icc7 compile memcpy() to the inline "rep movs".
 * gcc3 compiles memcpy(d, s, 4) to the inline "mov"es.
 * icc8 compile memcpy(d, s, 4) to the inline "mov"es or XMM moves.
 */
#define ngx_memcpy(dst, src, n)   (void) memcpy(dst, src, n)
#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n)) //���ص��ǿ������ݵ��յ㣬�����������������ʱ�ܷ���

#endif


#if ( __INTEL_COMPILER >= 800 )

/*
 * the simple inline cycle copies the variable length strings up to 16
 * bytes faster than icc8 autodetecting _intel_fast_memcpy()
 */

static ngx_inline u_char *
ngx_copy(u_char *dst, u_char *src, size_t len)
{
    if (len < 17) {

        while (len) {
            *dst++ = *src++;
            len--;
        }

        return dst;

    } else {
        return ngx_cpymem(dst, src, len);
    }
}

#else

#define ngx_copy                  ngx_cpymem

#endif


#define ngx_memmove(dst, src, n)   (void) memmove(dst, src, n)
#define ngx_movemem(dst, src, n)   (((u_char *) memmove(dst, src, n)) + (n))


/* msvc and icc7 compile memcmp() to the inline loop */
#define ngx_memcmp(s1, s2, n)  memcmp((const char *) s1, (const char *) s2, n)


u_char *ngx_cpystrn(u_char *dst, u_char *src, size_t n);
u_char *ngx_pstrdup(ngx_pool_t *pool, ngx_str_t *src);

u_char * ngx_cdecl ngx_sprintf(u_char *buf, const char *fmt, ...);
u_char * ngx_cdecl ngx_snprintf(u_char *buf, size_t max, const char *fmt, ...);
u_char * ngx_cdecl ngx_slprintf(u_char *buf, u_char *last, const char *fmt,
    ...);
/*
�������������������ַ�����ʽ����ngx_snprintf�ĵڶ�������maxָ��buf�Ŀռ��С��
ngx_slprintf��ͨ��last��ָ��buf�ռ�Ĵ�С���Ƽ�ʹ�õڶ������������������ʽ���ַ�����
ngx_sprintf�������ǱȽ�Σ�յģ����ײ������������©����
����һϵ�к����У�nginx�ڼ���glibc�и�ʽ���ַ�������ʽ֮�⣬
��������һЩ�����ʽ��nginx���͵�һЩת���ַ�������%V���ڸ�ʽ��ngx_str_t�ṹ��

�����ر�Ҫ���ѵ��ǣ���������ڸ�ʽ��ngx_str_t�ṹ�����Ӧ��ת�����%V������������һ��Ҫ��ָ�����ͣ��������ͻ�coredump������Ҳ�����������׷��Ĵ������磺

ngx_str_t str = ngx_string("hello world");
char buffer[1024];
ngx_snprintf(buffer, 1024, "%V", &str);    // ע�⣬strȡ��ַ
*/

u_char *ngx_vslprintf(u_char *buf, u_char *last, const char *fmt, va_list args);
#define ngx_vsnprintf(buf, max, fmt, args)                                   \
    ngx_vslprintf(buf, buf + (max), fmt, args)

//�����ִ�Сд�Ĳ������ȵ��ַ����Ƚϡ�
ngx_int_t ngx_strcasecmp(u_char *s1, u_char *s2);
//�����ִ�Сд�Ĵ����ȵ��ַ����Ƚϣ�ֻ�Ƚ�ǰn���ַ���
ngx_int_t ngx_strncasecmp(u_char *s1, u_char *s2, size_t n);

u_char *ngx_strnstr(u_char *s1, char *s2, size_t n);

u_char *ngx_strstrn(u_char *s1, char *s2, size_t n);
u_char *ngx_strcasestrn(u_char *s1, char *s2, size_t n);
u_char *ngx_strlcasestrn(u_char *s1, u_char *last, u_char *s2, size_t n);

ngx_int_t ngx_rstrncmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_rstrncasecmp(u_char *s1, u_char *s2, size_t n);
ngx_int_t ngx_memn2cmp(u_char *s1, u_char *s2, size_t n1, size_t n2);
ngx_int_t ngx_dns_strcmp(u_char *s1, u_char *s2);

ngx_int_t ngx_atoi(u_char *line, size_t n);
ngx_int_t ngx_atofp(u_char *line, size_t n, size_t point);
ssize_t ngx_atosz(u_char *line, size_t n);
off_t ngx_atoof(u_char *line, size_t n);
time_t ngx_atotm(u_char *line, size_t n);
ngx_int_t ngx_hextoi(u_char *line, size_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, size_t len);

//base64 ���룯���뺯���ͺ�
#define ngx_base64_encoded_length(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_length(len)  (((len + 3) / 4) * 3)
//��׼base64�ı����
void ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
/*
�������������ڶ�str����base64��������룬
����ǰ����Ҫ��֤dst�����㹻�Ŀռ�����Ž����
�����֪�������С��
���ȵ���ngx_base64_encoded_length��ngx_base64_decoded_length��Ԥ�����ռ�ÿռ䡣
*/

ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);

//utf-8 ���룯������غ���
uint32_t ngx_utf8_decode(u_char **p, size_t n);
size_t ngx_utf8_length(u_char *p, size_t n);
u_char *ngx_utf8_cpystrn(u_char *dst, u_char *src, size_t n, size_t len);


#define NGX_ESCAPE_URI            0
#define NGX_ESCAPE_ARGS           1
#define NGX_ESCAPE_URI_COMPONENT  2
#define NGX_ESCAPE_HTML           3
#define NGX_ESCAPE_REFRESH        4
#define NGX_ESCAPE_MEMCACHED      5
#define NGX_ESCAPE_MAIL_AUTH      6

#define NGX_UNESCAPE_URI       1
#define NGX_UNESCAPE_REDIRECT  2

//��src���б��룬����type������ͬ�ķ�ʽ���б��룬
//���dstΪNULL���򷵻���Ҫת����ַ����������ɴ˿ɵõ���Ҫ�Ŀռ��С��
uintptr_t ngx_escape_uri(u_char *dst, u_char *src, size_t size,
    ngx_uint_t type);

//��src���з����룬type������0��NGX_UNESCAPE_URI��NGX_UNESCAPE_REDIRECT������ֵ��
//�����0�����ʾsrc�е������ַ���Ҫ����ת�롣
//�����NGX_UNESCAPE_URI��NGX_UNESCAPE_REDIRECT����������?����ͽ����ˣ�������ַ��Ͳ����ˡ�
//��NGX_UNESCAPE_URI��NGX_UNESCAPE_REDIRECT֮���������NGX_UNESCAPE_URI������������Ҫת����ַ���
//����ת�룬��NGX_UNESCAPE_REDIRECT��ֻ��Էǿɼ��ַ�����ת�롣
void ngx_unescape_uri(u_char **dst, u_char **src, size_t size, ngx_uint_t type);

//��html��ǩ���б��롣
uintptr_t ngx_escape_html(u_char *dst, u_char *src, size_t size);


typedef struct {
    ngx_rbtree_node_t         node;
    ngx_str_t                 str;
} ngx_str_node_t;


void ngx_str_rbtree_insert_value(ngx_rbtree_node_t *temp,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
ngx_str_node_t *ngx_str_rbtree_lookup(ngx_rbtree_t *rbtree, ngx_str_t *name,
    uint32_t hash);


void ngx_sort(void *base, size_t n, size_t size,
    ngx_int_t (*cmp)(const void *, const void *));
#define ngx_qsort             qsort


#define ngx_value_helper(n)   #n
#define ngx_value(n)          ngx_value_helper(n)


#endif /* _NGX_STRING_H_INCLUDED_ */