#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

// TODO this should be either thread-local or hardware synchronized
static uint32_t next_msgid = 0;

// TODO function not thread-safe, review access synchronization
message_handle_t *message_handle_new(message_t message)
{
    message_handle_t *handle = shale_malloc(sizeof(message_handle_t));
    handle->handle_id = next_msgid++;
    handle->msg = message;
    handle->status = MXS_QUEUED;
    handle->reply = NULL;
}

void message_handle_destroy(message_handle_t *handle)
{
    shale_free(handle);
}

message_handle_t *shale_message_send(message_t message)
{
    message_handle_t *handle = message_handle_new(message);
    queue_add_blocking(&message.target->queue, handle);
    return handle;
}
void shale_message_await(message_handle_t *handle, uint8_t status)
{
    shale_thread_t *thread = shale_thread_current();
    thread->thread_cond = (thread_condition_t) { handle, status };
    thread->thread_state = SHALE_THREAD_BLOCKED;
    // TODO implement yield to message scheduler
}
