
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HTTP_REQUEST_H_INCLUDED_
#define _NGX_HTTP_REQUEST_H_INCLUDED_


#define NGX_HTTP_MAX_URI_CHANGES           10
#define NGX_HTTP_MAX_SUBREQUESTS           50

/* must be 2^n */
#define NGX_HTTP_LC_HEADER_LEN             32


#define NGX_HTTP_DISCARD_BUFFER_SIZE       4096
#define NGX_HTTP_LINGERING_BUFFER_SIZE     4096

//HTTPЭ��汾��
#define NGX_HTTP_VERSION_9                 9
#define NGX_HTTP_VERSION_10                1000
#define NGX_HTTP_VERSION_11                1001

#define NGX_HTTP_UNKNOWN                   0x0001
#define NGX_HTTP_GET                       0x0002
#define NGX_HTTP_HEAD                      0x0004
#define NGX_HTTP_POST                      0x0008
#define NGX_HTTP_PUT                       0x0010
#define NGX_HTTP_DELETE                    0x0020
#define NGX_HTTP_MKCOL                     0x0040
#define NGX_HTTP_COPY                      0x0080
#define NGX_HTTP_MOVE                      0x0100
#define NGX_HTTP_OPTIONS                   0x0200
#define NGX_HTTP_PROPFIND                  0x0400
#define NGX_HTTP_PROPPATCH                 0x0800
#define NGX_HTTP_LOCK                      0x1000
#define NGX_HTTP_UNLOCK                    0x2000
#define NGX_HTTP_PATCH                     0x4000
#define NGX_HTTP_TRACE                     0x8000

#define NGX_HTTP_CONNECTION_CLOSE          1
#define NGX_HTTP_CONNECTION_KEEP_ALIVE     2


#define NGX_NONE                           1


#define NGX_HTTP_PARSE_HEADER_DONE         1

#define NGX_HTTP_CLIENT_ERROR              10
#define NGX_HTTP_PARSE_INVALID_METHOD      10
#define NGX_HTTP_PARSE_INVALID_REQUEST     11
#define NGX_HTTP_PARSE_INVALID_09_METHOD   12

#define NGX_HTTP_PARSE_INVALID_HEADER      13


/* unused                                  1 */
#define NGX_HTTP_SUBREQUEST_IN_MEMORY      2
#define NGX_HTTP_SUBREQUEST_WAITED         4
#define NGX_HTTP_LOG_UNSAFE                8

//2XX״̬��
#define NGX_HTTP_OK                        200
#define NGX_HTTP_CREATED                   201
#define NGX_HTTP_ACCEPTED                  202
#define NGX_HTTP_NO_CONTENT                204
#define NGX_HTTP_PARTIAL_CONTENT           206
//3XX״̬��
#define NGX_HTTP_SPECIAL_RESPONSE          300
#define NGX_HTTP_MOVED_PERMANENTLY         301
#define NGX_HTTP_MOVED_TEMPORARILY         302
#define NGX_HTTP_SEE_OTHER                 303
#define NGX_HTTP_NOT_MODIFIED              304
#define NGX_HTTP_TEMPORARY_REDIRECT        307
//4XX״̬��
#define NGX_HTTP_BAD_REQUEST               400
#define NGX_HTTP_UNAUTHORIZED              401
#define NGX_HTTP_FORBIDDEN                 403
#define NGX_HTTP_NOT_FOUND                 404
#define NGX_HTTP_NOT_ALLOWED               405
#define NGX_HTTP_REQUEST_TIME_OUT          408
#define NGX_HTTP_CONFLICT                  409
#define NGX_HTTP_LENGTH_REQUIRED           411
#define NGX_HTTP_PRECONDITION_FAILED       412
#define NGX_HTTP_REQUEST_ENTITY_TOO_LARGE  413
#define NGX_HTTP_REQUEST_URI_TOO_LARGE     414
#define NGX_HTTP_UNSUPPORTED_MEDIA_TYPE    415
#define NGX_HTTP_RANGE_NOT_SATISFIABLE     416


/* Our own HTTP codes */

/* The special code to close connection without any response */
#define NGX_HTTP_CLOSE                     444

#define NGX_HTTP_NGINX_CODES               494

