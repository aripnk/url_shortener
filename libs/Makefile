#
# URL SHORTENER
#
CC=clang
CXX=clang++
AR = ar
ARFLAGS = cru
RANLIB = ranlib

LOCAL_PATH=`pwd`

LIB_STATIC_EXTERNAL_DIR = $(LOCAL_PATH)../external
LEVELDB_INCLUDES = $(LOCAL_PATH)/../external/leveldb/include/
SNAPPY_INCLUDES = $(LOCAL_PATH)/../external/snappy/

LOCAL_MODULE   = shortn
LOCAL_C_INCLUDES = -I$(LOCAL_PATH) \
	-I$(LOCAL_PATH)/logger \
	-I$(LOCAL_PATH)/platform \
	-I$(LIB_STATIC_EXTERNAL_DIR)/libevent-2.0.22-stable \
	-I$(LEVELDB_INCLUDES) \
	-I$(SNAPPY_INCLUDES)

LOCAL_SRC_FILES = \
	db.c \
	../external/cJSON/cJSON.c \
	general.c

LOCAL_CFLAGS = -DUSE_DEBUG
#LOCAL_CFLAGS =

all: makelib

source_obj:
	for o in	$(LOCAL_SRC_FILES); do \
		obj=`basename	$$o`; \
		echo "Compiling: `basename	$$o` -->	$${obj%.c}.o"; \
		$(CC)	$(LOCAL_CFLAGS)	$(LOCAL_C_INCLUDES) -c	$$o || exit 1; \
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
