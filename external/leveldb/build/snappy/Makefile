#
# SNAPPY
#
CC=clang
CXX=clang++
AR = ar
ARFLAGS = cru
RANLIB = ranlib

LOCAL_PATH=`pwd`

SNAPPY_DIR = $(LOCAL_PATH)/../../snappy
SNAPPY_SRCDIR = $(SNAPPY_DIR)/

LOCAL_MODULE   = snappy
LOCAL_C_INCLUDES = -I$(SNAPPY_DIR)/ 

LOCAL_CFLAGS = -g -O2 -DHAVE_CONFIG_H
LOCAL_CPPFLAGS =$(LOCAL_CFLAGS)
LOCAL_SRC_FILES = \
  $(SNAPPY_SRCDIR)/snappy.cc \
  $(SNAPPY_SRCDIR)/snappy-sinksource.cc \
  $(SNAPPY_SRCDIR)/snappy-stubs-internal.cc \
  $(SNAPPY_SRCDIR)/snappy-c.cc

#turn off warnings since we cannot fix them
#LOCAL_CFLAGS += -w

all: makelib

source_obj:
	for o in	$(LOCAL_SRC_FILES); do \
		obj=`basename	$$o`; \
		echo "Compiling: `basename	$$o` -->	$${obj%.cc}.o"; \
		$(CXX)	$(LOCAL_CPPFLAGS)	$(LOCAL_C_INCLUDES) -c	$$o; \
		echo -n $${obj%.cc}.o" " >> objs.lst; \
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
	rm -f *.lst
	rm -f lib$(LOCAL_MODULE).a

