/**
 * Copyright 2014 GameThrive
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

#import <GameThrive/GameThrive.h>
#import "GameThrivePush.h"


GameThrivePush* gameThrivePush;
GameThrive* gameThrive;

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
    [PushWrapper onDidReceiveRemoteNotification:gameThrivePush message:launchMessage additionalData:dictionaryToJsonString(launchAdditionalData) isActive:launchIsActive];
}

void initGameThriveObject(NSDictionary* launchOptions, NSString* appId, BOOL autoRegister) {
    if (gameThrive == nil) {
        gameThrive = [[GameThrive alloc] initWithLaunchOptions:launchOptions appId:appId handleNotification:^(NSString* message, NSDictionary* additionalData, BOOL isActive) {
            launchMessage = message;
            launchAdditionalData = additionalData;
            launchIsActive = isActive;
            
            if (initCalled)
                processNotificationOpened();
        } autoRegister:autoRegister];
    }
}

@implementation UIApplication(GameThriveCocos2dxPush)

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
    method_exchangeImplementations(class_getInstanceMethod(self, @selector(setDelegate:)), class_getInstanceMethod(self, @selector(setGameThriveCocos2dxDelegate:)));
}

static Class delegateClass = nil;

- (void) setGameThriveCocos2dxDelegate:(id<UIApplicationDelegate>)delegate {
    if(delegateClass != nil)
        return;
    delegateClass = [delegate class];
    
    injectSelector(self.class, @selector(gameThriveApplication:didFinishLaunchingWithOptions:),
                   delegateClass, @selector(application:didFinishLaunchingWithOptions:));
    [self setGameThriveCocos2dxDelegate:delegate];
}

- (BOOL)gameThriveApplication:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {
    if ([launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey] != nil)
        initGameThriveObject(launchOptions, nil, true);
    
    if ([self respondsToSelector:@selector(gameThriveApplication:didFinishLaunchingWithOptions:)])
        return [self gameThriveApplication:application didFinishLaunchingWithOptions:launchOptions];
    return YES;
}

@end

@implementation GameThrivePush

- (void) initWithAppId:(NSString*)appId autoRegister:(BOOL)autoRegister {
    gameThrivePush = self;
    initCalled = true;
    initGameThriveObject(nil, appId, autoRegister);
    
    if (launchMessage)
        processNotificationOpened();
}

- (void) registerForPushNotifications {
    [gameThrive registerForPushNotifications];
}

- (void) sendTag:(NSString*)key value:(NSString*)value {
    [gameThrive sendTag:key value:value];
}

- (void) deleteTag:(NSString*)key {
    [gameThrive deleteTag:key];
}

- (void) getTags {
    [gameThrive getTags:^(NSDictionary* result) {
        [PushWrapper onReceivedTags:self  tags:dictionaryToJsonString(result)];
    }];
}

- (void) getIds {
    [gameThrive IdsAvailable:^(NSString* playerId, NSString* pushToken) {
        [PushWrapper onReceivedIds:self userid:playerId pushToken:pushToken];
    }];
}

@end
