#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

static uint8_t thread_state = SHALE_THREAD_ACTIVE;
static thread_condition_t thread_cond = { NULL, MXS_QUEUED };

static uint32_t next_msgid = 0;

// TODO function not thread-safe, review access synchronization
msg_handle_t *message_handle_new(message_t message)
{
    msg_handle_t *handle = shale_malloc(sizeof(msg_handle_t));
    handle->handle_id = next_msgid++;
    handle->msg = message;
    handle->status = MXS_QUEUED;
    handle->reply = NULL;
}

void message_handle_destroy(msg_handle_t *handle)
{
    shale_free(handle);
}

msg_handle_t *shale_message_send(message_t message)
{
    msg_handle_t *handle = message_handle_new(message);
    queue_add_blocking(message.target->queue, handle);
    return handle;
}
void shale_message_await(msg_handle_t *handle, uint8_t status)
{
    thread_cond = (thread_condition_t) { handle, status };
    thread_state = SHALE_THREAD_BLOCKED;
    // TODO implement yield to message scheduler
}
