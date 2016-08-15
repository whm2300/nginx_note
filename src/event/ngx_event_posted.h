
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_EVENT_POSTED_H_INCLUDED_
#define _NGX_EVENT_POSTED_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


#if (NGX_THREADS)
extern ngx_mutex_t  *ngx_posted_events_mutex;
#endif


//将ev添加到queue首部
#define ngx_locked_post_event(ev, queue)                                      \
                                                                              \
    if (ev->prev == NULL) {                                                   \
        ev->next = (ngx_event_t *) *queue;                                    \
        ev->prev = (ngx_event_t **) queue;                                    \
        *queue = ev;                                                          \
                                                                              \
        if (ev->next) {                                                       \
            ev->next->prev = &ev->next;                                       \
        }                                                                     \
                                                                              \
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0, "post event %p", ev);  \
                                                                              \
    } else  {                                                                 \
        ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,                        \
                       "update posted event %p", ev);                         \
    }


//线程安全添加
#define ngx_post_event(ev, queue)                                             \
                                                                              \
    ngx_mutex_lock(ngx_posted_events_mutex);                                  \
    ngx_locked_post_event(ev, queue);                                         \
    ngx_mutex_unlock(ngx_posted_events_mutex);


//将ev从post队列中删除
#define ngx_delete_posted_event(ev)                                           \
                                                                              \
    *(ev->prev) = ev->next;                                                   \
                                                                              \
    if (ev->next) {                                                           \
        ev->next->prev = ev->prev;                                            \
    }                                                                         \
                                                                              \
    ev->prev = NULL;                                                          \
    ngx_log_debug1(NGX_LOG_DEBUG_CORE, ev->log, 0,                            \
                   "delete posted event %p", ev);



//调用ev的handler方法并将其从post队列中删除
void ngx_event_process_posted(ngx_cycle_t *cycle,
    ngx_thread_volatile ngx_event_t **posted);
void ngx_wakeup_worker_thread(ngx_cycle_t *cycle);

#if (NGX_THREADS)
ngx_int_t ngx_event_thread_process_posted(ngx_cycle_t *cycle);
#endif


extern ngx_thread_volatile ngx_event_t  *ngx_posted_accept_events;
extern ngx_thread_volatile ngx_event_t  *ngx_posted_events;


#endif /* _NGX_EVENT_POSTED_H_INCLUDED_ */
