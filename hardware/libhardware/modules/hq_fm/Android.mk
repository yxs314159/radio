# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw

LOCAL_SHARED_LIBRARIES := \
        libc \
        libcutils

LOCAL_C_INCLUDES := \
                bionic \
                hardware/libhardware \

LOCAL_SRC_FILES := \
        tea6851a.c      \
        hq_fm_hal.c

LOCAL_MODULE := hq_fm.default

include $(BUILD_SHARED_LIBRARY)
LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -DTARGET_BOARD_PLATFORM=\"$(TARGET_BOARD_PLATFORM)\"
# endif
                       
