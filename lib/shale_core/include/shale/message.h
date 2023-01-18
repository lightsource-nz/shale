#ifndef _SHALE_MESSAGE_H
#define _SHALE_MESSAGE_H

// message handler response codes
#define MX_DONE             0x00
#define MX_DELEGATE         0x01
#define MX_FORWARD          0x02
#define MX_ABORT            0x03
#define MX_ERROR            0xFF

// message delivery status codes
#define MXS_QUEUED          0x00
#define MXS_SENT            0x01
#define MXS_RECEIVED        0x02
#define MXS_RESPONDED       0x03
#define MXS_ERROR           0xFF

#define mx(target, id) mx_p(target, id, NULL)
#define mx_p(target, id, param) \
    shale_message_send((message_t){target, id, param})

typedef struct device device_t;

typedef struct message {
    device_t *target;
    uint8_t msg_id;
    void *param[];
} message_t;

typedef struct message_handle {
    uint32_t handle_id;
    uint8_t status;
    device_t *dest;
    message_t msg;
    void *reply;
} message_handle_t;

message_handle_t *message_handle_new(message_t message);
message_handle_t *shale_message_send(message_t message);
void shale_message_await(message_handle_t *handle, uint8_t status);

#endif