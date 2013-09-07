AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src

if WITH_CHECK
SUBDIRS += test
endif

if WITH_BENCH
SUBDIRS += benchmark
endif