#define NGX_HTTP_REQUEST_HEADER_TOO_LARGE  494

#define NGX_HTTPS_CERT_ERROR               495
#define NGX_HTTPS_NO_CERT                  496

/*
 * We use the special code for the plain HTTP requests that are sent to
 * HTTPS port to distinguish it from 4XX in an error page redirection
 */
#define NGX_HTTP_TO_HTTPS                  497

/* 498 is the canceled code for the requests with invalid host name */

/*
 * HTTP does not define the code for the case when a client closed
 * the connection while we are processing its request so we introduce
 * own code to log such situation when a client has closed the connection
 * before we even try to send the HTTP header to it
 */
#define NGX_HTTP_CLIENT_CLOSED_REQUEST     499


#define NGX_HTTP_INTERNAL_SERVER_ERROR     500
#define NGX_HTTP_NOT_IMPLEMENTED           501
#define NGX_HTTP_BAD_GATEWAY               502
#define NGX_HTTP_SERVICE_UNAVAILABLE       503
#define NGX_HTTP_GATEWAY_TIME_OUT          504
#define NGX_HTTP_INSUFFICIENT_STORAGE      507


#define NGX_HTTP_LOWLEVEL_BUFFERED         0xf0
#define NGX_HTTP_WRITE_BUFFERED            0x10
#define NGX_HTTP_GZIP_BUFFERED             0x20
#define NGX_HTTP_SSI_BUFFERED              0x01
#define NGX_HTTP_SUB_BUFFERED              0x02
#define NGX_HTTP_COPY_BUFFERED             0x04


typedef enum {
    NGX_HTTP_INITING_REQUEST_STATE = 0,
    NGX_HTTP_READING_REQUEST_STATE,
    NGX_HTTP_PROCESS_REQUEST_STATE,

    NGX_HTTP_CONNECT_UPSTREAM_STATE,
    NGX_HTTP_WRITING_UPSTREAM_STATE,
    NGX_HTTP_READING_UPSTREAM_STATE,

    NGX_HTTP_WRITING_REQUEST_STATE,
    NGX_HTTP_LINGERING_CLOSE_STATE,
    NGX_HTTP_KEEPALIVE_STATE
} ngx_http_state_e;


typedef struct {
    ngx_str_t                         name;
    ngx_uint_t                        offset;
    ngx_http_header_handler_pt        handler;
} ngx_http_header_t;


typedef struct {
    ngx_str_t                         name;
    ngx_uint_t                        offset;
} ngx_http_header_out_t;

//�ýṹ��洢�Ѿ�������HTTPͷ��
typedef struct {
	/*���н�������HTTPͷ������headers�����У������������Ԫ�ض���ngx_table_elt_t��Ա*/
    ngx_list_t                        headers;
	/*����ÿ��ngx_table_elt_t��Ա����RFC1616�淶�����HTTPͷ��������ʵ����ָ��headers�����еĳ�Ա��ע�⣬������Ϊ��ʱ����ʾû�н�������Ӧ��HTTPͷ��*/
    ngx_table_elt_t                  *host;//hostͷ
    ngx_table_elt_t                  *connection;//connectionͷ
    ngx_table_elt_t                  *if_modified_since;
    ngx_table_elt_t                  *if_unmodified_since;
    ngx_table_elt_t                  *user_agent;
    ngx_table_elt_t                  *referer;
    ngx_table_elt_t                  *content_length;
    ngx_table_elt_t                  *content_type;

    ngx_table_elt_t                  *range;
    ngx_table_elt_t                  *if_range;

    ngx_table_elt_t                  *transfer_encoding;
    ngx_table_elt_t                  *expect;

#if (NGX_HTTP_GZIP)
    ngx_table_elt_t                  *accept_encoding;
    ngx_table_elt_t                  *via;
#endif

    ngx_table_elt_t                  *authorization;

    ngx_table_elt_t                  *keep_alive;

#if (NGX_HTTP_PROXY || NGX_HTTP_REALIP || NGX_HTTP_GEO)
    ngx_table_elt_t                  *x_forwarded_for;
#endif

#if (NGX_HTTP_REALIP)
    ngx_table_elt_t                  *x_real_ip;
#endif

#if (NGX_HTTP_HEADERS)
    ngx_table_elt_t                  *accept;
    ngx_table_elt_t                  *accept_language;
#endif

#if (NGX_HTTP_DAV)
    ngx_table_elt_t                  *depth;
    ngx_table_elt_t                  *destination;
    ngx_table_elt_t                  *overwrite;
    ngx_table_elt_t                  *date;
#endif

    ngx_str_t                         user;
    ngx_str_t                         passwd;

    ngx_array_t                       cookies; //cookie

    ngx_str_t                         server;
    off_t                             content_length_n;
    time_t                            keep_alive_n;

    unsigned                          connection_type:2;
    unsigned                          msie:1;  //���������
    unsigned                          msie6:1;
    unsigned                          opera:1;
    unsigned                          gecko:1;
    unsigned                          chrome:1;
    unsigned                          safari:1;
    unsigned                          konqueror:1;
} ngx_http_headers_in_t;


