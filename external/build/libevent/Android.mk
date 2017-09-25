LOCAL_PATH:= $(call my-dir)/../../libevent-2.0.22-stable

#
# Libevent
#
include $(CLEAR_VARS)

LOCAL_MODULE:= libevent2
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
    buffer.c \
    bufferevent.c \
    bufferevent_filter.c \
    bufferevent_pair.c \
    bufferevent_ratelim.c \
    bufferevent_sock.c \
    epoll.c \
    epoll_sub.c \
    evdns.c \
    event.c \
    event_tagging.c \
    evmap.c \
    evthread.c \
    evthread_pthread.c \
    evutil.c \
    evutil_rand.c \
    listener.c \
    log.c \
    poll.c \
    select.c \
    signal.c \
    strlcpy.c 

LOCAL_C_INCLUDES += \
  $(LOCAL_PATH)/../build/libevent \
  $(LOCAL_PATH)/ \
  $(LOCAL_PATH)/include
  
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DANDROID -fvisibility=hidden
#turn off warnings since we cannot fix them
LOCAL_CFLAGS += -w

include $(BUILD_STATIC_LIBRARY)

