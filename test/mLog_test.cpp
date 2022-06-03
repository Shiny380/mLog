#ifdef BUILD_TESTING

    #include "../src/mLog.h"
    #include <stdarg.h>
    #include <stdio.h>
    #include <string.h>
    #include <catch2/catch_approx.hpp>
    #include <catch2/catch_test_macros.hpp>

char print_buffer[1024];
int  print_buffer_index = 0;

int testPrintf(const char* c, ...) {
    va_list args;
    va_start(args, c);
    int ret = vsprintf(print_buffer + print_buffer_index, c, args);
    va_end(args);
    print_buffer_index += ret;
    va_start(args, c);
    vprintf(c, args);
    va_end(args);
    return ret;
}

int testVPrintf(const char* c, va_list args) {
    int ret = vsprintf(print_buffer + print_buffer_index, c, args);
    print_buffer_index += ret;
    vprintf(c, args);
    return ret;
}

void resetBuffer() {
    print_buffer_index = 0;
    print_buffer[0] = 0;
}

    #define PRINT(a, b, c, d, e) \
        a = b;                   \
        c = d;                   \
        e(a);                    \
        INFO("" << print_buffer);

    #define PRINT_TRACE(a, b, c, d) PRINT(a, b, c, d, mlog_trace)
    #define PRINT_DEBUG(a, b, c, d) PRINT(a, b, c, d, mlog_debug)
    #define PRINT_INFO(a, b, c, d)  PRINT(a, b, c, d, mlog_info)
    #define PRINT_WARN(a, b, c, d)  PRINT(a, b, c, d, mlog_warn)
    #define PRINT_ERROR(a, b, c, d) PRINT(a, b, c, d, mlog_error)

    #define STR_EQL(a, b) strcmp(a, b) == 0

// TODO:
//      masks