//ngx_http_headers_out_t ��Ӧͷ
typedef struct {
    //�����͵�HTTPͷ������
    ngx_list_t                        headers;

    ngx_uint_t                        status; //http��Ӧ״̬������200
    ngx_str_t                         status_line; //��Ӧ��״̬�У����硰HTTP/1.1 201 CREATED��

    //�����Ա����RFC1616�ж����HTTPͷ����
    ngx_table_elt_t                  *server;
    ngx_table_elt_t                  *date;
    ngx_table_elt_t                  *content_length;
    ngx_table_elt_t                  *content_encoding;
    ngx_table_elt_t                  *location;
    ngx_table_elt_t                  *refresh;
    ngx_table_elt_t                  *last_modified;
    ngx_table_elt_t                  *content_range;
    ngx_table_elt_t                  *accept_ranges;
    ngx_table_elt_t                  *www_authenticate;
    ngx_table_elt_t                  *expires;
    ngx_table_elt_t                  *etag;

    ngx_str_t                        *override_charset;

    //���Ե���ngx_http_set_content_type(r)����������������Content-Typeͷ����
    //������������URI�е��ļ���չ������Ӧ��mime.type������Content-typeֵ
    size_t                            content_type_len;
    ngx_str_t                         content_type;
    ngx_str_t                         charset;
    u_char                           *content_type_lowcase;
    ngx_uint_t                        content_type_hash;

    ngx_array_t                       cache_control;

    //����ָ����content_length_n�󣬲����ٵ�ngx_table_elt_t��������
    off_t                             content_length_n; //��Ӧ���ݳ���
    time_t                            date_time;
    time_t                            last_modified_time;
} ngx_http_headers_out_t;

//ngx_http_headers_out_tʹ��ʾ����
/*
ngx_table_elt_t *h = ngx_list_push(&r->headers_out.headers)
if(h == NULL) {
    return NGX_ERROR;
}

h->hash=1
h->key.len = sizeof("TestHead") - 1;
h->key.data = (u_char *) "TestHead";
h->value.len = sizeof("TestValue") - 1;
h->value.data = (u_char *) "TestValue";

����������Ӧ������һ��HTTPͷ��
TestHead: TestValue\r\n
*/

typedef void (*ngx_http_client_body_handler_pt)(ngx_http_request_t *r);

typedef struct {
    // ���HTTP�������ʱ�ļ�
    ngx_temp_file_t                  *temp_file;

    // ����HTTP����Ļ�����������������Ҫȫ��������ڴ���ʱ�����һ��ngx_buf_t�������޷�����꣬��ʱ����Ҫʹ��ngx_chain_t������
    ngx_chain_t                      *bufs;

    // ֱ�ӽ���HTTP����Ļ���
    ngx_buf_t                        *buf;

    // ����content-lengthͷ�����ѽ��յ��İ��峤�ȣ�������Ļ���Ҫ���յİ��峤��
    off_t                             rest;

    // �û������������Ž�Ҫд���ļ��İ���
    ngx_chain_t                      *to_write;

    // HTTP���������Ϻ�ִ�еĻص�������Ҳ����ngx_http_read_client_request_body �������ݵĵڶ�������
    ngx_http_client_body_handler_pt   post_handler;
} ngx_http_request_body_t;


