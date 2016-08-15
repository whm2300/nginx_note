
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_EVENT_H_INCLUDED_
#define _NGX_EVENT_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


#define NGX_INVALID_INDEX  0xd0d0d0d0


#if (NGX_HAVE_IOCP)

typedef struct {
    WSAOVERLAPPED    ovlp;
    ngx_event_t     *event;
    int              error;
} ngx_event_ovlp_t;

#endif


typedef struct {
    ngx_uint_t       lock;

    ngx_event_t     *events;
    ngx_event_t     *last;
} ngx_event_mutex_t;


struct ngx_event_s {
    // �¼���صĶ���ͨ��data����ָ��ngx_connection_t���Ӷ��󡣿����ļ��첽I/Oʱ�������ܻ�ָ��ngx_event_aio_t�ṹ��
    void            *data;

    /*
    ��־λ��Ϊ1ʱ��ʾ�¼��ǿ�д�ġ�ͨ������£�����ʾ��Ӧ��TCP����Ŀǰ״̬�ǿ�д�ģ�Ҳ�������Ӵ��ڿ��Է����������״̬��
    */
    unsigned         write:1;

    // ��־λ��Ϊ1ʱ��ʾΪ���¼����Խ����µ����ӡ�ͨ������£���ngx_cycle_t�е�listening��̬�����У�ÿһ����������ngx_listening_t
    // ��Ӧ�Ķ��¼��е�accept��־λ�Ż���1
    unsigned         accept:1;

    /* used to detect the stale events in kqueue, rtsig, and epoll */
    /*
    �����־λ�������ֵ�ǰ�¼��Ƿ���ڣ��������Ǹ��¼�����ģ��ʹ�õģ����¼�����ģ��ɲ��ù��ġ�
    Ϊʲô��Ҫ�����־λ�أ�����ʼ����һ���¼�ʱ������ǰ����¼����ܻ�ر�һЩ���ӣ�����Щ�����п���Ӱ�������¼��л�δ�����ĺ�����¼���
    ��ʱ����ͨ��instance��־λ�����⴦�������Ѿ����ڵ��¼���
    */
    unsigned         instance:1;

    /*
     * the event was passed or would be passed to a kernel;
     * in aio mode - operation was posted.
     */
     /*
    ��־λ��Ϊ1��ʾ��ǰ�¼��ǻ�Ծ�ģ�Ϊ0��ʾ�¼��ǲ���Ծ�ġ�
    ���״̬��Ӧ���¼�����ģ�鴦��ʽ�Ĳ�ͬ�����磬������¼���ɾ���¼��ʹ����¼�ʱ��active��־λ�Ĳ�ͬ�����Ӧ�Ų�ͬ�Ĵ���ʽ��
    ��ʹ���¼�ʱ��һ�㲻��ֱ�Ӹı�active��־λ��
     */
    unsigned         active:1;

    /*
    ��־λ��Ϊ1��ʾ�����¼�������kqueue����rtsig�¼�����ģ������Ч��������epoll�¼�����ģ����û�����塣
    */
    unsigned         disabled:1;

    /* the ready event; in aio mode 0 means that no operation can be posted */
    // ��־λ��Ϊ1��ʾ��ǰ�¼��Ѿ�׼��������Ҳ����˵����������¼�������ģ�鴦������¼�����HTTP����У����������¼���ready��־λ��
    // ��ȷ���Ƿ���Խ���������߷�����Ӧ
    unsigned         ready:1;

    // �ñ�־λ����kqueue,eventport��ģ�������壬������linux�ϵ�epoll�¼�����ģ������������ġ�
    unsigned         oneshot:1;

    /* aio operation is complete */
    // �ñ�־λ�����첽AIO�¼��Ĵ���
    unsigned         complete:1;

    // ��־λ��Ϊ1ʱ��ʾ��ǰ������ַ����Ѿ�����
    unsigned         eof:1;
    // ��־λ��Ϊ1��ʾ�¼��ڴ�������г��ִ���
    unsigned         error:1;

