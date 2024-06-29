#include <am.h>
#include <klib.h>
#include <rtthread.h>

static Context *ev_handler(Event e, Context *c)
{
    switch (e.event)
    {
    case EVENT_IRQ_TIMER:
        // putch('t');
        break;
    case EVENT_YIELD:
        // putch('y');
        rt_thread_t thread = rt_thread_self();
        rt_ubase_t *from_to = (rt_ubase_t *)(thread->user_data);
        if (from_to[0] != 0)
            *(Context **)(from_to[0]) = c;
        c = *(Context **)from_to[1];
        break;
    default:
        printf("Unhandled event ID = %d\n", e.event);
        assert(0);
    }
    return c;
}

void __am_cte_init()
{
    cte_init(ev_handler);
}

void rt_hw_context_switch_to(rt_ubase_t to)
{
    // printf("switch to %d", to);
    rt_thread_t thread = rt_thread_self();
    rt_ubase_t tmp = thread->user_data;
    rt_ubase_t from_to[2] = {0, to};
    thread->user_data = (rt_ubase_t)from_to;
    yield();
    thread->user_data = tmp;
}

void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to)
{
    // printf("switch from %d to %d", from, to);
    rt_thread_t thread = rt_thread_self();
    rt_ubase_t tmp = thread->user_data;
    rt_ubase_t from_to[2] = {from, to};
    thread->user_data = (rt_ubase_t)from_to;
    yield();
    thread->user_data = tmp;
}

void rt_hw_context_switch_interrupt(void *context, rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread)
{
    assert(0);
}

#define MEM_ALIGN_MASK (sizeof(uintptr_t) - 1)
#define TENTRY_ADDR_OFFET 10

void ktentry(void *kparameter)
{
    void (*tentry)(void *) = (void (*)(void *)) * ((uintptr_t *)kparameter - TENTRY_ADDR_OFFET - 1);
    void *parameter = (void *)*((uintptr_t *)kparameter - TENTRY_ADDR_OFFET - 2);
    void (*texit)() = (void (*)()) * ((uintptr_t *)kparameter - TENTRY_ADDR_OFFET - 3);
    tentry(parameter);
    texit();
}

rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter, rt_uint8_t *stack_addr, void *texit)
{
    for (; ((uintptr_t)stack_addr & MEM_ALIGN_MASK) != 0; stack_addr++)
        ;
    uintptr_t *kparameter = (uintptr_t *)(stack_addr - sizeof(Context));
    *(kparameter - TENTRY_ADDR_OFFET - 1) = (uintptr_t)tentry;
    *(kparameter - TENTRY_ADDR_OFFET - 2) = (uintptr_t)parameter;
    *(kparameter - TENTRY_ADDR_OFFET - 3) = (uintptr_t)texit;
    return (rt_uint8_t *)kcontext((Area){stack_addr, stack_addr}, ktentry, kparameter);
}