typedef struct {
    ngx_http_request_t               *request;

    ngx_buf_t                       **busy;
    ngx_int_t                         nbusy;

    ngx_buf_t                       **free;
    ngx_int_t                         nfree;

    ngx_uint_t                        pipeline;    /* unsigned  pipeline:1; */
} ngx_http_connection_t;


typedef struct ngx_http_server_name_s  ngx_http_server_name_t;


typedef struct {
     ngx_hash_combined_t              names;

     ngx_uint_t                       nregex;
     ngx_http_server_name_t          *regex;
} ngx_http_virtual_names_t;


typedef void (*ngx_http_cleanup_pt)(void *data);

typedef struct ngx_http_cleanup_s  ngx_http_cleanup_t;

struct ngx_http_cleanup_s {
    ngx_http_cleanup_pt               handler;
    void                             *data;
    ngx_http_cleanup_t               *next;
};


typedef ngx_int_t (*ngx_http_post_subrequest_pt)(ngx_http_request_t *r,
    void *data, ngx_int_t rc);

typedef struct {
    ngx_http_post_subrequest_pt       handler;
    void                             *data;
} ngx_http_post_subrequest_t;


typedef struct ngx_http_postponed_request_s  ngx_http_postponed_request_t;

struct ngx_http_postponed_request_s {
    ngx_http_request_t               *request;
    ngx_chain_t                      *out;
    ngx_http_postponed_request_t     *next;
};


typedef struct ngx_http_posted_request_s  ngx_http_posted_request_t;

struct ngx_http_posted_request_s {
    // ָ��ǰ�������������ngx_http_request_t�ṹ��
    ngx_http_request_t               *request;

    // ָ����һ�����������û�У���ΪNULL
    ngx_http_posted_request_t        *next;
};

// ������ԭ�ͣ��κ�handlerģ��Ҫ�봦��HTTP���󶼱���ʵ���������
typedef ngx_int_t (*ngx_http_handler_pt)(ngx_http_request_t *r);
typedef void (*ngx_http_event_handler_pt)(ngx_http_request_t *r);

// ����ṹ������һ��HTTP����,�ýṹ�ᴩ��http������������̣�����˵����http������ǲ���ngx_http_request_t���ݽṹ
struct ngx_http_request_s {
    uint32_t                          signature;         /* "HTTP" */

    // ��������Ӧ�Ŀͻ�������
    ngx_connection_t                 *connection;  //��ǰrequest������

    // ָ��������HTTPģ��������Ľṹ���ָ�����飬�û����Խ�˽�����ݱ���������
    void                            **ctx;  //������

    // ָ�������Ӧ�Ĵ��main�������ýṹ���ָ������
    void                            **main_conf; //main����

    // ָ�������Ӧ�Ĵ��srv�������ýṹ���ָ������
    void                            **srv_conf;  //srv����

    // ָ�������Ӧ�Ĵ��loc�������ýṹ���ָ������
    void                            **loc_conf;  //loc����

    /*
    �ڽ�����HTTPͷ������һ����ҵ���ϴ���HTTP����ʱ��HTTP����ṩ�Ĵ�������ngx_http_process_request��
    ������÷����޷�һ�δ�����������ȫ��ҵ���ڹ黹����Ȩ��epoll�¼�ģ��󣬸������ٴα��ص�ʱ����ͨ��ngx_http_request_handler����������
    ����������ж��ڿɶ��¼��Ĵ�����ǵ���read_event_handler��������Ҳ����˵��HTTPģ��ϣ���ڵײ㴦������Ķ��¼�������ʵ��read_evet_handler������
    */
    /*nginxʹ�õ���reqctorģʽ�����������xxx_event_handlerʵ���Ͼ���ע�ᵽ��Ӧʱ���handler*/
    ngx_http_event_handler_pt         read_event_handler;

    /*
    ��read_event_handler�ص��������ƣ����ngx_http_request_handler�����жϵ�ǰ�¼��ǿ�д�¼��������write_event_handler��������
    */
    ngx_http_event_handler_pt         write_event_handler;

#if (NGX_HTTP_CACHE)
    ngx_http_cache_t                 *cache;
#endif

    // upstream�����õ��Ľṹ��
    ngx_http_upstream_t              *upstream;  //load-balance�����ģ����load-balance�Ļ��������
    ngx_array_t                      *upstream_states;
                                         /* of ngx_http_upstream_state_t */

