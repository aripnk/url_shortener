#
# LIBEVENT
#
CC=clang
CXX=clang++
AR = ar
ARFLAGS = cru
RANLIB = ranlib

LOCAL_PATH=`pwd`

LIBEVENT_DIR = $(LOCAL_PATH)/../../libevent-2.0.22-stable
LIBEVENT_SRCDIR = $(LIBEVENT_DIR)/

LOCAL_MODULE   = event2
LOCAL_C_INCLUDES = -I$(LIBEVENT_DIR)/ \
  -I$(LIBEVENT_DIR)/include/

LOCAL_CFLAGS = -O3 -DHAVE_CONFIG_H
LOCAL_CPPFLAGS =$(LOCAL_CFLAGS)
LOCAL_SRC_FILES = \
  $(LIBEVENT_SRCDIR)/buffer.c \
  $(LIBEVENT_SRCDIR)/bufferevent.c \\
	$(LIBEVENT_SRCDIR)/bufferevent_filter.c \
  $(LIBEVENT_SRCDIR)/bufferevent_openssl.c \
  $(LIBEVENT_SRCDIR)/bufferevent_pair.c \
  $(LIBEVENT_SRCDIR)/epoll.c \
  $(LIBEVENT_SRCDIR)/epoll_sub.c \
  $(LIBEVENT_SRCDIR)/evdns.c \
  $(LIBEVENT_SRCDIR)/event.c \
  $(LIBEVENT_SRCDIR)/event_tagging.c \
  $(LIBEVENT_SRCDIR)/evmap.c \
  $(LIBEVENT_SRCDIR)/evthread.c \
  $(LIBEVENT_SRCDIR)/evthread_pthread.c \
  $(LIBEVENT_SRCDIR)/evutil.c \
  $(LIBEVENT_SRCDIR)/evutil_rand.c \
  $(LIBEVENT_SRCDIR)/listener.c \
  $(LIBEVENT_SRCDIR)/log.c \
  $(LIBEVENT_SRCDIR)/poll.c \
  $(LIBEVENT_SRCDIR)/select.c \
  $(LIBEVENT_SRCDIR)/signal.c \
	$(LIBEVENT_SRCDIR)/strlcpy.c \
	$(LIBEVENT_SRCDIR)/http.c \
	$(LIBEVENT_SRCDIR)/evrpc.c \
	$(LIBEVENT_SRCDIR)/epoll_sub.c


#turn off warnings since we cannot fix them
LOCAL_CFLAGS += -w

all: makelib

source_obj:
	for o in	$(LOCAL_SRC_FILES); do \
		obj=`basename	$$o`; \
		echo "Compiling: `basename	$$o` -->	$${obj%.c}.o"; \
		$(CC)	$(LOCAL_CFLAGS)	$(LOCAL_C_INCLUDES) -c	$$o; \
		echo -n $${obj%.c}.o" " >> objs.lst; \
	done

OBJS = `cat objs.lst`
makelib: source_obj
	echo "lib$(LOCAL_MODULE).a <-- $(OBJS)"
	$(AR) $(ARFLAGS) lib$(LOCAL_MODULE).a $(OBJS)
	$(RANLIB) lib$(LOCAL_MODULE).a
	rm -f *.o
	rm -f *.lst


clean:
	rm -f *~
	rm -f *.o
	rm -f *.a
	rm -f lib$(LOCAL_MODULE).a
