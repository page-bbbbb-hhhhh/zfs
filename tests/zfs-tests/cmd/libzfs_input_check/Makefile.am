include $(top_srcdir)/config/Rules.am

pkgexecdir = $(datadir)/@PACKAGE@/zfs-tests/bin

pkgexec_PROGRAMS = libzfs_input_check

DEFAULT_INCLUDES = \
	-I$(top_srcdir)/../include \
	-I$(top_srcdir)/../lib/libspl/include

libzfs_input_check_SOURCES = libzfs_input_check.c
libzfs_input_check_LDADD = \
	$(top_builddir)/../lib/libnvpair/libnvpair.la \
	$(top_builddir)/../lib/libzfs_core/libzfs_core.la
