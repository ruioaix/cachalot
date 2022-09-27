#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cachalot.h"
#include "stack.h"

int cachalot_giveup(void)
{
    cachalot_stack *stack = cachalot_stack_get();
    if (!stack)
        return 0;
    return stack->giveup;
}

int cachalot_element_insert(cachalot_type type, const char *filename, const char *funcname, long lineno,\
        int err, const char *key, const char *value)
{
    if (type == CACHALOT_ELEMENT_TYPE_CAUSE || type == CACHALOT_ELEMENT_TYPE_WRAP)
        if (!err)
            return err;

    if (type == CACHALOT_ELEMENT_TYPE_MSG)
        if (!key || !value || !key[0] || !value[0])
            return -1;

    cachalot_stack *stack = cachalot_stack_get();
    if (!stack)
        return err;

    cachalot_element *elm = cachalot_element_new(type, filename, funcname, lineno);
    if (!elm)
    {
        stack->giveup++;
        return err;
    }

    if (type == CACHALOT_ELEMENT_TYPE_MSG)
    {
        elm->info.msg.key = malloc(strlen(key) + 1);
        if (!elm->info.msg.key)
        {
            stack->giveup++;
            return -1;
        }
        elm->info.msg.value = malloc(strlen(value) + 1);
        if (!elm->info.msg.value)
        {
            stack->giveup++;
            return -1;
        }
        strcpy(elm->info.msg.key, key);
        strcpy(elm->info.msg.value, value);
    }

    elm->pre = stack->anchor.pre;
    stack->anchor.pre = elm;

    if (type == CACHALOT_ELEMENT_TYPE_CAUSE || type == CACHALOT_ELEMENT_TYPE_WRAP)
        elm->info.code = err;

    if (type == CACHALOT_ELEMENT_TYPE_CAUSE)
        stack->latest_cause = err;

    return err;
}

int cachalot_cause(int err, const char *filename, const char *funcname, long lineno)
{
    return cachalot_element_insert(CACHALOT_ELEMENT_TYPE_CAUSE, filename, funcname, lineno, err, NULL, NULL);
}

int cachalot_wrap(int err, const char *filename, const char *funcname, long lineno)
{
    return cachalot_element_insert(CACHALOT_ELEMENT_TYPE_WRAP, filename, funcname, lineno, err, NULL, NULL);
}

void cachalot_trace(const char *filename, const char *funcname, long lineno)
{
    (void)cachalot_element_insert(CACHALOT_ELEMENT_TYPE_TRACE, filename, funcname, lineno, -1, NULL, NULL);
}

void cachalot_msg(const char *key, const char *value, const char *filename, const char *funcname, long lineno)
{
    (void)cachalot_element_insert(CACHALOT_ELEMENT_TYPE_TRACE, filename, funcname, lineno, -1, key, value);
}

void cachalot_empty(void)
{
    cachalot_stack *stack = cachalot_stack_get();
    if (!stack)
        return;
    while (true)
    {
        cachalot_element *elm = stack->anchor.pre;
        if (!elm)
            break;
        if (elm->type == CACHALOT_ELEMENT_TYPE_MSG)
        {
            free(elm->info.msg.key);
            free(elm->info.msg.value);
        }
        stack->anchor.pre = elm->pre;
        free(elm);
    }
}

int cachalot_remove(void)
{
    cachalot_stack *stack = cachalot_stack_get();
    if (!stack)
        return 0;
    while (true)
    {
        cachalot_element *elm = stack->anchor.pre;
        if (!elm)
            break;
        cachalot_type type = elm->type;
        if (type == CACHALOT_ELEMENT_TYPE_MSG)
        {
            free(elm->info.msg.key);
            free(elm->info.msg.value);
        }
        stack->anchor.pre = elm->pre;
        free(elm);
        if (type == CACHALOT_ELEMENT_TYPE_CAUSE)
            break;
    }
    cachalot_element *elm = stack->anchor.pre;
    while (elm)
    {
        if (elm->type == CACHALOT_ELEMENT_TYPE_CAUSE)
            return elm->info.code;
        elm = elm->pre;
    }
    return 0;
}

static int size_of_integer(long num)
{
    int i = 0;
    while (num)
    {
        i++;
        num /= 10;
    }
    return i;
}

char *cachalot_stringlize(void)
{
    cachalot_stack *stack = cachalot_stack_get();
    if (!stack)
        return NULL;
    int step = 1024, size = step, len = 0;
    char *res = malloc(size);
    if (!res)
        return NULL;
    res[0] = 0;
    cachalot_element *elm = stack->anchor.pre;
    while (elm)
    {
        size_t need = strlen(elm->filename) + 1 +\
                      strlen(elm->funcname) + 1 +\
                      size_of_integer(elm->lineno) + 1;
        if (elm->type == CACHALOT_ELEMENT_TYPE_CAUSE || elm->type == CACHALOT_ELEMENT_TYPE_WRAP)
        {
            need += size_of_integer(elm->info.code) + 2;
        }
        else if (elm->type == CACHALOT_ELEMENT_TYPE_MSG)
        {
            need += strlen(elm->info.msg.key) + 3 +\
                    strlen(elm->info.msg.value) + 2;
        }
        else if (elm->type == CACHALOT_ELEMENT_TYPE_TRACE)
        {
            need += 1;
        }

        if (need > size - len)
        {
            size += need + step;
            char *new = realloc(res, size);
            if (!new)
                break;
            res = new;
        }

        if (elm->type == CACHALOT_ELEMENT_TYPE_CAUSE || elm->type == CACHALOT_ELEMENT_TYPE_WRAP)
        {
            sprintf(res + len, "%s:%s:%ld %d\n", elm->filename, elm->funcname, elm->lineno, elm->info.code);
        }
        else if (elm->type == CACHALOT_ELEMENT_TYPE_MSG)
        {
            sprintf(res + len, "%s:%s:%ld %s = %s\n", elm->filename, elm->funcname, elm->lineno, elm->info.msg.key, elm->info.msg.value);
        }
        else if (elm->type == CACHALOT_ELEMENT_TYPE_TRACE)
        {
            sprintf(res + len, "%s:%s:%ld\n", elm->filename, elm->funcname, elm->lineno);
        }
        len += need - 1;
        elm = elm->pre;
    }
    return res;
}
