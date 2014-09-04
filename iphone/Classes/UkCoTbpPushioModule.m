/**
 * push-io
 *
 * Created by Your Name
 * Copyright (c) 2014 Your Company. All rights reserved.
 */

#import "UkCoTbpPushioModule.h"
#import "TiBase.h"
#import "TiHost.h"
#import "TiUtils.h"
#import "TiApp.h"

@implementation UkCoTbpPushioModule

#pragma mark Internal

// this is generated for your module, please do not change it
-(id)moduleGUID
{
	return @"0f2d1da9-5666-436a-a2ae-de5ba5103200";
}

// this is generated for your module, please do not change it
-(NSString*)moduleId
{
	return @"uk.co.tbp.pushio";
}

#pragma mark Lifecycle

-(void)startup
{
	// this method is called when the module is first loaded
	// you *must* call the superclass
	[super startup];

	NSLog(@"[INFO] %@ loaded",self);
}

-(void)shutdown:(id)sender
{
	// this method is called when the module is being unloaded
	// typically this is during shutdown. make sure you don't do too
	// much processing here or the app will be quit forceably

	// you *must* call the superclass
	[super shutdown:sender];
}

-(void)registerForPushNotifications:(id)args
{
	ENSURE_SINGLE_ARG(args,NSDictionary);
	
	UIApplication * app = [UIApplication sharedApplication];
	UIRemoteNotificationType ourNotifications = [app enabledRemoteNotificationTypes];
	
	NSArray *typesRequested = [args objectForKey:@"types"];
	
	RELEASE_TO_NIL(pushNotificationCallback);
	RELEASE_TO_NIL(pushNotificationError);
	RELEASE_TO_NIL(pushNotificationSuccess);
	
	pushNotificationSuccess = [[args objectForKey:@"success"] retain];
	pushNotificationError = [[args objectForKey:@"error"] retain];
	pushNotificationCallback = [[args objectForKey:@"callback"] retain];
	
	if (typesRequested!=nil)
	{
		for (id thisTypeRequested in typesRequested)
		{
			NSInteger value = [TiUtils intValue:thisTypeRequested];
			switch(value)
			{
				case 1: //NOTIFICATION_TYPE_BADGE
				{
					ourNotifications |= UIRemoteNotificationTypeBadge;
					break;
				}
				case 2: //NOTIFICATION_TYPE_ALERT
				{
					ourNotifications |= UIRemoteNotificationTypeAlert;
					break;
				}
				case 3: //NOTIFICATION_TYPE_SOUND
				{
					ourNotifications |= UIRemoteNotificationTypeSound;
					break;
				}
				case 4: // NOTIFICATION_TYPE_NEWSSTAND
				{
					ourNotifications |= UIRemoteNotificationTypeNewsstandContentAvailability;
					break;
				}
			}
		}
	}
	
	[[TiApp app] setRemoteNotificationDelegate:self];
	[app registerForRemoteNotificationTypes:ourNotifications];
    
    [[PushIOManager sharedInstance] setDelegate:self];
    [[PushIOManager sharedInstance] didFinishLaunchingWithOptions:nil];
	
	// check to see upon registration if we were started with a push
	// notification and if so, go ahead and trigger our callback
	id currentNotification = [[TiApp app] remoteNotification];
	if (currentNotification!=nil && pushNotificationCallback!=nil)
	{
		NSMutableDictionary * event = [TiUtils dictionaryWithCode:0 message:nil];
		[event setObject:currentNotification forKey:@"data"];
		[event setObject:NUMBOOL(YES) forKey:@"inBackground"];
		[self _fireEventToListener:@"remote" withObject:event listener:pushNotificationCallback thisObject:nil];
	}
}

-(void)unregisterForPushNotifications:(id)args
{
	UIApplication * app = [UIApplication sharedApplication];
	[app unregisterForRemoteNotifications];
}


