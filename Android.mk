LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := clib
LOCAL_SRC_FILES := lib.c
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cbump
LOCAL_C_FLAGS += -DANDROID
LOCAL_SRC_FILES := main.c
LOCAL_STATIC_LIBRARIES := clib
include $(BUILD_EXECUTABLE)
