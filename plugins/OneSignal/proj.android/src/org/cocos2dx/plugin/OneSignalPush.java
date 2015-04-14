/**
 * Copyright 2015 OneSignal
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.cocos2dx.plugin;

import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import com.onesignal.OneSignal;
import com.onesignal.OneSignal.GetTagsHandler;
import com.onesignal.OneSignal.IdsAvailableHandler;
import com.onesignal.OneSignal.NotificationOpenedHandler;

public class OneSignalPush implements InterfacePush, PluginListener {
	private Context mContext;
	private static OneSignalPush mInstance;
	
	public OneSignalPush(Context context) {
		mContext = context;
		mInstance = this;
	}
	
	public void init(String googleProjectNumber, String appId) {
		OneSignal.init((Activity)mContext, googleProjectNumber, appId, new NotificationOpenedHandler() {
			public void notificationOpened(String message, JSONObject additionalData, boolean isActive) {
				String curAdditionalData = null;
				if (additionalData != null)
					curAdditionalData = additionalData.toString();
				PushWrapper.onDidReceiveRemoteNotification(mInstance, message, curAdditionalData, isActive);
			}
		});
		
		PluginWrapper.addListener(this);
	}
	
	public void sendTag(String key, String value) {
		OneSignal.sendTag(key, value);
	}
	
	public void deleteTag(String key) {
		OneSignal.deleteTag(key);
	}
	
	public void getTags() {
		OneSignal.getTags(new GetTagsHandler() {
			@Override
			public void tagsAvailable(JSONObject tags) {
				try {
					PushWrapper.onReceivedTags(mInstance, tags.toString());
				} catch (Throwable t) {
					t.printStackTrace();
				}
			}
		});
	}
	
	public void getIds() {
		OneSignal.idsAvailable(new IdsAvailableHandler() {
			@Override
			public void idsAvailable(String userId, String registrationId) {
				try {
					PushWrapper.onReceivedIds(mInstance, userId, registrationId);
				} catch (Throwable t) {
					t.printStackTrace();
				}
			}
		});
	}

	@Override
	public void onResume() {
		OneSignal.onResumed();
	}

	@Override
	public void onPause() {
		OneSignal.onPaused();
	}

	@Override
	public void onDestroy() {
	}

	@Override
	public boolean onActivityResult(int requestCode, int resultCode, Intent data) {
		return false;
	}
}