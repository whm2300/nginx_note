
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

//http://blog.csdn.net/benbendy1984/article/details/6007313

#ifndef _NGX_CYCLE_H_INCLUDED_
#define _NGX_CYCLE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


#ifndef NGX_CYCLE_POOL_SIZE
#define NGX_CYCLE_POOL_SIZE     16384
#endif


#define NGX_DEBUG_POINTS_STOP   1
#define NGX_DEBUG_POINTS_ABORT  2


typedef struct ngx_shm_zone_s  ngx_shm_zone_t;

typedef ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, void *data);

struct ngx_shm_zone_s {
    void                     *data;
    ngx_shm_t                 shm;
    ngx_shm_zone_init_pt      init;
    void                     *tag;
};

/*
ע�����cycle,cycle�������ڵ���˼����Ӧ��һ���������̡�Ҳ����˵���������µ�nginx��reload�������滻��nginx���ᴴ��һ���µ�cycle������Ӧ��
*/
struct ngx_cycle_s {
    /*
     ����������ģ��洢������Ľṹ��ָ�룬
     ��������һ�����飬�����СΪngx_max_module��������Nginx��module����һ����
     ÿ�������Ա����һ��ָ�룬ָ����һ���洢��ָ������飬��˻ῴ��void ****

    ����ջ����š��������Nginx-ģ�鿪����ܹ�������һ��302ҳ��ͼ��
    ���⣬���ͼҲ����http://img.my.csdn.net/uploads/201202/9/0_1328799724GTUk.gif
    */
    void                  ****conf_ctx; 
    // �ڴ��
    ngx_pool_t               *pool;     

    /*
    ��־ģ�����ṩ�����ɻ���ngx_log_t��־����Ĺ��ܣ������logʵ�������ڻ�û��ִ��ngx_init_cycle����ǰ��
    Ҳ���ǻ�û�н�������ǰ���������Ϣ��Ҫ�������־���ͻ���ʱʹ��log���������������Ļ��
    ��ngx_init_cycle����ִ�к󣬽������nginx.conf�����ļ��е�������������ȷ����־�ļ�����ʱ���log���¸�ֵ��
    */
    ngx_log_t                *log;      
    /*
    ��nginx.conf�����ļ���ȡ����־�ļ�·���󣬽���ʼ��ʼ��error_log��־�ļ�������log���������������־����Ļ��
    ��ʱ����new_log������ʱ�Ե����log��־������ʼ���ɹ��󣬻���new_log�ĵ�ַ���������logָ��
    */
    ngx_log_t                 new_log; 

    /*
    ����poll��rtsig������ʱ��ģ�飬������Ч�ļ��������Ԥ�Ƚ�����Щngx_connection_t�ṹ�壬
    �Լ����¼����ռ����ַ�����ʱfiles�ͻᱣ������ngx_connection_t��ָ����ɵ����飬
    files_n����ָ������������ļ������ֵ��������files�����Ա
    */
    ngx_connection_t        **files;    

    // �������ӳأ���free_connection_n���ʹ��
    ngx_connection_t         *free_connections;
    // �������ӳ������ӵ�����
    ngx_uint_t                free_connection_n;    

    /* ˫������������Ԫ��������ngx_connection_t�ṹ�壬��ʾ���ظ�ʹ�����Ӷ��� */
    ngx_queue_t               reusable_connections_queue;  

    // ��̬���飬ÿ������Ԫ�ش�����ngx_listening_t��Ա����ʾ�����˿ڼ���صĲ���
    ngx_array_t               listening;        

    /*
    ��̬������������������nginx����Ҫ������Ŀ¼�������Ŀ¼�����ڣ��ͻ���ͼ������������Ŀ¼ʧ�ܾͻᵼ��nginx����ʧ�ܡ�
    */
    ngx_array_t               pathes;           
    /*
    ������������Ԫ��������ngx_open_file_t �ṹ�壬����ʾnginx�Ѿ��򿪵������ļ�����ʵ�ϣ�nginx��ܲ�����open_files����������ļ���
    �����ɶԴ˸���Ȥ��ģ������������ļ�·������nginx��ܻ���ngx_init_cycle �����д���Щ�ļ�
    */
    ngx_list_t                open_files;       