    /*
    ��ʾ���������ڴ�أ���ngx_http_free_request ���������١�����ngx_connection_t�е��ڴ�����岻ͬ���������ͷ�ʱ��
    TCP���ӿ��ܲ�û�йرգ���ʱ������ڴ�ػ����٣���ngx_connection_t���ڴ�ز�������
    */
    ngx_pool_t                       *pool;     //���ӳ�

    // ���ڽ���HTTP�������ݵĻ���������Ҫ���ڽ���HTTPͷ��
    ngx_buf_t                        *header_in;

    /*
    ngx_http_prcess_request_headers �����ڽ��գ�������HTTP�����ͷ���󣬻�ѽ������ÿ��HTTPͷ�����뵽headers_in��headers�����У�
    ͬʱ�ṹ��headers_in�е�������Ա
    */
    ngx_http_headers_in_t             headers_in; //request��header
    /*
    HTTPģ������Ҫ���͵�HTTP��Ӧ��Ϣ�ŵ�headers_out�У�����HTTP��ܽ�headers_out�еĳ�Ա���л�ΪHTTP��Ӧ�����͸��û�
    */
    ngx_http_headers_out_t            headers_out; //response��header��ʹ��ngx_http_send_header����

    // ����HTTP�����а�������ݽṹ
    ngx_http_request_body_t          *request_body; //response��body

    // �ӳٹر����ӵ�ʱ��
    time_t                            lingering_time;

    /*
    ��ǰ�����ʼ��ʱ��ʱ�䡣���������������������ʱ���������������ʱ�䣻�������������û��������������ǽ�����TCP���Ӻ󣬵�һ�ν��յ��ɶ��¼�ʱ��ʱ��
    */
    time_t                            start_sec;

    // ��start_sec���ʹ�ã���ʾ�����start_sec��ĺ���ƫ����
    ngx_msec_t                        start_msec;

    ngx_uint_t                        method;//���󷽷�
    ngx_uint_t                        http_version; //http�İ汾

    ngx_str_t                         request_line;//�洢�������������ַ���
    ngx_str_t                         uri;  //�����·�� eg '/query.php'
    ngx_str_t                         args; //����Ĳ��� eg 'name=john'
    ngx_str_t                         exten; //�������չ��
    ngx_str_t                         unparsed_uri;//δ������ԭʼURI

    ngx_str_t                         method_name;//���󷽷���
    ngx_str_t                         http_protocol;//HTTPЭ��汾�ַ���

    /*
    ��ʾ��Ҫ���͸��ͻ��˵�HTTP��Ӧ��out�б�������headers_out�����л���ı�ʾHTTPͷ����TCP�����ڵ���ngx_http_output_filter������
    out�л��ᱣ������͵�HTTP���壬����ʵ���첽���͵�HTTP��Ӧ�Ĺؼ�
    */
    ngx_chain_t                      *out; //�����chain

    /*
    ��ǰ����ȿ������û�����������Ҳ�������������������󣬶�main���ʶһϵ����ص������������ԭʼ����
    ����һ�����ͨ��main�͵�ǰ����ĵ�ַ�Ƿ�������жϵ�ǰ�����Ƿ�Ϊ�û�������ԭʼ����
    */
    ngx_http_request_t               *main;

    // ��ǰ����ĸ�����ע�⣬������δ����ԭʼ����
    ngx_http_request_t               *parent;

    // ��subrequest��������صĹ��ܡ�
    /*���ڽ�������������ӳ������������л������󣬶���ʹ��һ����첽���÷�ʽ������*/
    ngx_http_postponed_request_t     *postponed;
    /*
    �����������֮�󣬻���post_subrequest�������������ھͻ�����������write_handler����
    ������������������URIȥƥ��nginx.conf�к��ʵ�location,ѡ����ʵ�ģ���������Ĵ���
    �����������ʱ���ͻ�����������Handler��
    */
    ngx_http_post_subrequest_t       *post_subrequest;/*ָ��������������*/

    /*
    ������������ͨ��posted_requests��������������������ģ�ִ��post������ʱ���õ�ngx_http_run_posted_requests
    ��������ͨ�������õ�������ִ��������ġ�
    */
    ngx_http_posted_request_t        *posted_requests;

