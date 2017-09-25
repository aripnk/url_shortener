#
# URL SHORTENER
#
CC=clang
CXX=clang++
AR = ar
ARFLAGS = cru
RANLIB = ranlib

LOCAL_PATH=`pwd`

LEVELDB_INCLUDES = $(LOCAL_PATH)/../external/leveldb/include/
SNAPPY_INCLUDES = $(LOCAL_PATH)/../external/snappy/

LOCAL_MODULE   = shortensvc
LOCAL_CFLAGS = -O3
LOCAL_CPPFLAGS =$(LOCAL_CFLAGS)
LOCAL_SRC_FILES = \
	$(LOCAL_PATH)/shortensvc.c

LOCAL_C_INCLUDES = \
	-I$(LOCAL_PATH) \
	-I$(LEVELDB_INCLUDES) \
	-I$(SNAPPY_INCLUDES)

LOCAL_STATICLIB_DIR = $(LOCAL_PATH)/../external
LOCAL_STATICCORELIB_DIR = $(LOCAL_PATH)/../libs

LOCAL_STATIC_LIBRARIES= \
	$(LOCAL_STATICCORELIB_DIR)/libshortn.a \
	$(LOCAL_STATICLIB_DIR)/build/leveldb/libleveldb.a \
	$(LOCAL_STATICLIB_DIR)/build/snappy/libsnappy.a


LOCAL_LDLIBS = -lm -lpthread -lrt -static -Wl,--no-as-needed -ldl -levent
LOCAL_CFLAGS =
LOCAL_LDFLAGS =

all: makelibs source_obj linkall

OBJS=`f=*.o;echo $${f}`

linkall:
	$(CXX) -static $(OBJS) -o $(LOCAL_MODULE) $(LOCAL_LDFLAGS) $(LOCAL_STATIC_LIBRARIES) $(LOCAL_LDLIBS) -L.

source_obj:
	for o in	$(LOCAL_SRC_FILES); do \
		obj=`basename $$o`; \
		echo "Compiling: `basename	$$o` -->	$${obj%.c}.o"; \
		$(CC)	$(LOCAL_CFLAGS)	$(LOCAL_C_INCLUDES) -c $$o || exit 1; \
	done

makelibs:
	rm -f staticlib.lst; \
	for o in $(LOCAL_STATIC_LIBRARIES); do \
		i=`dirname $$o`; \
		echo $$i >> staticlib.lst; \
	done; \
	cat staticlib.lst | sort | uniq > staticlib-uniq.lst; \
	staticlibs=`cat staticlib-uniq.lst`; \
	for i in $$staticlibs;do \
		c=$(LOCAL_PATH); \
		cd $$i; \
		make -s || exit 1; \
		cd $$c; \
	done; \
	rm -f staticlib.lst; \
	rm -f staticlib-uniq.lst;

makelibs_clean:
	for o in $(LOCAL_STATIC_LIBRARIES);do \
		c=$(LOCAL_PATH); \
		i=`dirname $$o`; \
		cd $$i; \
		make -s clean; \
		cd $$c; \
	done; \

install:
	cp $(LOCAL_MODULE) /usr/local/bin/.

uninstall:
	rm /usr/local/bin/$(LOCAL_MODULE)

clean: makelibs_clean
	rm -f *~
	rm -f *.o
	rm -f *.a
	rm -f *.lst
	rm -f $(LOCAL_MODULE)
