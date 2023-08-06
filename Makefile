# See https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
# for -finstrument-functions
CFLAGS = -Wall -Wextra -Os -finstrument-functions
# -rdynamic is needed to get symbol names in backtraces
#  See https://www.gnu.org/software/libc/manual/html_node/Backtraces.html
LDFLAGS = -rdynamic

hello:
