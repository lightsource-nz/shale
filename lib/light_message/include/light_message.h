#ifndef _LIGHT_MESSAGE_H
#define _LIGHT_MESSAGE_H

#include <light_util.h>

enum msg_meta {
        MSG_EVENT = 0u,
        MSG_COMMAND
};

struct light_message;
struct message_type {
        const uint8_t meta;
        const uint8_t *name;
};

struct light_message {
        uint32_t id;
        struct message_type *type;
        void *params;
};

#define Message(type, name) (struct message_type) { type, #name }
#define Event(name) Message(MSG_EVENT, name)
#define Command(name) Message(MSG_COMMAND, name)

#define DefMessage(type,name) struct message_type name = Message(type, name)
#define DefEvent(name) DefMessage(MSG_EVENT, name)
#define DefCommand(name) DefMessage(MSG_COMMAND, name)

#endif