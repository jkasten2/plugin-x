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
package org.cocos2dx.plugin;

public class PushWrapper {

	public static void onDidReceiveRemoteNotification(InterfacePush obj, String msg, String additionalData, boolean isActive) {
		final String curMsg = msg;
		final String curAdditionalData = additionalData;
		final boolean curIsActive = isActive;
		final InterfacePush curAdapter = obj;
		
		PluginWrapper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				String name = curAdapter.getClass().getName();
				name = name.replace('.', '/');
				nativeOnDidReceiveRemoteNotification(name, curMsg, curAdditionalData, curIsActive);
			}
		});
	}
	
	private static native void nativeOnDidReceiveRemoteNotification(String className, String msg, String additionalData, boolean isActive);

	public static void onReceivedTags(InterfacePush obj, String tags) {
		final String curTags = tags;
		final InterfacePush curAdapter = obj;
		
		PluginWrapper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				String name = curAdapter.getClass().getName();
				name = name.replace('.', '/');
				nativeOnReceivedTags(name, curTags);
			}
		});
	}
	
	private static native void nativeOnReceivedTags(String className, String tags);

	public static void onReceivedIds(InterfacePush obj, String userId, String registrationId) {
		final String curUserId = userId;
		final String curRegistrationId = registrationId;
		final InterfacePush curAdapter = obj;
		
		PluginWrapper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				String name = curAdapter.getClass().getName();
				name = name.replace('.', '/');
				nativeOnReceivedIds(name, curUserId, curRegistrationId);
			}
		});
	}
	
	private static native void nativeOnReceivedIds(String className, String userId, String pushToken);
}