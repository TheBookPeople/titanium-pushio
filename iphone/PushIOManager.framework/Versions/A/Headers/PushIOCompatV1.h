//
//  PushIOCompatV1.h
//  PushIOManager
//
//  Copyright (c) 2013 Push IO Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
//#include <UIKit/UIKit.h>
#include <ifaddrs.h>
#include <net/if_dl.h>
#include <sys/socket.h>

#import "PushIOManager.h"

#ifdef DEBUG
#   define DNSLog(fmt, ...) NSLog((@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#   define DNSLog(...)
#endif

// NOTE ABOUT DEPRECATED ITEMS
// Calls to list tags from the server have been removed. 
// @property (readonly, nonatomic) NSMutableArray *notificationTypesArray;

@interface PushIOManager (PushIOCompatV1) 

@property (readonly, nonatomic) NSError *error;
@property (readonly, nonatomic) NSString *lastParameters;

// Register with Push IO with Channels (Channels are Strings)
- (void)registerChannelsWithPushIO:(NSArray *)channels;

// Register with Push IO with Channels and username
- (void)registerChannelsWithPushIO:(NSArray *)channels username:(NSString *)username;

// Unregister with Push IO with Channels (Channels are Strings)
- (void)unregisterChannelsWithPushIO:(NSArray *)channels;

// Unregister from all Push IO Channels the device is currently registered to for this app
- (void)unregisterAllChannelsWithPushIO;

// Notification Types available for your application as defined from your Push IO admin portal
- (void)requestAvailableNotificationTypes;

// Notification Types that this device has registered for
- (void)requestNotificationTypesForThisDevice;

- (void)trackEngagement:(NSDictionary *)optionDict;
// Track an engagement.
// Call this during application:didFinishLaunchingWithOptions: with the launchOptions dictionary supplied to that method.
// Example:
/*
 - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
 // Track engagement
 PushIOManager *pushIOManager = [[PushIOManager alloc] init];
 [pushIOManager trackEngagement:launchOptions];
 [pushIOManager release];
 ...
 }
 */

@end
