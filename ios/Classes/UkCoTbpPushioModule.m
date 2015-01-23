/**
 * Titatanium PushIO Module
 *
 * Created by The Book People Ltd
 * Copyright (c) 2014 Your Company. All rights reserved.
 *
 * This file is part of Titatanium PushIO Module.
 * Titatanium PushIO Module is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * Titatanium PushIO Module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with Titatanium PushIO Module.  If not, see <http://www.gnu.org/licenses/>.
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
    
    NSLog(@"[DEBUG] Startup PushIO module %@",self);
}

-(void)shutdown:(id)sender
{
     NSLog(@"[DEBUG] Shutdown PushIO module");
	// this method is called when the module is being unloaded
	// typically this is during shutdown. make sure you don't do too
	// much processing here or the app will be quit forceably

	// you *must* call the superclass
	[super shutdown:sender];
}

-(void)load
{
    NSLog(@"[DEBUG] Load PushIO module");
    
    [[PushIOManager sharedInstance] setDelegate:self];
    [[PushIOManager sharedInstance] didFinishLaunchingWithOptions:[[TiApp app] launchOptions]];
    [[PushIOManager sharedInstance] setDebugLevel:PUSHIO_DEBUG_VERBOSE];
}

-(void)registerDevice:(id)arg
{
	ENSURE_SINGLE_ARG(arg, NSString);
    NSLog(@"[DEBUG] PushIO registerDevice");

   
    // The token received in the success callback to 'Ti.Network.registerForPushNotifications' is a hex-encode
    // string. We need to convert it back to it's byte format as an NSData object.
    NSMutableData *deviceToken = [[NSMutableData alloc] init];
    unsigned char whole_byte;
    char byte_chars[3] = { '\0', '\0', '\0' };
    int i;
    for (i=0; i<[arg length]/2; i++) {
        byte_chars[0] = [arg characterAtIndex:i*2];
        byte_chars[1] = [arg characterAtIndex:i*2+1];
        whole_byte = strtol(byte_chars, NULL, 16);
        [deviceToken appendBytes:&whole_byte length:1];
    }
    
    [[PushIOManager sharedInstance] didRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
}

-(void)recordNotification:(id)arg
{
	id userInfo = [arg objectAtIndex:0];
	ENSURE_DICT(userInfo);
	
	NSLog(@"[DEBUG] PushIO recived notification");
    
	[[PushIOManager sharedInstance] didReceiveRemoteNotification:userInfo];
}

-(void)overridePushIOAPIKey:(id)apiKey
{
    ENSURE_ARG_COUNT(apiKey, 1);
    ENSURE_TYPE([apiKey objectAtIndex:0], NSString);
    
    [[PushIOManager sharedInstance] setOverridePushIOAPIKey:[apiKey objectAtIndex:0]];
}



#pragma mark Cleanup

-(void)dealloc
{
	// release any resources that have been retained by the module
	[super dealloc];
}

#pragma mark Internal Memory Management

-(void)didReceiveMemoryWarning:(NSNotification*)notification
{
	// optionally release any resources that can be dynamically
	// reloaded once memory is available - such as caches
	[super didReceiveMemoryWarning:notification];
}

#pragma mark Push Notification Delegates

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

#pragma mark Push IO

// A unique ID used by Push IO. You can use this for adding test devices at https://manage.push.io
// This call will always return a non-null value.
- (id) pushIOUUID{
    return [[PushIOManager sharedInstance] pushIOUUID];
}


#pragma Public APIs - Category
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

#pragma Public APIs - User Identifier

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

- (id) registeredUserID{
    return [[PushIOManager sharedInstance] registeredUserID];
}

- (id) isRegisteredForUserID:(id)userID{
    ENSURE_ARG_COUNT(userID, 1);
    ENSURE_TYPE([userID objectAtIndex:0], NSString);

    return NUMBOOL([[PushIOManager sharedInstance] isRegisteredForUserID:[userID objectAtIndex:0]]);
}


#pragma Public APIs - Engagement Metric Tracking


- (void) trackEngagementCustomMetric:(NSString *)customMetric{
    ENSURE_ARG_COUNT(customMetric, 1);
    ENSURE_TYPE([customMetric objectAtIndex:0], NSString);
    
    [[PushIOManager sharedInstance] trackEngagementCustomMetric:[customMetric objectAtIndex:0]];
}



@end
