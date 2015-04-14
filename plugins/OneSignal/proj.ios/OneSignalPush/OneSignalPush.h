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

#import <Foundation/Foundation.h>
#import "InterfacePush.h"

@interface OneSignalPush : NSObject<InterfacePush> {

}

- (void) initWithAppId:(NSString*)appId autoRegister:(BOOL)autoRegister;
- (void) registerForPushNotifications;
- (void) sendTag:(NSString*)key value:(NSString*)value;
- (void) deleteTag:(NSString*)key;
- (void) getTags;
- (void) getIds;

@end