    // ��־λ��Ϊ1��ʾ����¼��Ѿ���ʱ��������ʾ�¼�������ģ������ʱ��������timer_set�����˶�ʱ��
    unsigned         timedout:1;
    // ��־λ��Ϊ1��ʾ����¼������ڶ�ʱ����
    unsigned         timer_set:1;

    // ��־λ��delayedΪ1��ʾ��Ҫ�ӳٴ�������¼��������������ٹ���
    unsigned         delayed:1;

    // ��־λĿǰû��ʹ��
    unsigned         read_discarded:1;

    // Ŀǰû��ʹ��
    unsigned         unexpected_eof:1;

    // ��־λ��Ϊ1��ʾ�ӳٽ���TCP���ӣ�Ҳ����˵������TCP�������ֺ󲢲��������ӣ�����Ҫ�ȵ������ܵ����ݰ���ŻὨ��TCP����
    unsigned         deferred_accept:1;

    /* the pending eof reported by kqueue or in aio chain operation */
    // ��־λ��Ϊ1��ʾ�ȴ��ַ�����������ֻ��kqueue��aio�¼����������й�
    unsigned         pending_eof:1;

#if !(NGX_THREADS)
    // ��־λ�����Ϊ1����ʾ�ڴ���post�¼�ʱ����ǰ�¼��Ѿ�׼������
    unsigned         posted_ready:1;
#endif

#if (NGX_WIN32)
    /* setsockopt(SO_UPDATE_ACCEPT_CONTEXT) was successful */
    unsigned         accept_context_updated:1;
#endif

#if (NGX_HAVE_KQUEUE)
    unsigned         kq_vnode:1;

    /* the pending errno reported by kqueue */
    int              kq_errno;
#endif

    /*
     * kqueue only:
     *   accept:     number of sockets that wait to be accepted
     *   read:       bytes to read when event is ready
     *               or lowat when event is set with NGX_LOWAT_EVENT flag
     *   write:      available space in buffer when event is ready
     *               or lowat when event is set with NGX_LOWAT_EVENT flag
     *
     * iocp: TODO
     *
     * otherwise:
     *   accept:     1 if accept many, 0 otherwise
     */

#if (NGX_HAVE_KQUEUE) || (NGX_HAVE_IOCP)
    int              available;
#else
    // ��־λ����epoll�¼����������±�ʾһ�ξ����ܶཨ��TCP���ӣ�����mulit_accept�������Ӧ
    unsigned         available:1;
#endif

    // ����¼�����ʱ�Ĵ�������ÿ���¼�����ģ�鶼������ʵ����
    ngx_event_handler_pt  handler;


#if (NGX_HAVE_AIO)

#if (NGX_HAVE_IOCP)
    // Windowsϵͳ�µ�һ���¼�����ģ��
    ngx_event_ovlp_t ovlp;
#else
    // Linux aio�����ж���Ľṹ��
    struct aiocb     aiocb;
#endif

#endif

    // ����epoll �¼�������ʽ��ʹ��index���������ﲻ��˵��
    ngx_uint_t       index;

    // �����ڼ�¼error_log��־��ngx_log_t����
    ngx_log_t       *log;

    // ��ʱ���ڵ㣬���ڶ�ʱ���������
    ngx_rbtree_node_t   timer;

    // ��־λ��Ϊ1ʱ��ʾ��ǰ�¼��Ѿ��رգ�epollģ��û��ʹ����
    unsigned         closed:1;

    /* to test on worker exit */
    // ��ʵ������
    unsigned         channel:1;
    // ��ʵ������
    unsigned         resolver:1;

#if (NGX_THREADS)

    unsigned         locked:1;

    unsigned         posted_ready:1;
    unsigned         posted_timedout:1;
    unsigned         posted_eof:1;

#if (NGX_HAVE_KQUEUE)
    /* the pending errno reported by kqueue */
    int              posted_errno;
#endif

#if (NGX_HAVE_KQUEUE) || (NGX_HAVE_IOCP)
    int              posted_available;
#else
    unsigned         posted_available:1;
#endif

