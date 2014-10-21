# PushIO Module

This is a Titanium Mobile Mobile module project that adds PushIO
(http://www.responsys.com/marketing-cloud/products/push-IO) notifications to a project. 

This module currently only supports iOS. You will need a PushIO account to use this module.

## Setup

### Build

To build the module run the following code from the iphone folder

```shell
./build.py
```

### Install - Mac OS X

Extract distribution zip file
Copy modules/iphone/uk.co.tbp.pushio to ~/Library/Application Support/Titanium/modules/iphone

### Register the Module

Register the module with your application by editing `tiapp.xml` and adding your module.
Example:

```xml
<modules>
	<module version="1.0">uk.co.tbp.pushio</module>
</modules>
```

## Accessing the push-io Module

To access this module from JavaScript, you would do the following:

```javascript
var push_io = require("uk.co.tbp.pushio");
```

The push_io variable is a reference to the Module object.



When you run your project, the compiler will combine your module along with its dependencies and assets into the application.

### pushio_config_debug.json

The  pushio_config_debug.json file that is created in the PushIO management interface needs to be copied to
the assets folder of your titanium application.

### PushIOManager.framework

PushIO have created a iOS framework for their service. it can be downloaded from 

https://github.com/pushio/PushIOManager_iOS

module.xcconfig needs to be updated to point the absolute path of the folder containing PushIOManager.framework. The current supported
version of the framework has been included for convenience.

e.g.
```
OTHER_LDFLAGS=$(inherited) -F"/Users/[username]/Development/projects/titanium-pushio/iphone"  -framework PushIOManager
```

### Build

To build the module run the following code from the iphone folder

```
./build.py
```

## Usage


### Registering for notification

Use the Ti.Network.registerForPushNotifications method to register for notifications. This is usually placed in alloy.js

```javascript
var deviceToken = null;
var pushio = require('uk.co.tbp.pushio');

Ti.Network.registerForPushNotifications({
  // Specifies which notifications to receive
  types : [ Ti.Network.NOTIFICATION_TYPE_BADGE, 
            Ti.Network.NOTIFICATION_TYPE_ALERT, 
            Ti.Network.NOTIFICATION_TYPE_SOUND],
  success : deviceTokenSuccess,
  error : deviceTokenError,
  callback : receivePush
});

// Process incoming push notifications
function receivePush(e) {
  pushio.recordNotification(e.data);
  alert('Received push: ' + JSON.stringify(e));
}

// Save the device token for subsequent API calls
function deviceTokenSuccess(e) {
  pushio.registerDevice(e.deviceToken);
  deviceToken = e.deviceToken;
}

function deviceTokenError(e) {
  alert('Failed to register for push notifications! ' + e.error);
}
```  

## PushIO

A unique ID used by Push IO. You can use this for adding test devices at https://manage.push.io This call will always return a non-null value.

```javascript
Ti.API.Info(push_io.pushIOUUID); 
``` 

## Categories

This section has method calls associated with notification categories.

### Registering for Category

To register for a category use the registerCategory method on the push_io variable.

```javascript
push_io.registerCategory('Banana'); 
``` 

### Registering multiple categories

To register for multiple categories at once use the registerCategory method on the push_io variable.

```javascript
push_io.registerCategories(['Apples','Oranges']);
``` 
### Unregistering category

To unregister for a category use the unregisterCategory method on the push_io variable.

```javascript
push_io.unregisterCategory('Banana'); 
``` 

### Unregistering for multiple categories

To unregister for a category use the unregisterCategory method on the push_io variable.

```javascript
push_io.unregisterCategories(['Apples','Oranges']); 
``` 
### Unregistering all categories

To unregister for a category use the unregisterCategory method on the push_io variable.

```javascript
push_io.unregisterAllCategories(); 
``` 

### Check if category registered

The isRegisteredForCategory method can be used to check if you are registered for a category.

```javascript
if(push_io.isRegisteredForCategory('Bannana')){
  Ti.API.Info('We will have been registered for Bannanas');
}
``` 

## User identifier

A user identifier can be assosiated with a device. If you are only using PushIO, then they recommend that you use a hash value. However
if you need to integrate with Responsys you will need to use the plain value.  

The following identifies are supported for Responsys integration.

Email

Phone Number

Customer Id 

You must use the same identifier type for all users. 

### Register a user identifier

To register a user identifier with a device call the registerUserID method. This is usually done after a successfully
logout event.  

```javascript
push_io.registerUserID('me@example.com'); 
``` 
### Unregister a user identifier

To unregister a user identifier with a device call the unregisterUserID method. This is usually done after a successfully
logout event.

```javascript
push_io.unregisterUserID('me@example.com'); 
``` 

### Check if a user identifier is registered agains a device

The isRegisteredForUserID method can be used to check if you are registered for the device.

```javascript
if(push_io.isRegisteredForUserID('me@example.com')){
  Ti.API.Info('We will have been associated with this device.');
}
``` 

### Get user identifier registered agains a device

The registeredUserID method returns the current user identifier registered for the device.

```javascript
push_io.registeredUserID()
``` 


## Metrics

Custom engagement metrics can be tracked using the trackEngagementCustomMetric method.  

```javascript
push_io.trackEngagementCustomMetric('Purchased')
``` 

