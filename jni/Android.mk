LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   air/AppDelegate.cpp \
                   air/HelloWorldScene.cpp \
                   air/CCLayout.h \
                   air/GameOverScene.cpp 
                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-path,d:/develpment/cocos2d-x-2.2/cocos2d-x-2.2) \
$(call import-path,d:/develpment/cocos2d-x-2.2/cocos2d-x-2.2cocos2dx/platform/third_party/android/prebuilt) \
$(call import-module,cocos2dx)\
$(call import-module,CocosDenshion/android)\
$(call import-module,extensions)