    ngx_atomic_t    *lock;
    ngx_atomic_t    *own_lock;

#endif

    /* the links of the posted queue */
    /*
    post�¼����ṹ��һ�����У���ͳһ�������������next��prev��Ϊ����ָ�룬�Դ˹���һ�����׵�˫������
    ����nextָ���һ���¼��ĵ�ַ��prevָ��ǰһ���¼��ĵ�ַ��
    */
    ngx_event_t     *next;
    ngx_event_t    **prev;


#if 0

    /* the threads support */

    /*
     * the event thread context, we store it here
     * if $(CC) does not understand __thread declaration
     * and pthread_getspecific() is too costly
     */

    void            *thr_ctx;

#if (NGX_EVENT_T_PADDING)

    /* event should not cross cache line in SMP */

    uint32_t         padding[NGX_EVENT_T_PADDING];
#endif
#endif
};


#if (NGX_HAVE_FILE_AIO)

struct ngx_event_aio_s {
    void                      *data;
    ngx_event_handler_pt       handler;
    ngx_file_t                *file;

    ngx_fd_t                   fd;

#if (NGX_HAVE_EVENTFD)
    int64_t                    res;
#if (NGX_TEST_BUILD_EPOLL)
    ngx_err_t                  err;
    size_t                     nbytes;
#endif
#else
    ngx_err_t                  err;
    size_t                     nbytes;
#endif

#if (NGX_HAVE_AIO_SENDFILE)
    off_t                      last_offset;
#endif

    ngx_aiocb_t                aiocb;
    ngx_event_t                event;
};

#endif


typedef struct {
    in_addr_t  mask;
    in_addr_t  addr;
} ngx_event_debug_t;


typedef struct {
    /*
    ����¼����������������1������Ȥ���¼���ӵ�����ϵͳ�ṩ���¼��������ƣ���epoll��kqueue�ȣ��У�
    ���������¼�����֮�󣬽������ڵ��������process_envetsʱ��ȡ����¼���
    */
    ngx_int_t  (*add)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    /*
    ɾ���¼�����������һ���Ѿ��������¼����������е��¼�һ���������Ժ�ʹ����¼�����������process_events����ʱҲ�޷��ٻ�ȡ����¼�
    */
    ngx_int_t  (*del)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);

    /*
    ����һ���¼���Ŀǰ�¼���ܲ����������������󲿷��¼�����ģ����ڸ÷�����ʵ�ֶ����������add������ȫһ�µ�
    */
    ngx_int_t  (*enable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    /*
    ����һ���¼���Ŀǰ�¼���ܲ����������������󲿷��¼�����ģ����ڸ÷�����ʵ�ֶ����������del����һ��
    */
    ngx_int_t  (*disable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);

    /*
    ���¼��������������һ���µ����ӣ�����ζ�������ϵĶ�д�¼�����ӵ��¼�������������
    */
    ngx_int_t  (*add_conn)(ngx_connection_t *c);
    // ���¼�����������һ��һ�������Ķ�д�¼�
    ngx_int_t  (*del_conn)(ngx_connection_t *c, ngx_uint_t flags);

    // ���ڶ��̻߳����»ᱻ���ã�Ŀǰ��nginx�ڲ�Ʒ�����»������Զ��̷߳�ʽ���С�
    ngx_int_t  (*process_changes)(ngx_cycle_t *cycle, ngx_uint_t nowait);
    // �������Ĺ���ѭ���У���ͨ������process_events�����������¼���
    // �����������ngx_process_events_and_timers�����е��ã����Ǵ����ַ��¼��ĺ���
    ngx_int_t  (*process_events)(ngx_cycle_t *cycle, ngx_msec_t timer,
                   ngx_uint_t flags);

    // ��ʼ���¼�����ģ��ķ���
    ngx_int_t  (*init)(ngx_cycle_t *cycle, ngx_msec_t timer);
    // �˳��¼�����ģ��ǰ���õķ�����
    void       (*done)(ngx_cycle_t *cycle);
} ngx_event_actions_t;


