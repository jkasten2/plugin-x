/**
 * Copyright 2014 GameThrive
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

import com.gamethrive.GameThrive;
import com.gamethrive.GameThrive.GetTagsHandler;
import com.gamethrive.GameThrive.IdsAvailableHandler;
import com.gamethrive.NotificationOpenedHandler;

public class GameThrivePush implements InterfacePush, PluginListener {
	private Context mContext;
	private GameThrive gameThrive;
	private static GameThrivePush mInstance;
	
	public GameThrivePush(Context context) {
		mContext = context;
		mInstance = this;
	}
	
	public void init(String googleProjectNumber, String appId) {
		gameThrive = new GameThrive((Activity)mContext, googleProjectNumber, appId, new NotificationOpenedHandler() {
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
		if (gameThrive != null)
			gameThrive.sendTag(key, value);
	}
	
	public void deleteTag(String key) {
		if (gameThrive != null)
			gameThrive.deleteTag(key);
	}
	
	public void getTags() {
		if (gameThrive != null) {
			gameThrive.getTags(new GetTagsHandler() {
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
	}
	
	public void getIds() {
		if (gameThrive != null) {
			gameThrive.idsAvailable(new IdsAvailableHandler() {
				@Override
				public void idsAvailable(String playerId, String registrationId) {
					try {
						PushWrapper.onReceivedIds(mInstance, playerId, registrationId);
					} catch (Throwable t) {
						t.printStackTrace();
					}
				}
			});
		}
	}

	@Override
	public void onResume() {
		if (gameThrive != null)
			gameThrive.onResumed();
	}

	@Override
	public void onPause() {
		if (gameThrive != null)
			gameThrive.onPaused();
	}

	@Override
	public void onDestroy() {
	}

	@Override
	public boolean onActivityResult(int requestCode, int resultCode, Intent data) {
		return false;
	}
}