#ifndef _LIGHT_MESSAGE_H
#define _LIGHT_MESSAGE_H

#include <light_util.h>

enum msg_meta {
        MSG_EVENT = 0u,
        MSG_COMMAND,
        MSG_QUERY,
        MSG_STATE
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
#define Query(name) Message(MSG_QUERY, name)
#define State(name) Message(MSG_STATE, name)

#define DeclareMessage(type, name) extern struct message_type name
#define DeclareEvent(name) DeclareMessage(MSG_EVENT, name)
#define DeclareCommand(name) DeclareMessage(MSG_COMMAND, name)
#define DeclareQuery(name) DeclareMessage(MSG_QUERY, name)
#define DeclareState(name) DeclareMessage(MSG_STATE, name)

#define DefMessage(type,name) struct message_type name = Message(type, name)
#define DefEvent(name) DefMessage(MSG_EVENT, name)
#define DefCommand(name) DefMessage(MSG_COMMAND, name)
#define DefQuery(name) DefMessage(MSG_QUERY, name)
#define DefState(name) DefMessage(MSG_STATE, name)

static inline bool light_message_is_command(struct light_message *msg)
{
        return msg->type->meta == MSG_COMMAND;
}
static inline bool light_message_is_event(struct light_message *msg)
{
        return msg->type->meta == MSG_EVENT;
}
static inline bool light_message_is_query(struct light_message *msg)
{
        return msg->type->meta == MSG_QUERY;
}
static inline bool light_message_is_state(struct light_message *msg)
{
        return msg->type->meta == MSG_STATE;
}


#endif