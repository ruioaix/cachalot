#ifndef CACHALOT_H
#define CACHALOT_H

#ifdef SOURCE_ROOT
#define CACHALOT__FILE__ (__FILE__ + (__FILE__[0] == '/' ? sizeof(SOURCE_ROOT) : 0))
#else
#define CACHALOT__FILE__ __FILE__
#endif


/**
 * cachalot_giveup (never fail)
 *
 * return the number of giveup, which is incremented each time a cachalot api call fails.
 */
int cachalot_giveup(void);
#define CACHALOT_GIVEUP() cachalot_giveup()

/**
 * cachalot_cause (could fail)
 *
 * add an error element into error stack, and the error is a origin one, a cause.
 *
 * the function will fail only when malloc fail.
 * if fail, just return the input parameter and increment the giveup counter.
 */
int cachalot_cause(int err, const char *filename, const char *funcname, long lineno);
#define CACHALOT_CAUSE(err) cachalot_cause(err, CACHALOT__FILE__, __func__, __LINE__)

/**
 * cachalot_wrap (could fail)
 *
 * add an error element which is not a cause, but a more
 * comprehensive error caused by the cause.
 * the wrap element always belongs to the closest cause element before it.
 *
 * the function will fail only when malloc fail.
 * if fail, just return the input parameter and increment the giveup counter.
 */
int cachalot_wrap(int err, const char *filename, const char *funcname, long lineno);
#define CACHALOT_WRAP(err) cachalot_wrap(err, CACHALOT__FILE__, __func__, __LINE__)

/**
 * cachalot_trace (could fail)
 *
 * add a trace element into error stack.
 * the function will fail only when the malloc fail.
 * the trace element always belongs to the closest cause element before it.
 *
 * the function will fail only when malloc fail.
 * if fail, no element created and increment the giveup counter.
 */
void cachalot_trace(const char *filename, const char *funcname, long lineno);
#define CACHALOT_TRACE() cachalot_trace(CACHALOT__FILE__, __func__, __LINE__)

/**
 * cachalot_msg (could fail)
 *
 * add a msg element into error stack.
 * the function will fail only when the malloc fail.
 * in failure case, no element created, nothing will happen.
 * the msg element always belongs to the closest cause element before it.
 *
 * the function will fail only when malloc fail.
 * if fail, no element created and increment the giveup counter.
 */
void cachalot_msg(const char *key, const char *value, const char *filename, const char *funcname, long lineno);
#define CACHALOT_MSG(key, value) cachalot_msg(key, value, CACHALOT__FILE__, __func__, __LINE__)

/**
 * cachalot_empty (never fail)
 *
 * empty the whole stack, will never fail.
 */
void cachalot_empty(void);
#define CACHALOT_EMPTY() cachalot_empty()

/**
 * cachalot_remove (never fail)
 *
 * remove all the elements belonging to the latest cause.
 * will never fail.
 * if there is no cause or just 1 cause in the stack, return 0.
 * otherwise, return the previous cause which is the new latest cause after calling this function.
 */
int cachalot_remove(void);
#define CACHALOT_REMOVE() cachalot_remove()

/**
 * cachalot_stringlize (could fail)
 *
 * return a string, which is a formatted error stack message.
 * if no cause, the string will be an empty c-string, "".
 * returned string need to be freed.
 *
 * the function will fail only when malloc fail.
 * if fail, the function return NULL and increment the giveup counter.
 */
char *cachalot_stringlize(void);
#define CACHALOT_STRINGLIZE() cachalot_stringlize()

#endif
