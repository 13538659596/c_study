LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog  

LOCAL_MODULE := test   


LOCAL_SRC_FILES := test.cpp \
				   commen.cpp

include $(BUILD_SHARED_LIBRARY)
