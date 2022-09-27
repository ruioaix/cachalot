#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t cachalot_key;
static bool cachalot_key_set = false;

static void destructor(void *buf)
{
    free(buf);
}

static void createKey(void)
{
    if (!pthread_key_create(&cachalot_key, destructor))
        cachalot_key_set = true;
}

static void cachalot_element_init(cachalot_element *elm, cachalot_type type, const char *filename, const char *funcname, long lineno)
{
    elm->pre = NULL;
    elm->type = type;
    elm->filename = filename;
    elm->funcname = funcname;
    elm->lineno = lineno;
}

cachalot_element *cachalot_element_new(cachalot_type type, const char *filename, const char *funcname, long lineno)
{
    cachalot_element *elm = malloc(sizeof(cachalot_element));
    if (!elm)
        return NULL;
    cachalot_element_init(elm, type, filename, funcname, lineno);
    return elm;
}

cachalot_stack *cachalot_stack_get(void)
{
    if (pthread_once(&once, createKey))
        return NULL;
    if (!cachalot_key_set)
        return NULL;

    void *buf = pthread_getspecific(cachalot_key);
    if (buf)
        return buf;

    buf = malloc(sizeof(cachalot_stack));
    if (!buf)
        return NULL;

    if (pthread_setspecific(cachalot_key, buf))
    {
        free(buf);
        return NULL;
    }

    cachalot_stack *stack = (cachalot_stack *)buf;
    cachalot_element_init(&(stack->anchor), CACHALOT_ELEMENT_TYPE_ANCHOR, NULL, NULL, 0);
    stack->latest_cause = 0;
    stack->giveup = 0;
    return stack;
}
