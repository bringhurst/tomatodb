AC_INIT([haven],[0.0.1-alpha.1],[jonb@lanl.gov],[haven],[http://bringhurst.org/haven])

# Keep artifacts in directories as much as possible.
AC_CONFIG_AUX_DIR([auxdir])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Check for protobuf-c command.
AC_CHECK_PROG(PROTOBUF_C_CHECK,protoc-c,yes)
if test x"$PROTOBUF_C_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Could not find the protoc-c command.])
fi

# Use libtool.
LT_INIT

# Checks for libraries.
AC_CHECK_LIB([leveldb], [leveldb_open], [], \
             [AC_MSG_ERROR([leveldb_open was not found in LevelDB])])

AC_CHECK_LIB([protobuf-c], [protobuf_c_rpc_server_new], [], \
             [AC_MSG_ERROR([protobuf_c_rpc_server_new was not found in ProtoBuf-C])])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile              \
                 src/Makefile          \
                 src/common/Makefile   \
                 src/haven/Makefile    \
                 src/havend/Makefile   \
                 doc/Makefile          \
                 doc/man/Makefile      \
                 doc/man/man1/Makefile \
])

AC_OUTPUT
