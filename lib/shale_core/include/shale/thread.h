#ifndef _SHALE_THREAD_H
#define _SHALE_THREAD_H


typedef struct thread_condition {
    message_handle_t *handle;
    uint8_t status_awaited;
} thread_condition_t;

typedef struct shale_thread {
    uint8_t thread_state;
    uint8_t running_on;
    thread_condition_t thread_cond;
} shale_thread_t;

void shale_thread_init();
uint8_t shale_thread_hart_id_current();
shale_thread_t *shale_thread_new();
shale_thread_t *shale_thread_current();
void shale_thread_yield();

// TODO these should probably be considered internal scheduler functions
// and not part of the public thread API
uint8_t shale_thread_activate(shale_thread_t *thread, uint8_t hart);
uint8_t shale_thread_suspend(shale_thread_t *thread);

uint8_t shale_thread_set_state(shale_thread_t *thread, uint8_t state);

#endif
