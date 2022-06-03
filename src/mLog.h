#ifndef MLOG_H
#define MLOG_H

#include <stdarg.h>
#include <stdint.h>

// CONFIGS
#ifndef MLOG_HEADER_OPEN
    #define MLOG_HEADER_OPEN "["
#endif
#ifndef MLOG_HEADER_CLOSE
    #define MLOG_HEADER_CLOSE "]"
#endif

#ifndef MLOG_INDENT
    #define MLOG_INDENT "  "
#endif
// END CONFIG

#define MLOG_LEVEL_DEF_TRACE 0
#define MLOG_LEVEL_DEF_DEBUG 1
#define MLOG_LEVEL_DEF_INFO  2
#define MLOG_LEVEL_DEF_WARN  3
#define MLOG_LEVEL_DEF_ERROR 4

#ifndef MLOG_LEVEL
    #define MLOG_LEVEL MLOG_LEVEL_DEF_INFO
#endif

#define MLOG_GROUP_MAX_LEN (4 + 3)
#define MLOG_GROUP_STR(x)  MLOG_HEADER_OPEN x MLOG_HEADER_CLOSE

#define MLOG_GROUP_VAR_NAME mlog_group_name_
#define CONCAT_INNER(a, b)  a##b
#define CONCAT(a, b)        CONCAT_INNER(a, b)

#define MLOG_SET_FILE_GROUP(x) \
    const mlog_group_t CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__) = x;
#define MLOG_GET_FILE_GROUP() CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__)
#define MLOG_GET_GROUP(x)     MLOG_GROUP_STR(x)

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*fncptr_printf)(const char*, ...);
typedef int (*fncptr_vprintf)(const char*, va_list);

typedef const char mlog_group_t[MLOG_GROUP_MAX_LEN + 1];

typedef enum {
    MLOG_LEVEL_TRACE = MLOG_LEVEL_DEF_TRACE,
    MLOG_LEVEL_DEBUG = MLOG_LEVEL_DEF_DEBUG,
    MLOG_LEVEL_INFO = MLOG_LEVEL_DEF_INFO,
    MLOG_LEVEL_WARN = MLOG_LEVEL_DEF_WARN,
    MLOG_LEVEL_ERROR = MLOG_LEVEL_DEF_ERROR,
} mlog_Levels_e;

#if MLOG_LEVEL <= MLOG_LEVEL_DEF_TRACE
    #define mlog_trace(...)                                                    \
        mlog(MLOG_LEVEL_TRACE, CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), \
             __VA_ARGS__)
    #define mlog_trace_g(group, ...) mlog(MLOG_LEVEL_TRACE, group, __VA_ARGS__)
    #define mlog_trace_array(header, fmt, buf, len)                        \
        mlog_array(MLOG_LEVEL_TRACE,                                       \
                   CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), header, \
                   fmt, buf, len)
#else
    #define mlog_trace(...)
    #define mlog_trace_g(...)
    #define mlog_trace_array(...)
#endif

#if MLOG_LEVEL <= MLOG_LEVEL_DEF_DEBUG
    #define mlog_debug(...)                                                    \
        mlog(MLOG_LEVEL_DEBUG, CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), \
             __VA_ARGS__)
    #define mlog_debug_g(group, ...) mlog(MLOG_LEVEL_DEBUG, group, __VA_ARGS__)
    #define mlog_debug_array(header, fmt, buf, len)                        \
        mlog_array(MLOG_LEVEL_DEBUG,                                       \
                   CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), header, \
                   fmt, buf, len)
#else
    #define mlog_debug(...)
    #define mlog_debug_g(...)
    #define mlog_debug_array(...)
#endif

#if MLOG_LEVEL <= MLOG_LEVEL_DEF_INFO
    #define mlog_info(...)                                                    \
        mlog(MLOG_LEVEL_INFO, CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), \
             __VA_ARGS__)
    #define mlog_info_g(group, ...) mlog(MLOG_LEVEL_INFO, group, __VA_ARGS__)
    #define mlog_info_array(header, fmt, buf, len)                         \
        mlog_array(MLOG_LEVEL_INFO,                                        \
                   CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), header, \
                   fmt, buf, len)
#else
    #define mlog_info(...)
    #define mlog_info_g(...)
    #define mlog_info_array(...)
#endif

#if MLOG_LEVEL <= MLOG_LEVEL_DEF_WARN
    #define mlog_warn(...)                                                    \
        mlog(MLOG_LEVEL_WARN, CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), \
             __VA_ARGS__)
    #define mlog_warn_g(group, ...) mlog(MLOG_LEVEL_WARN, group, __VA_ARGS__)
    #define mlog_warn_array(header, fmt, buf, len)                         \
        mlog_array(MLOG_LEVEL_WARN,                                        \
                   CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), header, \
                   fmt, buf, len)
#else
    #define mlog_warn(...)
    #define mlog_warn_g(...)
    #define mlog_warn_array(...)
#endif

#if MLOG_LEVEL <= MLOG_LEVEL_DEF_ERROR
    #define mlog_error(...)                                                    \
        mlog(MLOG_LEVEL_ERROR, CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), \
             __VA_ARGS__)
    #define mlog_error_g(group, ...) mlog(MLOG_LEVEL_ERROR, group, __VA_ARGS__)
    #define mlog_error_array(header, fmt, buf, len)                        \
        mlog_array(MLOG_LEVEL_ERROR,                                       \
                   CONCAT(MLOG_GROUP_VAR_NAME, __INCLUDE_LEVEL__), header, \
                   fmt, buf, len)
#else
    #define mlog_error(...)
    #define mlog_error_g(...)
    #define mlog_error_array(...)
#endif

#ifndef MLOG_DISABLE_ALL
void mlog_set_printf(fncptr_printf fnc, fncptr_vprintf vfnc);
void mlog_set_level(mlog_Levels_e level);
void mlog_clear_mask_level();
void mlog_mask_level(mlog_Levels_e level);
void mlog_unmask_level(mlog_Levels_e level);
void mlog_add_indent();
void mlog_remove_indent();
void mlog_whitelist_group(mlog_group_t group);
void mlog_unwhitelist_group(mlog_group_t group);
void mlog_clear_whitelist();
void mlog_blacklist_group(mlog_group_t group);
void mlog_unblacklist_group(mlog_group_t group);
void mlog_clear_blacklist();

void mlog(mlog_Levels_e level, mlog_group_t group, const char* fmt, ...);
void mlog_array(mlog_Levels_e level, mlog_group_t group, const char* header,
                const char* fmt, const uint8_t* buf, unsigned int length);

#else  // MLOG_DISABLE_ALL

    #define mlog_set_printf(...)
    #define mlog_set_level(...)
    #define mlog_mask_level(...)
    #define mlog_unmask_level(...)
    #define mlog_add_indent(...)
    #define mlog_remove_indent(...)
    #define mlog_whitelist_group(...)
    #define mlog_unwhitelist_group(...)
    #define mlog_clear_whitelist(...)
    #define mlog_blacklist_group(...)
    #define mlog_unblacklist_group(...)
    #define mlog_clear_blacklist(...)
    #define mlog(...)
    #define mlog_array(...)
#endif  // MLOG_DISABLE_ALL

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // MLOG_H
