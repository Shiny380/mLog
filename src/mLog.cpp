#ifndef MLOG_DISABLE_ALL

    #include "mLog.h"
    #include <stdarg.h>
    #include <string.h>
    #include <cstdio>
    #include <vector>

    #define LEVEL_STRING(x) MLOG_HEADER_OPEN x MLOG_HEADER_CLOSE

    #ifndef MLOG_WHITELIST
        #define MLOG_WHITELIST \
            {}
    #endif
    #ifndef MLOG_BLACKLIST
        #define MLOG_BLACKLIST \
            {}
    #endif

const char* const level_names[] = {LEVEL_STRING("TRC"), LEVEL_STRING("DBG"),
                                   LEVEL_STRING("INF"), LEVEL_STRING("WRN"),
                                   LEVEL_STRING("ERR")};

fncptr_printf  _printf = &printf;
fncptr_vprintf _vprintf = &vprintf;
unsigned int   current_log_level_min = 0xFF << MLOG_LEVEL;
unsigned int   current_log_level_mask = 0xFF;
int            current_indent = 0;

std::vector<const char*> whitelist MLOG_WHITELIST;
std::vector<const char*> blacklist MLOG_BLACKLIST;

void mlog_set_printf(fncptr_printf fnc, fncptr_vprintf vfnc) {
    _printf = fnc;
    _vprintf = vfnc;
}

void mlog_set_level(mlog_Levels_e level) {
    if (level > MLOG_LEVEL_ERROR) {
        level = MLOG_LEVEL_ERROR;
    }
    current_log_level_min = 0xFF << (unsigned int)level;
}

void mlog_clear_mask_level() {
    current_log_level_mask = 0xFF;
}

void mlog_mask_level(mlog_Levels_e level) {
    if (level > MLOG_LEVEL_ERROR) {
        level = MLOG_LEVEL_ERROR;
    }
    if (current_log_level_mask == 0xFF) {
        current_log_level_mask = 0;
    }
    current_log_level_mask |= 1 << (unsigned int)level;
}

void mlog_unmask_level(mlog_Levels_e level) {
    if (level > MLOG_LEVEL_ERROR) {
        level = MLOG_LEVEL_ERROR;
    }
    current_log_level_mask |= 0xFF ^ (1 << (unsigned int)level);
    if (current_log_level_mask == 0) {
        mlog_clear_mask_level();
    }
}

void mlog_add_indent() {
    current_indent++;
}

void mlog_remove_indent() {
    current_indent--;
}

int _group_in_list(std::vector<const char*>& vec, mlog_group_t group) {
    for (unsigned int i = 0; i < vec.size(); i++) {
        if (strcmp((const char*)vec[i], (const char*)group) == 0) {
            return (int)i;
        }
    }
    return -1;
}

void mlog_erase_group(std::vector<const char*>& vec, mlog_group_t group) {
    int i = _group_in_list(vec, group);
    if (i >= 0) {
        vec.erase(vec.begin() + i);
    }
}

void mlog_whitelist_group(mlog_group_t group) {
    whitelist.push_back(group);
}

void mlog_unwhitelist_group(mlog_group_t group) {
    mlog_erase_group(whitelist, group);
}

void mlog_clear_whitelist() {
    whitelist.clear();
}

void mlog_blacklist_group(mlog_group_t group) {
    blacklist.push_back(group);
}

void mlog_unblacklist_group(mlog_group_t group) {
    mlog_erase_group(blacklist, group);
}

void mlog_clear_blacklist() {
    blacklist.clear();
}

void _log_header_group(mlog_Levels_e level, mlog_group_t group) {
    _printf(level_names[level]);
    _printf(MLOG_GET_GROUP("%-4s") " ", group);
}

void _log_indent(int indents) {
    for (int i = 0; i < indents; i++) {
        _printf(MLOG_INDENT);
    }
}

void _log_start(mlog_Levels_e level, mlog_group_t group) {
    _log_header_group(level, group);
    _log_indent(current_indent);
}

void _log_end() {
    _printf("\r\n");
}

bool _can_log(mlog_Levels_e level, mlog_group_t group) {
    unsigned int m = 1 << (unsigned int)level;
    if ((m & current_log_level_min) == 0 || (m & current_log_level_mask) == 0) {
        return false;
    }
    if (whitelist.size() && _group_in_list(whitelist, group) < 0) {
        return false;
    }
    if (blacklist.size() && _group_in_list(blacklist, group) >= 0) {
        return false;
    }
    return true;
}

void mlog(mlog_Levels_e level, mlog_group_t group, const char* fmt, ...) {
    if (!_can_log(level, group)) {
        return;
    }
    _log_start(level, group);
    va_list args;
    va_start(args, fmt);
    _vprintf(fmt, args);
    va_end(args);
    _log_end();
}

void mlog_array(mlog_Levels_e level, mlog_group_t group, const char* header,
                const char* fmt, const uint8_t* buf, unsigned int length) {
    if (!_can_log(level, group)) {
        return;
    }
    _log_start(level, group);
    _printf(header);
    for (int i = 0; i < length; i++) {
        _printf(fmt, buf[i]);
    }
    _log_end();
}

#endif