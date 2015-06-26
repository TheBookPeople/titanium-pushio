package uk.co.tbp.pushio;

import java.util.HashMap;
import java.util.Map;

import android.app.AlarmManager;
import android.app.IntentService;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.PowerManager;
import android.os.SystemClock;
import android.support.v4.app.NotificationCompat;
import android.text.TextUtils;
import android.util.Log;

import com.pushio.manager.PushIOActivityLauncher;
import com.pushio.manager.PushIOManager;
import com.pushio.manager.PushIOSharedPrefs;
import com.pushio.manager.utils.CommonUtils;

public class PushIOGCMIntentService extends IntentService {

	private static final Object LOCK = PushIOGCMIntentService.class;
	private static String KEY_REGISTRATIONID = "registration_id";
	private static String KEY_ERROR = "error";
	private static String KEY_UNREGISTERED = "unregistered";
	private static String ERROR_TYPE_SERVICE_NOT_AVAILABLE = "SERVICE_NOT_AVAILABLE";
	private static String PUSH_KEY_ALERT = "alert";

	private static int MAX_BACKOFF_DURATION_MS = 0xdbba00;
	private static android.os.PowerManager.WakeLock sWakeLock;
	private Map mPushServiceActionMap;
	private BroadcastReceiver mBoomerangReceiver;

	public PushIOGCMIntentService() {
		super("PushIOGCMIntentService");

		mPushServiceActionMap = new HashMap();
		mBoomerangReceiver = new BroadcastReceiver() {

			public void onReceive(Context context, Intent intent) {
				int lPushStatus = getResultExtras(true).getInt(PushIOManager.PUSH_STATUS, PushIOManager.PUSH_UNHANDLED);
				if (lPushStatus == PushIOManager.PUSH_HANDLED_NOTIFICATION)
					return;
				if (lPushStatus == PushIOManager.PUSH_HANDLED_IN_APP) {
					PushIOSharedPrefs lSharedPrefs = new PushIOSharedPrefs(getApplicationContext());
					lSharedPrefs.setEID(intent.getStringExtra(PushIOManager.PUSHIO_ENGAGEMENTID_KEY));
					lSharedPrefs.commit();
					PushIOManager lPushIOManager = PushIOManager.getInstance(context);
					lPushIOManager.trackEngagement(PushIOManager.PUSHIO_ENGAGEMENT_METRIC_ACTIVE_SESSION,
							intent.getStringExtra(PushIOManager.PUSHIO_ENGAGEMENTID_KEY));
					return;
				}
				if (lPushStatus == PushIOManager.PUSH_UNHANDLED && intent.hasExtra(PushIOGCMIntentService.PUSH_KEY_ALERT)
						&& !TextUtils.isEmpty(intent.getStringExtra(PushIOGCMIntentService.PUSH_KEY_ALERT)))

					notifyUser(context, intent);

			}

		};
		mPushServiceActionMap.put("GCM", "com.google.android.c2dm.intent.RECEIVE");
		mPushServiceActionMap.put("ADM", "com.amazon.device.messaging.intent.RECEIVE");
	}

	static void runIntentInService(Context context, Intent intent) {
		synchronized (LOCK) {
			if (sWakeLock == null) {
				PowerManager pm = (PowerManager) context.getSystemService("power");
				sWakeLock = pm.newWakeLock(1, "PushIOGCMWakeLock");
			}
		}
		sWakeLock.acquire();
		intent.setClassName(context, PushIOGCMIntentService.class.getName());
		context.startService(intent);
	}