    ngx_http_virtual_names_t         *virtual_names;

    /*
    ȫ�ֵ�ngx_http_phase_engine_t�ṹ���ж�����һ��ngx_http_phase_handler_t �ص�������ɵ����飬
    ��phase_handler��Ա������������ʹ�ã���ʾ�����´�Ӧ��ִ����phase_handler��Ϊ���ָ���������еĻص�������
    HTTP������������ַ�ʽ�Ѹ���HTTPģ�鼯��������������ġ�
    */
    ngx_int_t                         phase_handler;

    /*
    ��ʾNGX_HTTP_CONTENT_PHASE�׶��ṩ��HTTPģ�鴦�������һ�ַ�ʽ��content_handlerָ��HTTPģ��ʵ�ֵ�����������
    */
    ngx_http_handler_pt               content_handler;

    /*
    ��NGX_HTTP_ACCESS_PHASE�׶���Ҫ�ж������Ƿ���з���Ȩ��ʱ��ͨ��access_code������HTTPģ���handler�ص������ķ���ֵ��
    ���access_codeΪ0�����ʾ����߱�����Ȩ�ޣ���֮��˵�����󲻾߱�����Ȩ��
    */
    ngx_uint_t                        access_code;

    ngx_http_variable_value_t        *variables;

#if (NGX_PCRE)
    ngx_uint_t                        ncaptures;
    int                              *captures;
    u_char                           *captures_data;
#endif

    size_t                            limit_rate;

    /* used to learn the Apache compatible response length without a header */
    size_t                            header_size;

    // HTTP�����ȫ�����ȣ�����HTTP����
    off_t                             request_length;

    ngx_uint_t                        err_status;

    ngx_http_connection_t            *http_connection;

    ngx_http_log_handler_pt           log_handler;

    // ����������У��������ĳЩ��Դ������Ҫ���������ʱ�ͷţ���ô����Ҫ�ڰѶ�����ͷ���Դ������ӵ�cleanup��Ա�С�
    ngx_http_cleanup_t               *cleanup;

    unsigned                          subrequests:8;

    /*
    ��ʾ��ǰ��������ô��������磬��ʹ��subrequest����ʱ����������������ϵ���������Ŀ�᷵�ص�count�ϣ�ÿ����һ��������
    count����Ҫ��1�������κ�һ���������������µ�������ʱ����Ӧ��ԭʼ����mainָ��ָ������󣩵�countֵ��Ҫ��1.���磬
    �����ǽ���HTTP�����ʱ��������Ҳ��һ���첽���ã�����count��Ҳ��Ҫ��1�������ڽ�������ʱ���Ͳ�����count���ü���δ����ʱ��������
    */
    unsigned                          count:8;

    // ��־λ��Ŀǰ����aioʹ��
    unsigned                          blocked:8;

    // ��־λ��Ϊ1��ʾ��ǰ��������ʹ���첽�ļ�IO
    unsigned                          aio:1;

    unsigned                          http_state:4;

    /* URI with "/." and on Win32 with "//" */
    unsigned                          complex_uri:1;

    /* URI with "%" */
    unsigned                          quoted_uri:1;

    /* URI with "+" */
    unsigned                          plus_in_uri:1;

    /* URI with " " */
    unsigned                          space_in_uri:1;

    unsigned                          invalid_header:1;

    unsigned                          add_uri_to_alias:1;
    unsigned                          valid_location:1;
    unsigned                          valid_unparsed_uri:1;

    // ��־λ��Ϊ1��ʾURL������rewrite��д
    unsigned                          uri_changed:1;

    /*
    ��ʾʹ��rewrite��дURL�Ĵ�������ΪĿǰ�����Ը���10�Σ�����uri_changes��ʼ��Ϊ11����ÿ��дURLһ�ξͰ�uri_changes��1��
    һ��uri_changes����0�������û�����ʧ��
    */
    unsigned                          uri_changes:4;

    unsigned                          request_body_in_single_buf:1;
    unsigned                          request_body_in_file_only:1;
    unsigned                          request_body_in_persistent_file:1;
    unsigned                          request_body_in_clean_file:1;
    unsigned                          request_body_file_group_access:1;
    unsigned                          request_body_file_log_level:3;

