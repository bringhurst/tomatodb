AC_INIT([TomatoDB],[0.0.1-alpha.1],[jon@bringhurst.org],[TomatoDB],[http://tomato.sx/])

# Keep artifacts in directories as much as possible.
AC_CONFIG_AUX_DIR([auxdir])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AM_PROG_AS
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AM_PATH_PYTHON([2.7])

# Check for the uuidd command.
AC_CHECK_PROG(UUIDD_CHECK,uuidd,yes)
if test x"$UUIDD_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Could not find the uuidd command. Is uuid-runtime installed?])
fi

# Use libtool.
LT_INIT

# Checks for required libraries.
AC_CHECK_LIB([fuse], [fuse_main_compat2], [], \
             [AC_MSG_ERROR([Function fuse_main_compat2 was not found in FUSE.
Is libfuse-dev installed? Try running 'sudo apt-get install libfuse-dev'.])])

AC_CHECK_LIB([leveldb], [leveldb_open], [], \
             [AC_MSG_ERROR([Function leveldb_open was not found in LevelDB.
Is libleveldb-dev installed? Try running 'sudo apt-get install libleveldb-dev'.])])

AC_CHECK_LIB([uuid], [uuid_generate_time_safe], [], \
             [AC_MSG_ERROR([Function uuid_generate_time_safe was not found in libuuid.
Is uuid-dev installed? Try running 'sudo apt-get install uuid-dev'.])])

# Checks for header files.
AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h limits.h netdb.h \
                  netinet/in.h stdlib.h string.h strings.h sys/socket.h \
                  sys/time.h unistd.h malloc.h stddef.h Python.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_TYPES([ptrdiff_t])
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_TYPE_OFF_T
AC_TYPE_UINT64_T 
AC_C_INLINE

# Check for MSG_NOSIGNAL to ignore SIGPIPE.
AC_MSG_CHECKING(for MSG_NOSIGNAL)
AC_TRY_COMPILE([#include <sys/socket.h>],
               [int flag = MSG_NOSIGNAL;],
               [AC_MSG_RESULT(yes)
                 AC_DEFINE(HAVE_MSG_NOSIGNAL, 1,
                           [Define this symbol if you have MSG_NOSIGNAL]) ],
               [AC_MSG_RESULT(no)]
)

# Check for large file support for FUSE.
AC_SYS_LARGEFILE
AC_FUNC_FSEEKO

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([bzero gethostbyname gettimeofday memmove memset mkdir socket \
                strcasecmp strerror strrchr strtoul strdup])

# Generate the config files.
AC_CONFIG_FILES([src/tomato/setup.py], \
                [chmod +x src/tomato/setup.py])

AC_CONFIG_FILES([Makefile                            \
                 src/Makefile                        \
                 src/common/Makefile                 \
                 src/common/halloc/Makefile          \
                 src/common/task/Makefile            \
                 src/common/xdr/Makefile             \
                 src/tomato/Makefile               \
                 src/tomatod/Makefile                 \
                 doc/Makefile                        \
                 doc/man/Makefile                    \
                 doc/man/man1/Makefile               \
])

AC_OUTPUT
