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
#include "MyPushManager.h"
#include "PluginManager.h"
#include "cocos2d.h"
#include "Configs.h"
#include "ProtocolPush.h"

#define COCOS2D_DEBUG 1

using namespace cocos2d::plugin;
using namespace cocos2d;

MyPushManager* MyPushManager::s_pManager = NULL;

PushReceived MyPushManager::_pushReceived;

MyPushManager::MyPushManager() { }

MyPushManager::~MyPushManager() {
	unloadPlugin();
}

MyPushManager* MyPushManager::getInstance() {
	if (s_pManager == NULL) {
		s_pManager = new MyPushManager();
	}
	return s_pManager;
}

void MyPushManager::purgeManager() {
	if (s_pManager) {
		delete s_pManager;
		s_pManager = NULL;
	}
	PluginManager::end();
}

void MyPushManager::loadPlugin() {
	_push = (cocos2d::plugin::ProtocolPush*)PluginManager::getInstance()->loadPlugin("OneSignalPush");
    CCLog("MyPushManager::loadPlugin:%d", _push == NULL);
}

void MyPushManager::HandleReceivedNotification(const char* message, const char* additionalData, bool isActive) {
    CCLog("MyPushManager::HandleReceivedNotification");
    CCLog("message: %s", message);
    CCLog("additionalData: %s", additionalData);
    _pushReceived(message);
}

void MyPushManager::unloadPlugin() {
	if (_push) {
		PluginManager::getInstance()->unloadPlugin("OneSignalPush");
		_push = NULL;
	}
}

void MyPushManager::init(PushReceived cb) {
	if (_push) {
        _pushReceived = cb;
	    _push->init("b49e69ca-d0b8-11e3-97bf-c3d1433e8bc1", "703322744261", (ProtocolPushDidReceiveRemoteNotification) HandleReceivedNotification);
	}
}

void MyPushManager::sendTag() {
    if (_push) {
        _push->sendTag("cocos2d-x", "demo");
    }
}