extern ngx_event_actions_t   ngx_event_actions;


/*
 * The event filter requires to read/write the whole data:
 * select, poll, /dev/poll, kqueue, epoll.
 */
#define NGX_USE_LEVEL_EVENT      0x00000001

/*
 * The event filter is deleted after a notification without an additional
 * syscall: kqueue, epoll.
 */
#define NGX_USE_ONESHOT_EVENT    0x00000002

/*
 * The event filter notifies only the changes and an initial level:
 * kqueue, epoll.
 */
#define NGX_USE_CLEAR_EVENT      0x00000004

/*
 * The event filter has kqueue features: the eof flag, errno,
 * available data, etc.
 */
#define NGX_USE_KQUEUE_EVENT     0x00000008

/*
 * The event filter supports low water mark: kqueue's NOTE_LOWAT.
 * kqueue in FreeBSD 4.1-4.2 has no NOTE_LOWAT so we need a separate flag.
 */
#define NGX_USE_LOWAT_EVENT      0x00000010

/*
 * The event filter requires to do i/o operation until EAGAIN: epoll, rtsig.
 */
#define NGX_USE_GREEDY_EVENT     0x00000020

/*
 * The event filter is epoll.
 */
#define NGX_USE_EPOLL_EVENT      0x00000040

/*
 * No need to add or delete the event filters: rtsig.
 */
#define NGX_USE_RTSIG_EVENT      0x00000080

/*
 * No need to add or delete the event filters: overlapped, aio_read,
 * aioread, io_submit.
 */
#define NGX_USE_AIO_EVENT        0x00000100

/*
 * Need to add socket or handle only once: i/o completion port.
 * It also requires NGX_HAVE_AIO and NGX_USE_AIO_EVENT to be set.
 */
#define NGX_USE_IOCP_EVENT       0x00000200

/*
 * The event filter has no opaque data and requires file descriptors table:
 * poll, /dev/poll, rtsig.
 */
#define NGX_USE_FD_EVENT         0x00000400

/*
 * The event module handles periodic or absolute timer event by itself:
 * kqueue in FreeBSD 4.4, NetBSD 2.0, and MacOSX 10.4, Solaris 10's event ports.
 */
#define NGX_USE_TIMER_EVENT      0x00000800

/*
 * All event filters on file descriptor are deleted after a notification:
 * Solaris 10's event ports.
 */
#define NGX_USE_EVENTPORT_EVENT  0x00001000

/*
 * The event filter support vnode notifications: kqueue.
 */
#define NGX_USE_VNODE_EVENT      0x00002000


/*
 * The event filter is deleted just before the closing file.
 * Has no meaning for select and poll.
 * kqueue, epoll, rtsig, eventport:  allows to avoid explicit delete,
 *                                   because filter automatically is deleted
 *                                   on file close,
 *
 * /dev/poll:                        we need to flush POLLREMOVE event
 *                                   before closing file.
 */
#define NGX_CLOSE_EVENT    1

/*
 * disable temporarily event filter, this may avoid locks
 * in kernel malloc()/free(): kqueue.
 */
#define NGX_DISABLE_EVENT  2

/*
 * event must be passed to kernel right now, do not wait until batch processing.
 */
#define NGX_FLUSH_EVENT    4


/* these flags have a meaning only for kqueue */
#define NGX_LOWAT_EVENT    0
#define NGX_VNODE_EVENT    0


#if (NGX_HAVE_KQUEUE)

#define NGX_READ_EVENT     EVFILT_READ
#define NGX_WRITE_EVENT    EVFILT_WRITE

#undef  NGX_VNODE_EVENT
#define NGX_VNODE_EVENT    EVFILT_VNODE

/*
 * NGX_CLOSE_EVENT, NGX_LOWAT_EVENT, and NGX_FLUSH_EVENT are the module flags
 * and they must not go into a kernel so we need to choose the value
 * that must not interfere with any existent and future kqueue flags.
 * kqueue has such values - EV_FLAG1, EV_EOF, and EV_ERROR:
 * they are reserved and cleared on a kernel entrance.
 */
