#
# LEVELDB
#
CC=clang
CXX=clang++
AR = ar
ARFLAGS = cru
RANLIB = ranlib

LOCAL_PATH=`pwd`

LEVELDB_DIR = $(LOCAL_PATH)/../../leveldb
LEVELDB_SRCDIR = $(LEVELDB_DIR)/

SNAPPY_INCLUDES = $(LOCAL_PATH)/../../snappy/

LOCAL_MODULE   = leveldb
LOCAL_C_INCLUDES = -I$(LEVELDB_DIR)/ \
	-I$(LEVELDB_DIR)/include \
	-I$(SNAPPY_INCLUDES)

LOCAL_CPPFLAGS =-fno-builtin-memcmp -pthread -DOS_LINUX -DLEVELDB_PLATFORM_POSIX -DSNAPPY
LOCAL_SRC_FILES = \
  $(LEVELDB_SRCDIR)/db/builder.cc \
  $(LEVELDB_SRCDIR)/db/c.cc \
  $(LEVELDB_SRCDIR)/db/dbformat.cc \
  $(LEVELDB_SRCDIR)/db/db_impl.cc \
  $(LEVELDB_SRCDIR)/db/db_iter.cc \
  $(LEVELDB_SRCDIR)/db/dumpfile.cc \
  $(LEVELDB_SRCDIR)/db/filename.cc \
  $(LEVELDB_SRCDIR)/db/log_reader.cc \
  $(LEVELDB_SRCDIR)/db/log_writer.cc \
  $(LEVELDB_SRCDIR)/db/memtable.cc \
  $(LEVELDB_SRCDIR)/db/repair.cc \
  $(LEVELDB_SRCDIR)/db/table_cache.cc \
  $(LEVELDB_SRCDIR)/db/version_edit.cc \
  $(LEVELDB_SRCDIR)/db/version_set.cc \
  $(LEVELDB_SRCDIR)/db/write_batch.cc \
  $(LEVELDB_SRCDIR)/table/block_builder.cc \
  $(LEVELDB_SRCDIR)/table/block.cc \
  $(LEVELDB_SRCDIR)/table/filter_block.cc \
  $(LEVELDB_SRCDIR)/table/format.cc \
  $(LEVELDB_SRCDIR)/table/iterator.cc \
  $(LEVELDB_SRCDIR)/table/merger.cc \
  $(LEVELDB_SRCDIR)/table/table_builder.cc \
  $(LEVELDB_SRCDIR)/table/table.cc \
  $(LEVELDB_SRCDIR)/table/two_level_iterator.cc \
  $(LEVELDB_SRCDIR)/util/arena.cc \
  $(LEVELDB_SRCDIR)/util/bloom.cc \
  $(LEVELDB_SRCDIR)/util/cache.cc \
  $(LEVELDB_SRCDIR)/util/coding.cc \
  $(LEVELDB_SRCDIR)/util/comparator.cc \
  $(LEVELDB_SRCDIR)/util/crc32c.cc \
  $(LEVELDB_SRCDIR)/util/env.cc \
  $(LEVELDB_SRCDIR)/util/env_posix.cc \
  $(LEVELDB_SRCDIR)/util/filter_policy.cc \
  $(LEVELDB_SRCDIR)/util/hash.cc \
  $(LEVELDB_SRCDIR)/util/histogram.cc \
  $(LEVELDB_SRCDIR)/util/logging.cc \
  $(LEVELDB_SRCDIR)/util/options.cc \
  $(LEVELDB_SRCDIR)/util/status.cc  \
  $(LEVELDB_SRCDIR)/port/port_posix.cc

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

