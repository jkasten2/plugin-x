/**
 * Copyright 2015 OneSignal
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import <UIKit/UIKit.h>
#import <objc/runtime.h>

#import "PushWrapper.h"

#import <OneSignal/OneSignal.h>
#import "OneSignalPush.h"


OneSignalPush* oneSignalPush;
OneSignal* oneSignal;

NSString* launchMessage;
NSDictionary* launchAdditionalData;
BOOL launchIsActive;

BOOL initCalled = false;


NSString* dictionaryToJsonString(NSDictionary* dictionaryToConvert) {
    if (!dictionaryToConvert)
        return nil;
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:dictionaryToConvert options:0 error:nil];
    return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
}

void processNotificationOpened() {
    [PushWrapper onDidReceiveRemoteNotification:oneSignalPush message:launchMessage additionalData:dictionaryToJsonString(launchAdditionalData) isActive:launchIsActive];
}

void initOneSignalObject(NSDictionary* launchOptions, NSString* appId, BOOL autoRegister) {
    if (oneSignal == nil) {
        [OneSignal setValue:@"coscos2dx" forKey:@"mSDKType"];
        oneSignal = [[OneSignal alloc] initWithLaunchOptions:launchOptions appId:appId handleNotification:^(NSString* message, NSDictionary* additionalData, BOOL isActive) {
            launchMessage = message;
            launchAdditionalData = additionalData;
            launchIsActive = isActive;
            
            if (initCalled)
                processNotificationOpened();
        } autoRegister:autoRegister];
    }
}

@implementation UIApplication(OneSignalCocos2dxPush)

static void injectSelector(Class newClass, SEL newSel, Class addToClass, SEL makeLikeSel) {
    Method newMeth = class_getInstanceMethod(newClass, newSel);
    IMP imp = method_getImplementation(newMeth);
    const char* methodTypeEncoding = method_getTypeEncoding(newMeth);
    
    BOOL successful = class_addMethod(addToClass, makeLikeSel, imp, methodTypeEncoding);
    if (!successful) {
        class_addMethod(addToClass, newSel, imp, methodTypeEncoding);
        newMeth = class_getInstanceMethod(addToClass, newSel);
        
        Method orgMeth = class_getInstanceMethod(addToClass, makeLikeSel);
        
        method_exchangeImplementations(orgMeth, newMeth);
    }
}

+ (void)load {
    method_exchangeImplementations(class_getInstanceMethod(self, @selector(setDelegate:)), class_getInstanceMethod(self, @selector(setOneSignalCocos2dxDelegate:)));
}

static Class delegateClass = nil;

- (void) setOneSignalCocos2dxDelegate:(id<UIApplicationDelegate>)delegate {
    if(delegateClass != nil)
        return;
    delegateClass = [delegate class];
    
    injectSelector(self.class, @selector(oneSignalApplication:didFinishLaunchingWithOptions:),
                   delegateClass, @selector(application:didFinishLaunchingWithOptions:));
    [self setOneSignalCocos2dxDelegate:delegate];
}

- (BOOL)oneSignalApplication:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    if ([launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey] != nil)
        initOneSignalObject(launchOptions, nil, true);
    
    if ([self respondsToSelector:@selector(oneSignalApplication:didFinishLaunchingWithOptions:)])
        return [self oneSignalApplication:application didFinishLaunchingWithOptions:launchOptions];
    return YES;
}

@end

@implementation OneSignalPush

- (void) initWithAppId:(NSString*)appId autoRegister:(BOOL)autoRegister {
    oneSignalPush = self;
    initCalled = true;
    initOneSignalObject(nil, appId, autoRegister);
    
    if (launchMessage)
        processNotificationOpened();
}

- (void) registerForPushNotifications {
    [oneSignal registerForPushNotifications];
}

- (void) sendTag:(NSString*)key value:(NSString*)value {
    [oneSignal sendTag:key value:value];
}

- (void) deleteTag:(NSString*)key {
    [oneSignal deleteTag:key];
}

- (void) getTags {
    [oneSignal getTags:^(NSDictionary* result) {
        [PushWrapper onReceivedTags:self  tags:dictionaryToJsonString(result)];
    }];
}

- (void) getIds {
    [oneSignal IdsAvailable:^(NSString* playerId, NSString* pushToken) {
        [PushWrapper onReceivedIds:self userid:playerId pushToken:pushToken];
    }];
}

@end
