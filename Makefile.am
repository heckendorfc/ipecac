ACLOCAL_AMFLAGS = -I m4
bin_PROGRAMS = ipecac
ipecac_SOURCES = \
src/arithmetic.o\
src/bitwise.o\
src/convert.o\
src/function.o\
src/internal.o\
src/misc.o\
src/util.o


if WITH_CHECK
ipecac_SOURCES += \
test/check_arithmetic.o\
test/check_bitwise.o\
test/check_convert.o\
test/check_function.o\
test/check_main.o
endif


if WITH_BENCH
ipecac_SOURCES += \
benchmark/bench_arithmetic.o\
benchmark/bench_main.o
endif