#pragma mark Cleanup

-(void)dealloc
{
	// release any resources that have been retained by the module
	[super dealloc];
    RELEASE_TO_NIL(pushNotificationCallback);
	RELEASE_TO_NIL(pushNotificationError);
	RELEASE_TO_NIL(pushNotificationSuccess);
}

#pragma mark Internal Memory Management

-(void)didReceiveMemoryWarning:(NSNotification*)notification
{
	// optionally release any resources that can be dynamically
	// reloaded once memory is available - such as caches
	[super didReceiveMemoryWarning:notification];
}

#pragma mark Push Notification Delegates

-(void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
	// called by TiApp
	if (pushNotificationSuccess!=nil)
	{
		NSString *token = [[[[deviceToken description] stringByReplacingOccurrencesOfString:@"<"withString:@""]
							stringByReplacingOccurrencesOfString:@">" withString:@""]
						   stringByReplacingOccurrencesOfString: @" " withString: @""];
		NSMutableDictionary * event = [TiUtils dictionaryWithCode:0 message:nil];
		[event setObject:token forKey:@"deviceToken"];
		[self _fireEventToListener:@"remote" withObject:event listener:pushNotificationSuccess thisObject:nil];
	}
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
	// called by TiApp
	if (pushNotificationCallback!=nil)
	{
		NSMutableDictionary * event = [TiUtils dictionaryWithCode:0 message:nil];
		[event setObject:userInfo forKey:@"data"];
		BOOL inBackground = (application.applicationState != UIApplicationStateActive);
		[event setObject:NUMBOOL(inBackground) forKey:@"inBackground"];
		[self _fireEventToListener:@"remote" withObject:event listener:pushNotificationCallback thisObject:nil];
	}
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
	// called by TiApp
	if (pushNotificationError!=nil)
	{
		NSString * message = [TiUtils messageFromError:error];
		NSMutableDictionary * event = [TiUtils dictionaryWithCode:[error code] message:message];
		[self _fireEventToListener:@"remote" withObject:event listener:pushNotificationError thisObject:nil];
	}
}

#pragma mark Listener Notifications


#pragma Public APIs
// Any categories not present in the array will be degregistered if already registered with the server.
// An empty array is treated as an Unregister call.
- (void) registerCategories:(id)categories{
    ENSURE_TYPE(categories, NSArray);
    [[PushIOManager sharedInstance] registerCategories:categories];
}

// Registers a single category, leaving any other categories still registered.
- (void) registerCategory:(id)category{
    ENSURE_TYPE(category, NSString);
    [[PushIOManager sharedInstance] registerCategory:category];
}

// Unregisters a single category, leaving all other categories in place.
- (void) unregisterCategory:(id)category{
    ENSURE_TYPE(category, NSString);
    [[PushIOManager sharedInstance] unregisterCategory:category];
}

// Unregisters a group of categories, leaving any categories not in the pased in array still registered.
- (void) unregisterCategories:(id)categories{
    ENSURE_TYPE(categories, NSArray);
    [[PushIOManager sharedInstance] unregisterCategories:categories];
}

// Unregister all categories for this device from Push IO
- (void) unregisterAllCategories{
    [[PushIOManager sharedInstance] unregisterAllCategories];
}

// Returns information on the categories for which this app has requested registration.
- (id) isRegisteredForCategory:(id)category{
    ENSURE_TYPE(category, NSString);
    return NUMBOOL([[PushIOManager sharedInstance] isRegisteredForCategory:category]);
}

- (id) allRegisteredCategories{
    return [[PushIOManager sharedInstance] allRegisteredCategories];
}

//UserID
-(void)registerUserID:(id)userID
{
    ENSURE_TYPE(userID, NSString);
    [[PushIOManager sharedInstance] registerUserID:userID];
}

- (void) unregisterUserID{
     [[PushIOManager sharedInstance] unregisterUserID];
}



@end
