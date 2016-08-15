
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PROCESS_H_INCLUDED_
#define _NGX_PROCESS_H_INCLUDED_


#include <ngx_setproctitle.h>


typedef pid_t       ngx_pid_t;

#define NGX_INVALID_PID  -1

typedef void (*ngx_spawn_proc_pt) (ngx_cycle_t *cycle, void *data);

/*��Ź���������Ϣ�����ݽṹ*/
typedef struct {
    // ����ID
    ngx_pid_t           pid;  
    // ��waitpidϵͳ���û�ȡ���Ľ���״̬
    int                 status;        

    //������socketpairϵͳ���ò����������ڽ��̼�ͨ�ŵ�socket�������һ��socket������Ի���ͨ�ţ�
    //Ŀǰ����master ��������worker�ӽ��̼��ͨ�š�
    ngx_socket_t        channel[2];     

    // �ӽ��̵�ѭ��ִ�з������������̵���ngx_spawn_process �����ӽ���ʱʹ��
    ngx_spawn_proc_pt   proc;          

    //�����ngx_spawn_proc_pt�����еڶ���������Ҫ����һ��ָ�룬���ǿ�ѡ�ġ�����worker�ӽ��̾Ͳ���Ҫ��
    //��cache manage���̾���Ҫngx_cache_manager_ctx�����ĳ�Ա����ʱdataһ����ngx_spawn_proc_pt�����еڶ��������ǵȼ۵ġ�
    void               *data;
    // �������ơ�����ϵͳ����ʾ�Ľ���������name��ͬ
    char               *name;

    // ��־λ��Ϊ1��ʾ�����������ӽ���
    unsigned            respawn:1;
    // ��־λ��Ϊ1��ʾ���������ӽ���
    unsigned            just_spawn:1;
    // ��־λ��Ϊ1��ʾ�ڽ��и����ӽ��̷���
    unsigned            detached:1;
    // ��־λ��Ϊ1��ʾ���������˳�
    unsigned            exiting:1;
    // ��־λ��Ϊ1��ʾ�����Ѿ��˳�
    unsigned            exited:1;
} ngx_process_t;


typedef struct {
    char         *path;
    char         *name;
    char *const  *argv;
    char *const  *envp;
} ngx_exec_ctx_t;


#define NGX_MAX_PROCESSES         1024

#define NGX_PROCESS_NORESPAWN     -1
#define NGX_PROCESS_JUST_SPAWN    -2
#define NGX_PROCESS_RESPAWN       -3
#define NGX_PROCESS_JUST_RESPAWN  -4
#define NGX_PROCESS_DETACHED      -5


#define ngx_getpid   getpid

#ifndef ngx_log_pid
#define ngx_log_pid  ngx_pid
#endif


ngx_pid_t ngx_spawn_process(ngx_cycle_t *cycle,
    ngx_spawn_proc_pt proc, void *data, char *name, ngx_int_t respawn);
ngx_pid_t ngx_execute(ngx_cycle_t *cycle, ngx_exec_ctx_t *ctx);
ngx_int_t ngx_init_signals(ngx_log_t *log);
void ngx_debug_point(void);


#if (NGX_HAVE_SCHED_YIELD)
#define ngx_sched_yield()  sched_yield()
#else
#define ngx_sched_yield()  usleep(1)
#endif


extern int            ngx_argc;
extern char         **ngx_argv;
extern char         **ngx_os_argv;

extern ngx_pid_t      ngx_pid;
extern ngx_socket_t   ngx_channel;
extern ngx_int_t      ngx_process_slot;
extern ngx_int_t      ngx_last_process;
extern ngx_process_t  ngx_processes[NGX_MAX_PROCESSES];


#endif /* _NGX_PROCESS_H_INCLUDED_ */