	public final void onHandleIntent(Intent intent) {
		Log.d("pushio", "************************** WG Push Broadcast");
		String action = intent.getAction();
		if (action.equals("com.google.android.c2dm.intent.REGISTRATION") || action.equals("com.amazon.device.messaging.intent.REGISTRATION")) {
			PushIOSharedPrefs lPrefs = new PushIOSharedPrefs(getApplicationContext());
			if (intent.hasExtra(KEY_ERROR)) {
				String lError = intent.getStringExtra(KEY_ERROR);
				if (lError.equals(ERROR_TYPE_SERVICE_NOT_AVAILABLE)) {
					long backoffTimeMs = lPrefs.getBackoffTime();
					if (backoffTimeMs == 0L)
						backoffTimeMs = 1000L;
					long nextAttempt = SystemClock.elapsedRealtime() + backoffTimeMs;
					Intent retryIntent = new Intent("com.pushio.manager.push.intent.RETRY");
					PendingIntent retryPendingIntent = PendingIntent.getBroadcast(this, 0, retryIntent, 0);
					AlarmManager lAlarmManager = (AlarmManager) getSystemService("alarm");
					lAlarmManager.set(3, nextAttempt, retryPendingIntent);
					Log.e("pushio",
							(new StringBuilder()).append("Push source registration error. Not available. Retrying in ").append(backoffTimeMs).append(" MS")
									.toString());
					backoffTimeMs *= 2L;
					if (backoffTimeMs > (long) MAX_BACKOFF_DURATION_MS)
						backoffTimeMs = MAX_BACKOFF_DURATION_MS;
					lPrefs.setBackoffTime(backoffTimeMs);
					lPrefs.commit();
				} else {
					Log.e("pushio", (new StringBuilder()).append("Push source registration error. Code=").append(intent.getStringExtra(KEY_ERROR)).toString());
				}
			} else if (intent.hasExtra(KEY_UNREGISTERED) && intent.getBooleanExtra(KEY_UNREGISTERED, false)) {
				Log.d("pushio", "Unregistered from ADM.");
				lPrefs.setProjectId(null);
				lPrefs.commit();
				PushIOManager.getInstance(this).registerCategories(null, false);
			} else {
				String lRegistrationId = intent.getStringExtra(KEY_REGISTRATIONID);
				Log.d("pushio", (new StringBuilder()).append("Push registration received. id: ").append(lRegistrationId).toString());
				lPrefs.setRegistrationKey(lRegistrationId);
				lPrefs.setBackoffTime(0L);
				int lVersionCode = -1;
				try {
					lVersionCode = getPackageManager().getPackageInfo(getPackageName(), 0).versionCode;
				} catch (NameNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				lPrefs.setLastVersion(lVersionCode);
				lPrefs.commit();
				PushIOManager.getInstance(this).registerCategories(null, false);
				Log.d("pushio", (new StringBuilder()).append("Device Token= ").append(lRegistrationId).toString());
			}
		} else if (action.equals("com.google.android.c2dm.intent.RECEIVE") || action.equals("com.amazon.device.messaging.intent.RECEIVE")) {
			Log.d("pushio", "Push received!");
			PushIOSharedPrefs lPrefs = new PushIOSharedPrefs(getApplicationContext());
			String registeredPushService = lPrefs.getNotificationService();
			if (!TextUtils.isEmpty(registeredPushService)) {
				if (((String) mPushServiceActionMap.get(registeredPushService)).equalsIgnoreCase(action))
					handleMessage(intent);
				else
					Log.d("pushio", "Push service registered is different from this message sender. Received message will be ignored");
			} else {
				Log.d("pushio", "No push service registered. Received message will be ignored");
			}
		} else if (action.equals("com.pushio.manager.push.intent.RETRY")) {
			Log.d("pushio", "Retry received.");
			PushIOSharedPrefs lPrefs = new PushIOSharedPrefs(getApplicationContext());
			Intent lIntent = new Intent(lPrefs.getNotificationService().equals("GCM") ? "com.google.android.c2dm.intent.REGISTER"
					: "com.amazon.device.messaging.intent.REGISTER");
			lIntent.putExtra("app", PendingIntent.getBroadcast(getApplicationContext(), 0, new Intent(), 0));
			lIntent.putExtra("sender", lPrefs.getProjectId());
			if (android.os.Build.VERSION.SDK_INT >= 21)
				lIntent = CommonUtils.createExplicitFromImplicitIntent(getApplicationContext(), lIntent);
			if (lIntent != null)
				getApplicationContext().startService(lIntent);
			else
				Log.w("pushio", "Device supports no known notification services.");
		}
		synchronized (LOCK) {
			if (sWakeLock != null && sWakeLock.isHeld())
				sWakeLock.release();
		}

	}

	public void handleMessage(Intent intent) {
		String containerPackageName = getApplicationContext().getPackageName();
		String lIntentStr = (new StringBuilder()).append(containerPackageName).append(".PUSHIOPUSH").toString();
		Intent lCustomNotificationIntent = new Intent(lIntentStr);
		lCustomNotificationIntent.putExtras(intent);
		lCustomNotificationIntent.setPackage(containerPackageName);
		String broadcastPermissionStr = (new StringBuilder()).append(containerPackageName).append(".permission.PUSHIO_MESSAGE").toString();
		sendOrderedBroadcast(lCustomNotificationIntent, broadcastPermissionStr, mBoomerangReceiver, null, 0, null, null);
	}

	private void notifyUser(Context context, Intent intent) {
		PushIOSharedPrefs sharedPrefs = new PushIOSharedPrefs(getApplicationContext());
		NotificationManager lNotificationManager = (NotificationManager) getSystemService("notification");
		String alert = intent.getStringExtra(PUSH_KEY_ALERT);
		int notificationId = sharedPrefs.getIsNotificationsStacked() ? 0 : sharedPrefs.getAndIncrementNotificationCount();
		Log.d("pushio", (new StringBuilder()).append("alert=").append(alert).toString());
		
		Intent launchIntent = new Intent(context, PushIOActivityLauncher.class);
		launchIntent.putExtras(intent);
		int pendingIntentFlag = sharedPrefs.getIsNotificationsStacked() ? 0x8000000 : 0x40000000;
		PendingIntent pendingIntent = PendingIntent.getActivity(context, notificationId, launchIntent, pendingIntentFlag);
		String alertStrings[] = alert.split("\\n");
		String titleText;
		String contentText;
		if (alertStrings.length >= 2) {
			titleText = alertStrings[0].trim();
			contentText = alertStrings[1].trim();
		} else {
			titleText = getResources().getString(getApplicationInfo().labelRes);
			contentText = alert;
		}

		Notification notification;

		Notification.Builder notificationBuilder = new Notification.Builder(getApplicationContext());
		notificationBuilder = notificationBuilder.setContentTitle(titleText)
				                                 .setContentText(contentText)
				                                 .setSmallIcon(sharedPrefs.getSmallDefaultIcon())
				                                 .setTicker(alert)
				                                 .setContentIntent(pendingIntent)
				                                 .setAutoCancel(true);
		
		if(android.os.Build.VERSION.SDK_INT >= 21){
			notificationBuilder.setColor(Color.rgb(230, 42, 18));
		}
		

		if (sharedPrefs.getLargeDefaultIcon() != 0) {
			Bitmap largeIconBitmap = BitmapFactory.decodeResource(getResources(), sharedPrefs.getLargeDefaultIcon());
			notificationBuilder = notificationBuilder.setLargeIcon(largeIconBitmap);
		}

	
			notification = notificationBuilder.getNotification();
			

		lNotificationManager.notify(notificationId, notification);
	}

}
