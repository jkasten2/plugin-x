/****************************************************************************
 Copyright (c) 2012+2013 cocos2d+x.org
 http://www.cocos2d+x.org
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

#import "PushWrapper.h"
#include "PluginUtilsIOS.h"
#include "ProtocolPush.h"

using namespace cocos2d::plugin;

@implementation PushWrapper

+ (void) onDidReceiveRemoteNotification:(id)obj message:(NSString*)message additionalData:(NSString*)additionalData isActive:(BOOL)isActive {
    PluginProtocol* pPlugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolPush* pPush = dynamic_cast<ProtocolPush*>(pPlugin);
    if (pPush) {
        ProtocolPushDidReceiveRemoteNotification callback = pPush->getCallbackDidReceiveRemoteNotification();
        const char* strAdditionalData = nil;
        if (additionalData)
            strAdditionalData = [additionalData UTF8String];
        callback([message UTF8String], strAdditionalData, isActive);
    }
    else
        PluginUtilsIOS::outputLog("Can't find the C++ object of the Push plugin");
}

+ (void) onReceivedTags:(id)obj tags:(NSString*)tags {
    PluginProtocol* pPlugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolPush* pPush = dynamic_cast<ProtocolPush*>(pPlugin);
    if (pPush) {
        ProtocolPushReceivedTags callback = pPush->getCallbackReceivedTags();
        const char* strTags = nil;
        if (tags)
            strTags = [tags UTF8String];
        callback(strTags);
    }
    else
        PluginUtilsIOS::outputLog("Can't find the C++ object of the Push plugin");
}

+ (void) onReceivedIds:(id)obj userid:(NSString*)userId pushToken:(NSString*)pushToken {
    PluginProtocol* pPlugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolPush* pPush = dynamic_cast<ProtocolPush*>(pPlugin);
    if (pPush) {
        ProtocolPushReceivedIds callback = pPush->getCallbackReceivedIds();
        const char* strPushToken = nil;
        if (pushToken)
            strPushToken = [pushToken UTF8String];
        callback([userId UTF8String], strPushToken);
    }
    else
        PluginUtilsIOS::outputLog("Can't find the C++ object of the Push plugin");
}

@end