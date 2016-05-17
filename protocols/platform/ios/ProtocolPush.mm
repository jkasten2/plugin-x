/****************************************************************************
 Copyright (c) 2012-2013 cocos2d-x.org
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
#include "PluginUtilsIOS.h"
#import "InterfacePush.h"

namespace cocos2d { namespace plugin {
    
    ProtocolPush::ProtocolPush() {}
    ProtocolPush::~ProtocolPush() {}

    void ProtocolPush::init(const char* appId, const char* googleProjectNumber, ProtocolPushDidReceiveRemoteNotification cb, bool autoRegister) {
        _notificationReceivedCallback = cb;
        
        PluginOCData* pData = PluginUtilsIOS::getPluginOCData(this);
        id ocObj = pData->obj;
        if ([ocObj conformsToProtocol:@protocol(InterfacePush)]) {
            NSObject<InterfacePush>* curObj = ocObj;
            [curObj initWithAppId:[NSString stringWithUTF8String:appId] autoRegister:autoRegister];
        }
    }
    
    void ProtocolPush::registerForPushNotifications() {
        PluginOCData* pData = PluginUtilsIOS::getPluginOCData(this);
        id ocObj = pData->obj;
        if ([ocObj conformsToProtocol:@protocol(InterfacePush)]) {
            NSObject<InterfacePush>* curObj = ocObj;
            [curObj registerForPushNotifications];
        }
    }
    
    void ProtocolPush::sendTag(const char *key, const char *value) {
        PluginOCData* pData = PluginUtilsIOS::getPluginOCData(this);
        id ocObj = pData->obj;
        if ([ocObj conformsToProtocol:@protocol(InterfacePush)]) {
            NSObject<InterfacePush>* curObj = ocObj;
            [curObj sendTag:[NSString stringWithUTF8String:key] value:[NSString stringWithUTF8String:value]];
        }
    }
    
    void ProtocolPush::deleteTag(const char *key) {
        PluginOCData* pData = PluginUtilsIOS::getPluginOCData(this);
        id ocObj = pData->obj;
        if ([ocObj conformsToProtocol:@protocol(InterfacePush)]) {
            NSObject<InterfacePush>* curObj = ocObj;
            [curObj deleteTag:[NSString stringWithUTF8String:key]];
        }
    }
    
    void ProtocolPush::getTags(ProtocolPushReceivedTags cb) {
        _receivedTagsCallback = cb;
        PluginOCData* pData = PluginUtilsIOS::getPluginOCData(this);
        
        id ocObj = pData->obj;
        if ([ocObj conformsToProtocol:@protocol(InterfacePush)]) {
            NSObject<InterfacePush>* curObj = ocObj;
            [curObj getTags];
        }
    }
    
    void ProtocolPush::getIds(ProtocolPushReceivedIds cb) {
        _receivedIdsCallback = cb;
        PluginOCData* pData = PluginUtilsIOS::getPluginOCData(this);
        
        id ocObj = pData->obj;
        if ([ocObj conformsToProtocol:@protocol(InterfacePush)]) {
            NSObject<InterfacePush>* curObj = ocObj;
            [curObj getIds];
        }
    }
    
}} // namespace cocos2d { namespace plugin {
