#define __SHALE_SOURCE__

#include "shale.h"
#include "shale_internal.h"

static void _thread_release(struct light_object *obj);

struct lobj_type ltype_thread = {
        .release = &_thread_release
};
static shale_thread_t *thread_table[SHALE_MAX_THREADS];
static uint8_t thread_count;
static shale_thread_t *thread_active[SHALE_CPU_HARD_THREAD_COUNT];
static shale_thread_t *thread_waiting[SHALE_MAX_THREADS];
static uint8_t thread_waiting_count;

uint8_t shale_port_rp2040_hart_id_current();

static void _thread_release(struct light_object *obj)
{
    free(to_thread(obj));
}
uint8_t _thread_register(shale_thread_t *thread)
{
    if(!thread_count < SHALE_MAX_THREADS)
        return ERROR_MAX_ENTITIES;
    
    thread_table[thread_count++] = thread;
    return SHALE_SUCCESS;
}
// create default thread, assigned to the hart on which shale_thread_init() is called
void shale_thread_init()
{
    uint8_t active_hart = shale_thread_hart_id_current();
    shale_thread_t *init_thread = shale_thread_new();
    init_thread->thread_state = SHALE_THREAD_ACTIVE;
    init_thread->running_on = active_hart;
    thread_active[active_hart] = init_thread;
}
// TODO decouple this API function from hardware-specific code
uint8_t shale_thread_hart_id_current()
{
    return shale_port_rp2040_hart_id_current();
}

shale_thread_t *shale_thread_new()
{
    shale_thread_t *obj = shale_malloc(sizeof(shale_thread_t));

    obj->thread_state = SHALE_THREAD_READY;
}

shale_thread_t *shale_thread_current()
{
    return thread_active[shale_thread_hart_id_current()];
}

void shale_thread_yield()
{
    shale_thread_current()->thread_state = SHALE_THREAD_READY;
    _service_message_queues();
}

uint8_t shale_thread_activate(shale_thread_t *thread, uint8_t hart);
uint8_t shale_thread_suspend(shale_thread_t *thread)
{
    // TODO assert_thread_active

    // check if thread is running on this hart or a different one
    if(thread->running_on == shale_thread_hart_id_current()) {

    }
}
uint8_t shale_thread_set_state(shale_thread_t *thread, uint8_t state)
{
    if(thread->thread_state == state)
        return SHALE_SUCCESS;
    if(state == SHALE_THREAD_ACTIVE) {
        _list_delete_item((void**)thread_waiting, &thread_waiting_count, thread);
    }
}

// ------------------------------------------------------
// BEGIN PLATFORM-SPECIFIC CODE
#ifdef PICO_RP2040
#include <pico/sync.h>

uint8_t shale_port_rp2040_hart_id_current()
{
    // on RP2040, hart-id is equal to core number
    return get_core_num();
}
#else // by default assume hardware is single-threaded
uint8_t shale_port_rp2040_hart_id_current()
{
    return 0;
}
#endif
