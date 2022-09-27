#ifndef CACHALOT_STACK_H
#define CACHALOT_STACK_H

typedef struct cachalot_message
{
    char *key;
    char *value;
} cachalot_message;

typedef int cachalot_type;

typedef struct cachalot_element
{
    struct cachalot_element *pre;

#define CACHALOT_ELEMENT_TYPE_UNKNOWN 0
#define CACHALOT_ELEMENT_TYPE_ANCHOR  1
#define CACHALOT_ELEMENT_TYPE_CAUSE   2
#define CACHALOT_ELEMENT_TYPE_WRAP    3
#define CACHALOT_ELEMENT_TYPE_TRACE   4
#define CACHALOT_ELEMENT_TYPE_MSG     5
    cachalot_type type;

    const char *filename;
    const char *funcname;
    long lineno;

    union
    {
        int code;
        struct cachalot_message msg;
    } info;
} cachalot_element;

typedef struct cachalot_stack
{
    struct cachalot_element anchor;
    int latest_cause;
    int giveup;
} cachalot_stack;

/**
 * cachalot_stack_get
 *
 * if fail, return NULL.
 */
cachalot_stack *cachalot_stack_get(void);

/**
 */
cachalot_element *cachalot_element_new(cachalot_type type, const char *filenmae, const char *funcname, long lineno);

#endif