    unsigned                          subrequest_in_memory:1;
    unsigned                          waited:1;

#if (NGX_HTTP_CACHE)
    unsigned                          cached:1;
#endif

#if (NGX_HTTP_GZIP)
    unsigned                          gzip_tested:1;
    unsigned                          gzip_ok:1;
    unsigned                          gzip_vary:1;
#endif

    unsigned                          proxy:1;
    unsigned                          bypass_cache:1;
    unsigned                          no_cache:1;

    /*
     * instead of using the request context data in
     * ngx_http_limit_zone_module and ngx_http_limit_req_module
     * we use the single bits in the request structure
     */
    unsigned                          limit_zone_set:1;
    unsigned                          limit_req_set:1;

#if 0
    unsigned                          cacheable:1;
#endif

    unsigned                          pipeline:1;
    unsigned                          plain_http:1;
    unsigned                          chunked:1;
    unsigned                          header_only:1;

    // ��־λ��Ϊ1��ʾ��ǰ������keepalive����
    unsigned                          keepalive:1;

    // �ӳٹرձ�־λ��Ϊ1��ʾ��Ҫ�ӳٹرա������ڽ�����HTTPͷ��ʱ������ְ�����ڣ��ñ�־λ������1�����������հ������Ϊ0
    unsigned                          lingering_close:1;

    // ��־λ��Ϊ1��ʾ���ڶ���HTTP�����еİ���
    unsigned                          discard_body:1;

    // ��־λ��Ϊ1��ʾ����ĵ�ǰ״̬�������ڲ���ת
    unsigned                          internal:1;
    unsigned                          error_page:1;
    unsigned                          ignore_content_encoding:1;
    unsigned                          filter_finalize:1;
    unsigned                          post_action:1;
    unsigned                          request_complete:1;
    unsigned                          request_output:1;

    // ��־λ��Ϊ1��ʾ���͸��ͻ��˵�HTTP��Ӧͷ���Ѿ����͡��ڵ���ngx_http_send_header���������Ѿ��ɹ���������Ӧͷ���������̣�
    // �ñ�־λ�ͻ���1��������ֹ�����ط���ͷ����
    unsigned                          header_sent:1;
    unsigned                          expect_tested:1;
    unsigned                          root_tested:1;
    unsigned                          done:1;
    unsigned                          logged:1;

    // ��ʾ�������Ƿ��д��������ݵı�־λ
    unsigned                          buffered:4;

    unsigned                          main_filter_need_in_memory:1;
    unsigned                          filter_need_in_memory:1;
    unsigned                          filter_need_temporary:1;
    unsigned                          allow_ranges:1;

#if (NGX_STAT_STUB)
    unsigned                          stat_reading:1;
    unsigned                          stat_writing:1;
#endif

    /* used to parse HTTP headers */

    // ״̬������HTTPʱʹ��stats����ʾ��ǰ�Ľ���״̬��
    ngx_uint_t                        state;

    ngx_uint_t                        header_hash;
    ngx_uint_t                        lowcase_index;
    u_char                            lowcase_header[NGX_HTTP_LC_HEADER_LEN];

    u_char                           *header_name_start;
    u_char                           *header_name_end;
    u_char                           *header_start;
    u_char                           *header_end;

    /*
     * a memory that can be reused after parsing a request line
     * via ngx_http_ephemeral_t
     */

    u_char                           *uri_start;
    u_char                           *uri_end;
    u_char                           *uri_ext;
    u_char                           *args_start;
    u_char                           *request_start;
    u_char                           *request_end;
    u_char                           *method_end;
    u_char                           *schema_start;
    u_char                           *schema_end;
    u_char                           *host_start;
    u_char                           *host_end;
    u_char                           *port_start;
    u_char                           *port_end;

    unsigned                          http_minor:16;
    unsigned                          http_major:16;
};


typedef struct {
    ngx_http_posted_request_t         terminal_posted_request;
#if (NGX_HAVE_AIO_SENDFILE)
    u_char                            aio_preload;
#endif
} ngx_http_ephemeral_t;


extern ngx_http_header_t       ngx_http_headers_in[];
extern ngx_http_header_out_t   ngx_http_headers_out[];


#endif /* _NGX_HTTP_REQUEST_H_INCLUDED_ */
