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

#ifndef __CCX_PROTOCOL_PUSH_H__
#define __CCX_PROTOCOL_PUSH_H__

#include "PluginProtocol.h"

namespace cocos2d { namespace plugin {

	typedef void (*ProtocolPushDidReceiveRemoteNotification) (const char*, const char*, bool);
	typedef void (*ProtocolPushReceivedTags) (const char*);
	typedef void (*ProtocolPushReceivedIds) (const char*, const char*);

	class ProtocolPush : public PluginProtocol {
		public:
		    ProtocolPush();
		    virtual ~ProtocolPush();

		    void init(const char* appId, const char* googleProjectNumber, ProtocolPushDidReceiveRemoteNotification cb, bool autoRegister = true);

		    void sendTag(const char* key, const char* value);

		    void deleteTag(const char* key);

		    void getTags(ProtocolPushReceivedTags cb);

		    void getIds(ProtocolPushReceivedIds cb);

		    inline ProtocolPushDidReceiveRemoteNotification& getCallbackDidReceiveRemoteNotification() {
		        return _notificationReceivedCallback;
		    }

		    inline ProtocolPushReceivedTags& getCallbackReceivedTags() {
		        return _receivedTagsCallback;
		    }

		    inline ProtocolPushReceivedIds& getCallbackReceivedIds() {
		        return _receivedIdsCallback;
		    }

		protected:
			ProtocolPushDidReceiveRemoteNotification _notificationReceivedCallback;
			ProtocolPushReceivedTags _receivedTagsCallback;
			ProtocolPushReceivedIds _receivedIdsCallback;
	};

}} // namespace cocos2d { namespace plugin {

#endif /* __CCX_PROTOCOL_USER_H__ */