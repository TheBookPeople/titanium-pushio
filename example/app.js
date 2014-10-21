//open a single window
var win = Ti.UI.createWindow({
  backgroundColor:'white'
});

var label = Ti.UI.createLabel();
win.add(label);
//win.open();



var pushio = require('uk.co.tbp.pushio');

// Check if the device is running iOS 8 or later
 if (Ti.Platform.name == "iPhone OS" && parseInt(Ti.Platform.version.split(".")[0]) >= 8) {
   function registerForPush() {
    Ti.Network.registerForPushNotifications({
      success : deviceTokenSuccess,
      error : deviceTokenError,
      callback : receivePush
    });
    // Remove event listener once registered for push notifications
    Ti.App.iOS.removeEventListener('usernotificationsettings', registerForPush);
  };

  // Wait for user settings to be registered before registering for push notifications
  Ti.App.iOS.addEventListener('usernotificationsettings', registerForPush);

  // Register notification types to use
  Ti.App.iOS.registerUserNotificationSettings({
    types : [Ti.App.iOS.USER_NOTIFICATION_TYPE_ALERT, Ti.App.iOS.USER_NOTIFICATION_TYPE_SOUND, Ti.App.iOS.USER_NOTIFICATION_TYPE_BADGE]
 });
 
 } else {
  // For iOS 7 and earlier
  Ti.Network.registerForPushNotifications({
    // Specifies which notifications to receive
    types : [Ti.Network.NOTIFICATION_TYPE_BADGE, Ti.Network.NOTIFICATION_TYPE_ALERT, Ti.Network.NOTIFICATION_TYPE_SOUND],
    success : deviceTokenSuccess,
    error : deviceTokenError,
    callback : receivePush
  });
}

// Save the device token for subsequent API calls
function deviceTokenSuccess(e) {
  Ti.API.info("pushio.pushIOUUID() "+pushio.pushIOUUID);
  
  pushio.registerDevice(e.deviceToken);
  pushio.registerUserID('a@bb.com');
  pushio.registerCategories(['Socks','Burgers']);

  pushio.unregisterCategory('Bannana');
  pushio.unregisterCategories(['Socks','Burgers']);

  pushio.unregisterCategory('Bannana');
  pushio.unregisterCategories(['Socks','Burgers']);

  pushio.unregisterAllCategories();

  pushio.isRegisteredForCategory();

  pushio.allRegisteredCategories();

  pushio.registerUserID();

  pushio.unregisterUserID();

  pushio.registeredUserID();

  pushio.isRegisteredForUserID();

  pushio.trackEngagementCustomMetric();

  pushio.registerUserID('a@bb.com');
  
}

function deviceTokenError(e) {
   Ti.API.error('Failed to register for push notifications! ' + e.error);
}

function receivePush(e) {
  Ti.API.error('Recived Push' + JSON.stringify(e,null,""));
  
  pushio.recordNotification(e.data);
  
  
    
}




