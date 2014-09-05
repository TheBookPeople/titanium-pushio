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
    
    [[PushIOManager sharedInstance] setDelegate:self];
    [[PushIOManager sharedInstance] didFinishLaunchingWithOptions:[[TiApp app] launchOptions]];
    [[PushIOManager sharedInstance] setDebugLevel:PUSHIO_DEBUG_VERBOSE];
    
    [[TiApp app] setRemoteNotificationDelegate:self];
    
	[app registerForRemoteNotificationTypes:ourNotifications];
    
    
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
    NSLog(@"[INFO] didRegisterForRemoteNotificationsWithDeviceToken %@ ",deviceToken);
    [[PushIOManager sharedInstance] didRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
    
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
     NSLog(@"[INFO] didReceiveRemoteNotification");
    [[PushIOManager sharedInstance] didReceiveRemoteNotification:userInfo];
    
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
    
     NSLog(@"[INFO] didFailToRegisterForRemoteNotificationsWithError %@ ",error);
    [[PushIOManager sharedInstance] didFailToRegisterForRemoteNotificationsWithError:error];

	// called by TiApp
	if (pushNotificationError!=nil)
	{
		NSString * message = [TiUtils messageFromError:error];
		NSMutableDictionary * event = [TiUtils dictionaryWithCode:[error code] message:message];
		[self _fireEventToListener:@"remote" withObject:event listener:pushNotificationError thisObject:nil];
	}
}


#pragma mark Push IO

- (void) readyForRegistration
{
    NSLog(@"Push IO is ready for registration");
    
    // This will register for broadcast. Alternatively, register for categories to segment.
    [[PushIOManager sharedInstance] registerWithPushIO];
}

- (void)registrationSucceeded
{
    NSLog(@"Push IO registration succeeded");
}

- (void)registrationFailedWithError:(NSError *)error statusCode:(int)statusCode
{
    NSLog(@"Push IO registration failed");
}

- (void)newNewsstandContentAvailable
{
    // Start your content download here.
    NSLog(@"Push IO new newstand content available");
}

#pragma mark Listener Notifications


#pragma Public APIs
// Any categories not present in the array will be degregistered if already registered with the server.
// An empty array is treated as an Unregister call.
- (void) registerCategories:(id)categories{
    ENSURE_ARG_COUNT(categories, 1);
    ENSURE_TYPE([categories objectAtIndex:0], NSArray);

    [[PushIOManager sharedInstance] registerCategories:[categories objectAtIndex:0]];
}

// Registers a single category, leaving any other categories still registered.
- (void) registerCategory:(id)category{
    ENSURE_ARG_COUNT(category, 1);
    ENSURE_TYPE([category objectAtIndex:0], NSString);
    
    [[PushIOManager sharedInstance] registerCategory:[category objectAtIndex:0]];
}

// Unregisters a single category, leaving all other categories in place.
- (void) unregisterCategory:(id)category{
    ENSURE_ARG_COUNT(category, 1);
    ENSURE_TYPE([category objectAtIndex:0], NSString);
    
    [[PushIOManager sharedInstance] unregisterCategory:[category objectAtIndex:0]];
}

// Unregisters a group of categories, leaving any categories not in the pased in array still registered.
- (void) unregisterCategories:(id)categories{
    ENSURE_ARG_COUNT(categories, 1);
    ENSURE_TYPE([categories objectAtIndex:0], NSString);
    
    [[PushIOManager sharedInstance] unregisterCategories:[categories objectAtIndex:0]];
}

// Unregister all categories for this device from Push IO
- (void) unregisterAllCategories{
    [[PushIOManager sharedInstance] unregisterAllCategories];
}

// Returns information on the categories for which this app has requested registration.
- (id) isRegisteredForCategory:(id)category{
    ENSURE_ARG_COUNT(category, 1);
    ENSURE_TYPE([category objectAtIndex:0], NSString);
    
    return NUMBOOL([[PushIOManager sharedInstance] isRegisteredForCategory:[category objectAtIndex:0]]);
}

- (id) allRegisteredCategories{
    return [[PushIOManager sharedInstance] allRegisteredCategories];
}

//UserID
-(void)registerUserID:(id)userID
{
    ENSURE_ARG_COUNT(userID, 1);
    ENSURE_TYPE([userID objectAtIndex:0], NSString);

    [[PushIOManager sharedInstance] registerUserID:[userID objectAtIndex:0]];
}

- (void) unregisterUserID{
     [[PushIOManager sharedInstance] unregisterUserID];
}



@end