TEST_CASE("Logs") {
    MLOG_SET_FILE_GROUP("ABCD");
    mlog_set_level(MLOG_LEVEL_TRACE);
    mlog_set_printf(&testPrintf, &testVPrintf);

    const char* s1;
    const char* s2;

    resetBuffer();

    SECTION("trace") {
        PRINT_TRACE(s1, "trace 1234", s2, "[TRC][ABCD] trace 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("debug") {
        PRINT_DEBUG(s1, "debug 1234", s2, "[DBG][ABCD] debug 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("info") {
        PRINT_INFO(s1, "info 1234", s2, "[INF][ABCD] info 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("warn") {
        PRINT_WARN(s1, "warn 1234", s2, "[WRN][ABCD] warn 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("error") {
        PRINT_ERROR(s1, "error 1234", s2, "[ERR][ABCD] error 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("formatting") {
        s1 = "string: %s, uint8_t: %i, int: %i, float: %.3f";
        s2 = "[INF][ABCD] string: string, uint8_t: 1, int: -1, float: "
             "1.230\r\n";
        mlog_info(s1, "string", (uint8_t)1, -1, (float)1.230);
        INFO("" << print_buffer);
        REQUIRE(STR_EQL(print_buffer, s2));
    }
}

TEST_CASE("Levels") {
    MLOG_SET_FILE_GROUP("ABCD");
    mlog_set_level(MLOG_LEVEL_TRACE);
    mlog_set_printf(&testPrintf, &testVPrintf);

    const char* s1;
    const char* s2;

    resetBuffer();
    SECTION("trace") {
        mlog_set_level(MLOG_LEVEL_TRACE);
        PRINT_TRACE(s1, "1234", s2, "[TRC][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        mlog_set_level(MLOG_LEVEL_DEBUG);
        PRINT_TRACE(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("debug") {
        mlog_set_level(MLOG_LEVEL_DEBUG);
        PRINT_DEBUG(s1, "1234", s2, "[DBG][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        mlog_set_level(MLOG_LEVEL_INFO);
        PRINT_DEBUG(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("info") {
        mlog_set_level(MLOG_LEVEL_INFO);
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        mlog_set_level(MLOG_LEVEL_WARN);
        PRINT_DEBUG(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("warn") {
        mlog_set_level(MLOG_LEVEL_WARN);
        PRINT_WARN(s1, "1234", s2, "[WRN][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        mlog_set_level(MLOG_LEVEL_ERROR);
        PRINT_WARN(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("error") {
        mlog_set_level(MLOG_LEVEL_ERROR);
        PRINT_ERROR(s1, "1234", s2, "[ERR][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        mlog_set_level(mlog_Levels_e(MLOG_LEVEL_ERROR + 1));
        PRINT_ERROR(s1, "1234", s2, "[ERR][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
}

TEST_CASE("Indents") {
    MLOG_SET_FILE_GROUP("ABCD");
    mlog_set_level(MLOG_LEVEL_TRACE);
    mlog_set_printf(&testPrintf, &testVPrintf);
    resetBuffer();
    const char* s1;
    const char* s2;

    PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
    REQUIRE(STR_EQL(print_buffer, s2));
    resetBuffer();
    mlog_add_indent();
    PRINT_INFO(s1, "1234", s2, "[INF][ABCD]   1234\r\n");
    REQUIRE(STR_EQL(print_buffer, s2));
    resetBuffer();
    mlog_add_indent();
    PRINT_INFO(s1, "1234", s2, "[INF][ABCD]     1234\r\n");
    REQUIRE(STR_EQL(print_buffer, s2));
    resetBuffer();
    mlog_remove_indent();
    PRINT_INFO(s1, "1234", s2, "[INF][ABCD]   1234\r\n");
    REQUIRE(STR_EQL(print_buffer, s2));
    resetBuffer();
    mlog_remove_indent();
    PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
    REQUIRE(STR_EQL(print_buffer, s2));
    resetBuffer();
}

TEST_CASE("Custom group") {
    MLOG_SET_FILE_GROUP("ABCD");
    mlog_set_level(MLOG_LEVEL_TRACE);
    mlog_set_printf(&testPrintf, &testVPrintf);
    resetBuffer();
    const char* s1;
    const char* s2;

    PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
    REQUIRE(STR_EQL(print_buffer, s2));
    resetBuffer();
    SECTION("trace") {
        s2 = "[TRC][trce] 1234\r\n";
        mlog_trace_g("trce", s1);
        INFO("" << print_buffer);
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("debug") {
        s2 = "[DBG][dbug] 1234\r\n";
        mlog_debug_g("dbug", s1);
        INFO("" << print_buffer);
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("info") {
        s2 = "[INF][info] 1234\r\n";
        mlog_info_g("info", s1);
        INFO("" << print_buffer);
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("warn") {
        s2 = "[WRN][warn] 1234\r\n";
        mlog_warn_g("warn", s1);
        INFO("" << print_buffer);
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("error") {
        s2 = "[ERR][eror] 1234\r\n";
        mlog_error_g("eror", s1);
        INFO("" << print_buffer);
        REQUIRE(STR_EQL(print_buffer, s2));
    }
}

TEST_CASE("Whitelist") {
    MLOG_SET_FILE_GROUP("ABCD");
    mlog_set_level(MLOG_LEVEL_TRACE);
    mlog_set_printf(&testPrintf, &testVPrintf);
    resetBuffer();
    const char* s1;
    const char* s2;

    SECTION("whitelist this") {
        mlog_whitelist_group(MLOG_GET_FILE_GROUP());
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("remove whitelist") {
        mlog_unwhitelist_group(MLOG_GET_FILE_GROUP());
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("remove non existing group") {
        mlog_unwhitelist_group("YEET");
        mlog_unwhitelist_group(MLOG_GET_FILE_GROUP());
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("whitelist other") {
        mlog_whitelist_group("CUST");
        PRINT_INFO(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        mlog_unwhitelist_group("CUST");
    }
    SECTION("clear whitelist") {
        mlog_whitelist_group("CUST");
        mlog_clear_whitelist();
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
}

TEST_CASE("Blacklist") {
    MLOG_SET_FILE_GROUP("ABCD");
    mlog_set_level(MLOG_LEVEL_TRACE);
    mlog_set_printf(&testPrintf, &testVPrintf);
    resetBuffer();
    const char* s1;
    const char* s2;

    SECTION("blacklist other") {
        mlog_blacklist_group("CUST");
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("blacklist this") {
        mlog_blacklist_group(MLOG_GET_FILE_GROUP());
        PRINT_INFO(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("remove blacklists") {
        mlog_unblacklist_group(MLOG_GET_FILE_GROUP());
        mlog_unblacklist_group("CUST");
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("remove non existing group") {
        mlog_unblacklist_group("YEET");
        mlog_unblacklist_group(MLOG_GET_FILE_GROUP());
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("clear blacklist") {
        mlog_blacklist_group(MLOG_GET_FILE_GROUP());
        PRINT_INFO(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        mlog_clear_blacklist();
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        mlog_unblacklist_group("CUST");
    }
}

TEST_CASE("small group name") {
    MLOG_SET_FILE_GROUP("AB");
    mlog_set_level(MLOG_LEVEL_TRACE);
    mlog_set_printf(&testPrintf, &testVPrintf);
    const char* s1;
    const char* s2;
    resetBuffer();

    PRINT_INFO(s1, "1234", s2, "[INF][AB  ] 1234\r\n");
    REQUIRE(STR_EQL(print_buffer, s2));
    resetBuffer();
    s2 = "[INF][1   ] 1234\r\n";
    mlog_info_g("1", s1);
    INFO("" << print_buffer);
    REQUIRE(STR_EQL(print_buffer, s2));
}

TEST_CASE("mask") {
    MLOG_SET_FILE_GROUP("ABCD");
    mlog_set_level(MLOG_LEVEL_TRACE);
    mlog_set_printf(&testPrintf, &testVPrintf);
    const char* s1;
    const char* s2;
    resetBuffer();

    SECTION("trace") {
        mlog_mask_level(MLOG_LEVEL_TRACE);
        PRINT_TRACE(s1, "1234", s2, "[TRC][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_DEBUG(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_INFO(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_WARN(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_ERROR(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        mlog_unmask_level(MLOG_LEVEL_TRACE);
    }
    SECTION("debug") {
        mlog_mask_level(MLOG_LEVEL_DEBUG);
        PRINT_TRACE(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_DEBUG(s1, "1234", s2, "[DBG][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_INFO(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_WARN(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_ERROR(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        mlog_unmask_level(MLOG_LEVEL_DEBUG);
    }
    SECTION("info") {
        mlog_mask_level(MLOG_LEVEL_INFO);
        PRINT_TRACE(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_DEBUG(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_WARN(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_ERROR(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        mlog_unmask_level(MLOG_LEVEL_INFO);
    }
    SECTION("clear") {
        mlog_mask_level(MLOG_LEVEL_TRACE);
        PRINT_TRACE(s1, "1234", s2, "[TRC][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        mlog_clear_mask_level();
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("clear") {
        mlog_mask_level(MLOG_LEVEL_TRACE);
        PRINT_TRACE(s1, "1234", s2, "[TRC][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_INFO(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        mlog_clear_mask_level();
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("min level above mask") {
        mlog_set_level(MLOG_LEVEL_INFO);
        mlog_mask_level(MLOG_LEVEL_INFO);
        mlog_mask_level(MLOG_LEVEL_TRACE);
        mlog_mask_level(MLOG_LEVEL_DEBUG);
        PRINT_INFO(s1, "1234", s2, "[INF][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_DEBUG(s1, "1234", s2, "");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();

        mlog_set_level(MLOG_LEVEL_TRACE);
        PRINT_TRACE(s1, "1234", s2, "[TRC][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        PRINT_DEBUG(s1, "1234", s2, "[DBG][ABCD] 1234\r\n");
        REQUIRE(STR_EQL(print_buffer, s2));
        resetBuffer();
        mlog_clear_mask_level();
    }
}

    #define PRINT_ARR(a, b, c, d, e, f, g, h) \
        a = b;                                \
        c = d;                                \
        e(a, f, g, h);                        \
        INFO("" << print_buffer);

    #define PRINT_ARR_TRACE(a, b, c, d, e, f, g) \
        PRINT_ARR(a, b, c, d, mlog_trace_array, e, f, g)
    #define PRINT_ARR_DEBUG(a, b, c, d, e, f, g) \
        PRINT_ARR(a, b, c, d, mlog_debug_array, e, f, g)
    #define PRINT_ARR_INFO(a, b, c, d, e, f, g) \
        PRINT_ARR(a, b, c, d, mlog_info_array, e, f, g)
    #define PRINT_ARR_WARN(a, b, c, d, e, f, g) \
        PRINT_ARR(a, b, c, d, mlog_warn_array, e, f, g)
    #define PRINT_ARR_ERROR(a, b, c, d, e, f, g) \
        PRINT_ARR(a, b, c, d, mlog_error_array, e, f, g)

TEST_CASE("arrays") {
    MLOG_SET_FILE_GROUP("ABCD");
    mlog_set_level(MLOG_LEVEL_TRACE);
    mlog_set_printf(&testPrintf, &testVPrintf);
    resetBuffer();
    const char* s1;
    const char* s2;

    uint8_t   buffer[] = {0, 1, 2, 3, 4, 5};
    const int len = sizeof(buffer);
    SECTION("trace") {
        PRINT_ARR_TRACE(s1, "header: ", s2,
                        "[TRC][ABCD] header: 000102030405\r\n", "%02X", buffer,
                        len)
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("debug") {
        PRINT_ARR_DEBUG(s1, "header: ", s2,
                        "[DBG][ABCD] header: 000102030405\r\n", "%02X", buffer,
                        len)
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("info") {
        PRINT_ARR_INFO(s1, "header: ", s2,
                       "[INF][ABCD] header: 000102030405\r\n", "%02X", buffer,
                       len)
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("warn") {
        PRINT_ARR_WARN(s1, "header: ", s2,
                       "[WRN][ABCD] header: 000102030405\r\n", "%02X", buffer,
                       len)
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("error") {
        PRINT_ARR_ERROR(s1, "header: ", s2,
                        "[ERR][ABCD] header: 000102030405\r\n", "%02X", buffer,
                        len)
        REQUIRE(STR_EQL(print_buffer, s2));
    }
    SECTION("array level") {
        mlog_set_level(MLOG_LEVEL_DEBUG);
        PRINT_ARR_TRACE(s1, "header: ", s2, "", "%02X", buffer, len)
        REQUIRE(STR_EQL(print_buffer, s2));
    }
}

#endif
