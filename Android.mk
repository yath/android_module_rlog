# !!! build with APP_STL=gnustl_static !!!
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := rlog

LOCAL_SRC_FILES := $(addprefix rlog/,rlog.cpp rloginit.cpp rloglocation.cpp \
				Error.cpp RLogChannel.cpp RLogNode.cpp RLogPublisher.cpp \
				RLogTime.cpp StdioNode.cpp SyslogNode.cpp)

MY_RLOG_COMPONENT       ?= "UNKNOWN"
LOCAL_CFLAGS             = -DRLOG_COMPONENT="\"$(MY_RLOG_COMPONENT)\""
LOCAL_EXPORT_CFLAGS      = $(LOCAL_CFLAGS)

LOCAL_C_INCLUDES        := $(LOCAL_PATH) $(LOCAL_PATH)/android-include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_CPPFLAGS        := -fexceptions
LOCAL_EXPORT_CPPFLAGS := $(LOCAL_CPPFLAGS)

include $(BUILD_STATIC_LIBRARY)
