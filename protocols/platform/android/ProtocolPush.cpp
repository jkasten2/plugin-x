/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "ProtocolPush.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace cocos2d { namespace plugin {

extern "C" {
    // TODO: Split this into 3 differnt ones for notificationReceived, tagsReceived, and idsReceived.
    JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_PushWrapper_nativeOnDidReceiveRemoteNotification(JNIEnv*  env, jobject thiz, jstring className, jstring msg, jstring additionalData, bool isActive) {
        std::string strMsg = PluginJniHelper::jstring2string(msg);
        std::string strAdditionalData = PluginJniHelper::jstring2string(additionalData);
        std::string strClassName = PluginJniHelper::jstring2string(className);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
        PluginUtils::outputLog("ProtocolPush", "nativeOnDidReceiveRemoteNotification(), Get plugin ptr : %p", pPlugin);
        if (pPlugin != NULL) {
            PluginUtils::outputLog("ProtocolPush", "nativeOnDidReceiveRemoteNotification(), Get plugin name : %s", pPlugin->getPluginName());
            ProtocolPush* pUser = dynamic_cast<ProtocolPush*>(pPlugin);

            if (pUser != NULL) {
            	ProtocolPushDidReceiveRemoteNotification callback = pUser->getCallbackDidReceiveRemoteNotification();
            	if (callback)
    				callback(strMsg.c_str(), strAdditionalData.c_str(), isActive);
            }
        }
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_PushWrapper_nativeOnReceivedTags(JNIEnv* env, jobject thiz, jstring className, jstring tags) {
        std::string strTags = PluginJniHelper::jstring2string(tags);
        std::string strClassName = PluginJniHelper::jstring2string(className);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
        PluginUtils::outputLog("ProtocolPush", "nativeOnActionResult(), Get plugin ptr : %p", pPlugin);
        if (pPlugin != NULL) {
            PluginUtils::outputLog("ProtocolPush", "nativeOnActionResult(), Get plugin name : %s", pPlugin->getPluginName());
            ProtocolPush* pUser = dynamic_cast<ProtocolPush*>(pPlugin);

            if (pUser != NULL) {
                ProtocolPushReceivedTags callback = pUser->getCallbackReceivedTags();
                if (callback)
                    callback(strTags.c_str());
            }
        }
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_PushWrapper_nativeOnReceivedIds(JNIEnv* env, jobject thiz, jstring className, jstring userId, jstring pushToken) {
        std::string strUserId = PluginJniHelper::jstring2string(userId);
        std::string strPushToken = PluginJniHelper::jstring2string(pushToken);
        std::string strClassName = PluginJniHelper::jstring2string(className);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
        PluginUtils::outputLog("ProtocolPush", "nativeOnReceivedIds(), Get plugin ptr : %p", pPlugin);
        if (pPlugin != NULL) {
            PluginUtils::outputLog("ProtocolPush", "nativeOnReceivedIds(), Get plugin name : %s", pPlugin->getPluginName());
            ProtocolPush* pUser = dynamic_cast<ProtocolPush*>(pPlugin);

            if (pUser != NULL) {
                ProtocolPushReceivedIds callback = pUser->getCallbackReceivedIds();
                if (callback)
                    callback(strUserId.c_str(), strPushToken.c_str());
            }
        }
    }

}

ProtocolPush::ProtocolPush() {}
ProtocolPush::~ProtocolPush() {}

void ProtocolPush::init(const char* appId, const char* googleProjectNumber, ProtocolPushDidReceiveRemoteNotification cb, bool autoRegister) {
	_notificationReceivedCallback = cb;

    PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
    PluginJniMethodInfo t;
    if (PluginJniHelper::getMethodInfo(t,
        pData->jclassName.c_str(),
        "init",
        "(Ljava/lang/String;Ljava/lang/String;)V")) {

        // invoke java method
        t.env->CallVoidMethod(pData->jobj, t.methodID, t.env->NewStringUTF(googleProjectNumber), t.env->NewStringUTF(appId));
        t.env->DeleteLocalRef(t.classID);
    }
}

void ProtocolPush::sendTag(const char* key, const char* value) {
    PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
    PluginJniMethodInfo t;
    if (PluginJniHelper::getMethodInfo(t,
        pData->jclassName.c_str(),
        "sendTag",
        "(Ljava/lang/String;Ljava/lang/String;)V")) {

        // invoke java method
        t.env->CallVoidMethod(pData->jobj, t.methodID, t.env->NewStringUTF(key), t.env->NewStringUTF(value));
        t.env->DeleteLocalRef(t.classID);
    }
}

void ProtocolPush::deleteTag(const char* key) {
    PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
    PluginJniMethodInfo t;
    if (PluginJniHelper::getMethodInfo(t,
        pData->jclassName.c_str(),
        "sendTag",
        "(Ljava/lang/String;)V")) {

        // invoke java method
        t.env->CallVoidMethod(pData->jobj, t.methodID, t.env->NewStringUTF(key));
        t.env->DeleteLocalRef(t.classID);
    }
}

void ProtocolPush::getTags(ProtocolPushReceivedTags cb) {
    _receivedTagsCallback = cb;

    PluginUtils::callJavaFunctionWithName(this, "getTags");
}

void ProtocolPush::getIds(ProtocolPushReceivedIds cb) {
    _receivedIdsCallback = cb;

    PluginUtils::callJavaFunctionWithName(this, "getIds");
}

}} // namespace cocos2d { namespace plugin {