#undef  NGX_CLOSE_EVENT
#define NGX_CLOSE_EVENT    EV_EOF

#undef  NGX_LOWAT_EVENT
#define NGX_LOWAT_EVENT    EV_FLAG1

#undef  NGX_FLUSH_EVENT
#define NGX_FLUSH_EVENT    EV_ERROR

#define NGX_LEVEL_EVENT    0
#define NGX_ONESHOT_EVENT  EV_ONESHOT
#define NGX_CLEAR_EVENT    EV_CLEAR

#undef  NGX_DISABLE_EVENT
#define NGX_DISABLE_EVENT  EV_DISABLE


#elif (NGX_HAVE_DEVPOLL || NGX_HAVE_EVENTPORT)

#define NGX_READ_EVENT     POLLIN
#define NGX_WRITE_EVENT    POLLOUT

#define NGX_LEVEL_EVENT    0
#define NGX_ONESHOT_EVENT  1


#elif (NGX_HAVE_EPOLL)

#define NGX_READ_EVENT     EPOLLIN
#define NGX_WRITE_EVENT    EPOLLOUT

#define NGX_LEVEL_EVENT    0
#define NGX_CLEAR_EVENT    EPOLLET
#define NGX_ONESHOT_EVENT  0x70000000
#if 0
#define NGX_ONESHOT_EVENT  EPOLLONESHOT
#endif


#elif (NGX_HAVE_POLL)

#define NGX_READ_EVENT     POLLIN
#define NGX_WRITE_EVENT    POLLOUT

#define NGX_LEVEL_EVENT    0
#define NGX_ONESHOT_EVENT  1


#else /* select */

#define NGX_READ_EVENT     0
#define NGX_WRITE_EVENT    1

#define NGX_LEVEL_EVENT    0
#define NGX_ONESHOT_EVENT  1

#endif /* NGX_HAVE_KQUEUE */


#if (NGX_HAVE_IOCP)
#define NGX_IOCP_ACCEPT      0
#define NGX_IOCP_IO          1
#define NGX_IOCP_CONNECT     2
#endif


#ifndef NGX_CLEAR_EVENT
#define NGX_CLEAR_EVENT    0    /* dummy declaration */
#endif


#define ngx_process_changes  ngx_event_actions.process_changes
#define ngx_process_events   ngx_event_actions.process_events /*ngx_event_actionsΪnginx�����IOģ�ͽӿں����ṹ�壬��װ��selet,poll,epoll���¼�����ģ��*/
#define ngx_done_events      ngx_event_actions.done

#define ngx_add_event        ngx_event_actions.add
#define ngx_del_event        ngx_event_actions.del
#define ngx_add_conn         ngx_event_actions.add_conn
#define ngx_del_conn         ngx_event_actions.del_conn

#define ngx_add_timer        ngx_event_add_timer
#define ngx_del_timer        ngx_event_del_timer


extern ngx_os_io_t  ngx_io;

#define ngx_recv             ngx_io.recv
#define ngx_recv_chain       ngx_io.recv_chain
#define ngx_udp_recv         ngx_io.udp_recv
#define ngx_send             ngx_io.send
#define ngx_send_chain       ngx_io.send_chain


#define NGX_EVENT_MODULE      0x544E5645  /* "EVNT" */
#define NGX_EVENT_CONF        0x02000000