    // ������������Ԫ��������ngx_shm_zone_t�ṹ�壬ÿ��Ԫ�ر�ʾһ�鹲���ڴ�
    ngx_list_t                shared_memory;   

    // ��ǰ�������������Ӷ������������connections��Ա���ʹ��
    ngx_uint_t                connection_n;    
    ngx_uint_t                files_n;     

    // ָ��ǰ�����е��������Ӷ�����connection_n���ʹ��
    ngx_connection_t         *connections;   
    // ָ��ǰ�����е����ж��¼�����connection_nͬʱ��ʾ���ж��¼�������
    ngx_event_t              *read_events;   
    // ָ��ǰ�����е�����д�¼�����connection_nͬʱ��ʾ����д�¼�������
    ngx_event_t              *write_events;  

    /*
    �ɵ�ngx_cycle_t ��������������һ��ngx_cycle_t �����еĳ�Ա������ngx_init_cycle ���������������ڣ�
    ��Ҫ����һ����ʱ��ngx_cycle_t���󱣴�һЩ�������ٵ���ngx_init_cycle ����ʱ�Ϳ��԰Ѿɵ�ngx_cycle_t ���󴫽�ȥ��
    ����ʱold_cycle����ͻᱣ�����ǰ�ڵ�ngx_cycle_t����
    */
    ngx_cycle_t              *old_cycle;    

    // �����ļ�����ڰ�װĿ¼��·������
    ngx_str_t                 conf_file;
    // nginx ���������ļ�ʱ��Ҫ���⴦�����������Я���Ĳ�����һ����-g ѡ��Я���Ĳ���
    ngx_str_t                 conf_param;      
    // nginx�����ļ�����Ŀ¼��·��
    ngx_str_t                 conf_prefix;
    //nginx��װĿ¼��·��
    ngx_str_t                 prefix;
    // ���ڽ��̼�ͬ�����ļ�������
    ngx_str_t                 lock_file;
    // ʹ��gethostnameϵͳ���õõ���������
    ngx_str_t                 hostname;   
};
//[p] ����nginx��������Ļ�������
typedef struct {
     ngx_flag_t               daemon;  //[p] �ػ����̱�־            
     ngx_flag_t               master;  //[p] master���̱�־

     ngx_msec_t               timer_resolution;

     ngx_int_t                worker_processes;    //[p] worker���̵�����
     ngx_int_t                debug_points;

     ngx_int_t                rlimit_nofile;
     ngx_int_t                rlimit_sigpending;
     off_t                    rlimit_core;

     int                      priority;

     ngx_uint_t               cpu_affinity_n;
     u_long                  *cpu_affinity;

     char                    *username;             
     ngx_uid_t                user;                 /* user ID */  
     ngx_gid_t                group;                /* group ID*/ 

     ngx_str_t                working_directory;
     ngx_str_t                lock_file;

     ngx_str_t                pid;
     ngx_str_t                oldpid;				

     ngx_array_t              env;
     char                   **environment;

#if (NGX_THREADS)
     ngx_int_t                worker_threads;
     size_t                   thread_stack_size;
#endif

} ngx_core_conf_t;


typedef struct {
     ngx_pool_t              *pool;   /* pcre's malloc() pool */
} ngx_core_tls_t;


#define ngx_is_init_cycle(cycle)  (cycle->conf_ctx == NULL)


ngx_cycle_t *ngx_init_cycle(ngx_cycle_t *old_cycle);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log);
void ngx_delete_pidfile(ngx_cycle_t *cycle);
ngx_int_t ngx_signal_process(ngx_cycle_t *cycle, char *sig);
void ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user);
char **ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last);
ngx_pid_t ngx_exec_new_binary(ngx_cycle_t *cycle, char *const *argv);
u_long ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name,
    size_t size, void *tag);


extern volatile ngx_cycle_t  *ngx_cycle;
extern ngx_array_t            ngx_old_cycles;
extern ngx_module_t           ngx_core_module;
extern ngx_uint_t             ngx_test_config;
extern ngx_uint_t             ngx_quiet_mode;
#if (NGX_THREADS)
extern ngx_tls_key_t          ngx_core_tls_key;
#endif


#endif /* _NGX_CYCLE_H_INCLUDED_ */
