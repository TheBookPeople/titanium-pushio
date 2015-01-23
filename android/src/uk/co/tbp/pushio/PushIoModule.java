package uk.co.tbp.pushio;

import java.util.ArrayList;
import java.util.List;

import org.appcelerator.kroll.KrollModule;
import org.appcelerator.kroll.annotations.Kroll;
import org.appcelerator.kroll.common.Log;
import org.appcelerator.titanium.TiApplication;

import android.app.Activity;

import com.pushio.manager.PushIOManager;

@Kroll.module(name = "PushIo", id = "uk.co.tbp.pushio")
public class PushIoModule extends KrollModule {

	private static final String UNKNOWN_PUSHIO_UUID = "UNKNOWN_PUSHIO_UUID";

	private static final String MODULE_NAME = "PushIo";
	private static final String LCAT = "PushIoModule";
	private static PushIOManager myPushIOManager = null;

	@Kroll.onAppCreate
	public static void onAppCreate(TiApplication app) {
		Log.d(LCAT, "[MODULE LIFECYCLE EVENT] appCreate");
		PushIOManager pushIOManager = getPushIOManager();

		if (!isSetup(pushIOManager, "ensureRegistration"))
			return;

	}

	@Override
	public void onStop(Activity activity) {
		Log.d(LCAT, "[MODULE LIFECYCLE EVENT]  stop");
		super.onStop(activity);
		PushIOManager pushIOManager = getPushIOManager();
		if (!isSetup(pushIOManager, "resetEID"))
			return;

		getPushIOManager().resetEID();
	}

	public static PushIoModule getModule() {
		TiApplication appContext = TiApplication.getInstance();
		PushIoModule uaModule = (PushIoModule)appContext.getModuleByName(MODULE_NAME);
	
		if (uaModule == null) {
			Log.w(LCAT,"PushIoModule module not currently loaded");
		}
		return uaModule;
	}

	@Kroll.method
	public void registerDevice() {
		Log.d(LCAT, "registerDevice - NOP - just included for compatibility with iOS");
	}
	
	@Kroll.method
	public void trackCustomEngagement(String engagement) {
		Log.d(LCAT, "trackCustomEngagement - "+ engagement);
	 
		PushIOManager pushIOManager = getPushIOManager();
		if (!isSetup(pushIOManager, "trackCustomEngagement"))
			return;

		getPushIOManager().trackCustomEngagement(engagement);
	}


	@Kroll.method
	public void recordNotification() {
		Log.d(LCAT, "registerUserID - NOP - just included for compatibility with iOS");
	}

	@Kroll.getProperty
	@Kroll.method
	public String getPushIOUUID() {
		Log.d(LCAT, "getPushIOUUID");

		PushIOManager pushIOManager = getPushIOManager();

		if (!isSetup(pushIOManager, "getPushIOUUID"))
			return UNKNOWN_PUSHIO_UUID;

		return pushIOManager.getUUID();
	}

	@Kroll.method
	public void registerCategories(Object[] categories) {
		PushIOManager pushIOManager = getPushIOManager();

		if (!isSetup(pushIOManager, "registerCategories"))
			return;

		List<String> categoryList = getStringList(categories);
		pushIOManager.registerCategories(categoryList, false);

	}

	@Kroll.method
	public void registerCategory(String category) {
		PushIOManager pushIOManager = getPushIOManager();

		if (!isSetup(pushIOManager, "registerCategory"))
			return;

		pushIOManager.registerCategory(category);

	}

	@Kroll.method
	public void unregisterCategory(String category) {
		PushIOManager pushIOManager = getPushIOManager();

		if (!isSetup(pushIOManager, "unregisterCategory"))
			return;

		pushIOManager.unregisterCategory(category);

	}

	@Kroll.method
	public void unregisterCategories(Object[] categories) {
		PushIOManager pushIOManager = getPushIOManager();

		if (!isSetup(pushIOManager, "unregisterCategories"))
			return;

		List<String> categoryList = getStringList(categories);
		pushIOManager.unregisterCategories(categoryList);

	}

	@Kroll.method
	public void unregisterAllCategories() {
		PushIOManager pushIOManager = getPushIOManager();

		if (!isSetup(pushIOManager, "unregisterAllCategories"))
			return;

		pushIOManager.unregisterAllCategories();

	}

	@Kroll.method
	public void registerUserID(String userId) {
		PushIOManager pushIOManager = getPushIOManager();

		if (!isSetup(pushIOManager, "registerUserId"))
			return;

		pushIOManager.registerUserId(userId);

	}

	@Kroll.method
	public void unregisterUserID() {
		PushIOManager pushIOManager = getPushIOManager();

		if (!isSetup(pushIOManager, "unregisterUserId"))
			return;

		if (pushIOManager == null) {
			Log.e(LCAT, "Cant unregisterUserId PushIO configuration not valid.");
			return;
		}

		pushIOManager.unregisterUserId();

	}

	private static boolean isSetup(PushIOManager pushIOManager, String method) {
		if (pushIOManager == null) {
			Log.e(LCAT, "Cant " + method + ", PushIO configuration not valid.");
			return false;
		}
		return true;
	}

	private List<String> getStringList(Object[] items) {
		List<String> results = new ArrayList<String>();

		for (Object item : items) {
			if (item instanceof String) {
				results.add((String) item);
			}

		}
		return results;
	}

	private static PushIOManager getPushIOManager() {

		if (myPushIOManager != null) {
			return myPushIOManager;
		}

		try {
			myPushIOManager = PushIOManager.getInstance(TiApplication.getInstance());
			myPushIOManager.ensureRegistration();
		} catch (Throwable e) {
			Log.e(LCAT, "********************** Can't setup PushIO *************************", e);
			myPushIOManager = null;
		}

		return myPushIOManager;
	}

}