typedef struct {
    // ���ӳصĴ�С
    ngx_uint_t    connections;
    // ѡ�õ��¼�ģ���������¼�ģ���е����
    ngx_uint_t    use;

    // ��־λ�����Ϊ1�����ʾ�ڽ��յ�һ���������¼�ʱ��һ���Խ��������ܶ������
    ngx_flag_t    multi_accept;
    //��ʶλ��Ϊ1��ʾ���ø��ؾ�����
    ngx_flag_t    accept_mutex;

    /*
    ���ؾ�������ʹ��Щworker�������ò�����ʱ�ӳٽ��������ӣ�accept_mutex_delay��������ӳ�ʱ��ĳ���
    */
    ngx_msec_t    accept_mutex_delay;

    // ��ѡ���¼�ģ������֣�����use��Ա��ƥ���
    u_char       *name;

#if (NGX_DEBUG)
    /*
    �� --with-debug ����ģʽ�£����Խ����ĳЩ�ͻ��˽���������������Լ������־����debug_connection�������ڱ�����Щ�ͻ��˵ĵ�ַ��Ϣ
    */
    ngx_array_t   debug_connection;
#endif
} ngx_event_conf_t;


typedef struct {
    // �¼�ģ�������
    ngx_str_t              *name;

    // �ڽ���������ǰ������ص��������ڴ����洢����������Ľṹ��
    void                 *(*create_conf)(ngx_cycle_t *cycle);
    // �ڽ�����������ɺ�init_conf�����ᱻ���ã������ۺϴ���ǰ�¼�ģ�����Ȥ��ȫ�������
    char                 *(*init_conf)(ngx_cycle_t *cycle, void *conf);

    // �����¼��������ƣ�ÿ���¼�ģ����Ҫʵ�ֵ�10�����󷽷�
    ngx_event_actions_t     actions;
} ngx_event_module_t;


extern ngx_atomic_t          *ngx_connection_counter;

extern ngx_atomic_t          *ngx_accept_mutex_ptr;
extern ngx_shmtx_t            ngx_accept_mutex;
extern ngx_uint_t             ngx_use_accept_mutex;
extern ngx_uint_t             ngx_accept_events;
extern ngx_uint_t             ngx_accept_mutex_held;
extern ngx_msec_t             ngx_accept_mutex_delay;
extern ngx_int_t              ngx_accept_disabled;


#if (NGX_STAT_STUB)

extern ngx_atomic_t  *ngx_stat_accepted;
extern ngx_atomic_t  *ngx_stat_handled;
extern ngx_atomic_t  *ngx_stat_requests;
extern ngx_atomic_t  *ngx_stat_active;
extern ngx_atomic_t  *ngx_stat_reading;
extern ngx_atomic_t  *ngx_stat_writing;

#endif


#define NGX_UPDATE_TIME         1
#define NGX_POST_EVENTS         2
#define NGX_POST_THREAD_EVENTS  4


extern sig_atomic_t           ngx_event_timer_alarm;
extern ngx_uint_t             ngx_event_flags;
extern ngx_module_t           ngx_events_module;
extern ngx_module_t           ngx_event_core_module;


#define ngx_event_get_conf(conf_ctx, module)                                  \
             (*(ngx_get_conf(conf_ctx, ngx_events_module))) [module.ctx_index];



void ngx_event_accept(ngx_event_t *ev);
ngx_int_t ngx_trylock_accept_mutex(ngx_cycle_t *cycle);
u_char *ngx_accept_log_error(ngx_log_t *log, u_char *buf, size_t len);


void ngx_process_events_and_timers(ngx_cycle_t *cycle);
ngx_int_t ngx_handle_read_event(ngx_event_t *rev, ngx_uint_t flags);
ngx_int_t ngx_handle_write_event(ngx_event_t *wev, size_t lowat);


#if (NGX_WIN32)
void ngx_event_acceptex(ngx_event_t *ev);
ngx_int_t ngx_event_post_acceptex(ngx_listening_t *ls, ngx_uint_t n);
u_char *ngx_acceptex_log_error(ngx_log_t *log, u_char *buf, size_t len);
#endif


ngx_int_t ngx_send_lowat(ngx_connection_t *c, size_t lowat);


/* used in ngx_log_debugX() */
#define ngx_event_ident(p)  ((ngx_connection_t *) (p))->fd


#include <ngx_event_timer.h>
#include <ngx_event_posted.h>
#include <ngx_event_busy_lock.h>

#if (NGX_WIN32)
#include <ngx_iocp_module.h>
#endif


#endif /* _NGX_EVENT_H_INCLUDED_ */
