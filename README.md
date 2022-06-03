# mLog - C/C++ Micro Logger

Desgined mostly for embedded systems so super light weight with lots of lovely macro magic. Fits into roughly 600 bytes (<1KB)

Realised after a bit that it is basically a rip off of [mbed-trace](https://github.com/PelionIoT/mbed-trace)
so leant into it and ripped off even more. Now just a more stripped down version.

Example:
```
[TRC][ABC1] example trace
[DBG][ABC ] example debug
[INF][GRP1] example info
[WRN][GRP2] example warning
[ERR][MAIN] example error
```

### Features
- Specifically non buffered. Direct to output
- Group/Module names
- All your standard log levels
- Compile time / run time filtering of:
    - groups
    - levles
- Utils:
    - Indenting
    - Arrays

### TODOs
- Thread safety (Mutex locking)
- Stream overload (`<<`) macros
- File output

## Usage
```
#include "mLog.h"

MLOG_SET_FILE_GROUP("ABCD")

...

mlog_info("normal printf stuff %s", "some string);
```
#### Setup
`MLOG_SET_FILE_GROUP()` sets the group name for specific source file (**Max 4 chars**)
- Note: Redefinition errors will occue if this is used in multiple header files at
the same include level. This is minimised by the `__INCLUDE_LEVEL__` macro.
Refer to [Specifying Group](#specifying-group) for how to overcome.

#### Debug Macros
- `mlog_trace()`
- `mlog_debug()`
- `mlog_info()`
- `mlog_warn()`
- `mlog_error()`

#### Disable All
- `#define MLOG_DISABLE_ALL`

#### Output Specifying (default: stdout)
- `mlog_set_printf(printf, vprintf);`

#### Level Filtering
Compile Time:

- `#define MLOG_LOG_LEVEL MLOG_LEVEL_DEF_<level>`

Run Time:

- `mlog_set_level(MLOG_LEVEL_<level>);`

    Or with masks:

- `mlog_mask_level(MLOG_LEVEL_<level>);`
- `mlog_unmask_level(MLOG_LEVEL_<level>);`

    Note: Masks don't apply to anything below the max log level as specified above.

#### Group Filtering
Consists of Whitelist and Blacklist

Compile Time:

- `#define MLOG_WHITELIST {\"GRP1\"}`
- `#define MLOG_BLACKLIST {\"GRP2\"}`

    Note: must escape quotes (`\"`).

Run Time:

- `mlog_whitelist_group("GRP1");`
- `mlog_unwhitelist_group("GRP1");`
- `mlog_clear_whitelist_group("GRP2");`
- `mlog_blacklist_group("GRP2");`
- `mlog_unblacklist_group("GRP2");`
- `mlog_clear_blacklist_group("GRP2");`
- use `MLOG_GET_FILE_GROUP()` to filter on the current file.

#### Utils
Indenting:
- `mlog_add_indent();`
- `mlog_remove_indent();`

Arrays:
- `mlog_<level>_array(header, fmt, buf, len)`

- `mlog_debug_array("some start text", "%02X", array, length);`

#### Specifying Group
You can directly specify the group in the log macros.
Useful for header file logging and one off groups.
- `mlog_<level>_g("ABCD", ...);`

#### Configs
(might have to be done with compile time flags instead of in source)
- `#define MLOG_HEADER_OPEN  "["`
- `#define MLOG_HEADER_CLOSE "["`
- `#define MLOG_INDENT       "  "`

## Contributing

### Tests
#### Setup
```
cmake -B build
cd build
```
#### Run
```
make
ctest --output-on-failure
```