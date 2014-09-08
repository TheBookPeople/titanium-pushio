# PushIO Module

This is a Titanium Mobile Mobile module project that adds PushIO
(http://www.responsys.com/marketing-cloud/products/push-IO) notifications to a project. 
This module currently only supports iOS. You will need a PushIO account to use this module.


## Accessing the push-io Module

To access this module from JavaScript, you would do the following:

    var push_io = require("uk.co.tbp.pushio");

The push_io variable is a reference to the Module object.

## Setup


### pushio___config_debug.json

The  pushio_config_debug.json file that is created in the PushIO management interface needs to be copied to
the assets folder of your titanium application.

### PushIOManager.framework

PushIO have created a iOS framework for their service. it can be downloaded from 

https://github.com/pushio/PushIOManager_iOS

module.xcconfig needs to be updated so that it can find the framework file.

OTHER_LDFLAGS=$(inherited) -F"[PUSHIO_FRAMEWOK_DIR]"  -framework PushIOManager

### Build

To build the module run the following code from the iphone folder

```
./build.py
```

## Usage


### Registering for notification
The registration method has the same signature as the built in Titanium Ti.Network.registerForPushNotifications.
It also uses the same constant for determining the type of notification to accept. When the registerForPushNotifications
method is called if it is successful the device will be registered for broadcast messages.

```javascript
  var push_io = require('uk.co.tbp.pushio');
  push_io.registerForPushNotifications({
    // Specifies which notifications to receive
    types : [Ti.Network.NOTIFICATION_TYPE_BADGE, 
             Ti.Network.NOTIFICATION_TYPE_ALERT, 
             Ti.Network.NOTIFICATION_TYPE_SOUND],

    success : function(e) {
      alert('Device token : ' + JSON.stringify(e));
    },

    error : function(e) {
      alert('error: ' + JSON.stringify(e));
    },

    callback : function(e) {
      alert('callback: ' + JSON.stringify(e));
      }
  });
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

