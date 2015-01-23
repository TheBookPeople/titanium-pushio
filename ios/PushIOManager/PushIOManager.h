//
//  PushIOManager.h
//  PushIOManager
//
//  Copyright (c) 2009-2013 Push IO Inc. All rights reserved.
//

// This version of the PushIOManager library is 2.10.9

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//! Project version number for PushIOManager.
FOUNDATION_EXPORT double PushIOManagerVersionNumber;

//! Project version string for PushIOManager.
FOUNDATION_EXPORT const unsigned char PushIOManagerVersionString[];


// IMPORTANT: To use the Push IO service, you are required to have a PushIO API key. You can
// obtain this by setting up an account and adding an app at https://manage.push.io
//
// After you add an app, set up your iOS platform via the "Set Up" section. You will then see a link
// to download a "pushio_config.json" file, which contains your API Key.  
//
// Place this file in your project alongside the AppDelegate (so that it is included in your application bundle).

// If you subscribe to these notification you'll be notified when a push is processed by this library, or the system has registered.
// You can also get the same information by setting a delegate for PushIOManager, described below.

extern NSString * const kPushProcessedNotification; // includes dictionary of lastPushMeta in userInfo
extern NSString * const kPushRegistrationSuccess;
extern NSString * const kPushRegistrationFailure; // Includes dictionary with error and statusCode entries.


@protocol PushIOManagerDelegate <NSObject>
@optional

// PushIO manager has a valid token and is ready to send a registration to the PushIO servers.
- (void)readyForRegistration;

// Status of registration calls.  Registration is called after a token is received, or when categories/trackers change.
- (void)registrationSucceeded;
- (void)registrationFailedWithError:(NSError *)error statusCode:(int)statusCode;

// In processing a push we found that new Newsstand content is ready.
- (void)newNewsstandContentAvailable;

// PushIOManager has extracted a push dictionary, the metadata is ready to be retrieved.
- (void)pushProcessed;
@end


typedef enum  {
    PUSHIO_DEBUG_NONE = 0,
    PUSHIO_DEBUG_ERRORS_ONLY = 1,
    PUSHIO_DEBUG_ACTIVITY = 2,
    PUSHIO_DEBUG_VERBOSE = 3
} PushIODebugLevel;

typedef enum  {
    PUSHIO_ENGAGEMENT_METRIC_LAUNCH = 0,  // Push IO internal use
    PUSHIO_ENGAGEMENT_METRIC_ACTIVE_SESSION = 1, // Push IO internal use
    PUSHIO_ENGAGEMENT_METRIC_INAPP_PURCHASE = 2,
    PUSHIO_ENGAGEMENT_METRIC_PREMIUM_CONTENT = 3,
    PUSHIO_ENGAGEMENT_METRIC_SOCIAL = 4,
    PUSHIO_ENGAGEMENT_METRIC_ACTION = 5, // Push IO internal use
    PUSHIO_ENGAGEMENT_METRIC_OTHER = 6, // Push IO internal use
} PushIOEngagementMetrics;

// Because 64-bit suppport added to iOS requires a iOS6.0 deployment target or higher, this version of the framework
// also requires a deployment target of 6.0 or higher.
// If you need iOS5 support, a non 64-bit version of the framework is availaible in the PushIOManager-IOS repository,
// under the directory "iOS5CompatibleFramework".
#if !defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) || defined(IOS_50_COMPAT)
#else
#if __IPHONE_OS_VERSION_MIN_REQUIRED < 60000
#error The 64-bit compabtible version of the PushIOManager framework requires a deployment target of 6.0 or higher.
#error Use the "iOS5CompatibleFramework" PushIOManager.framework from the public repository for a 5.0 compatible version.
#endif
#endif

@interface PushIOManager : NSObject

// Set a delegate to get messages about push status, or to make calls that need to call back.
@property (nonatomic, assign) id <PushIOManagerDelegate> delegate;


// See the PushIODebugLevel enum for valid debug levels.
@property (nonatomic, assign) PushIODebugLevel debugLevel;


// By default PushIOManger reports the local time zone to the server to use for push determination.
// This allows you to set a custom timezone around which pushes can be filtered.
@property (nonatomic, retain) NSTimeZone *overrideTimeZone;

// We store away the last updates to badge values, alert text and sound files stored in the alert so you
// do not have to do your own parsing.
@property (nonatomic, strong) NSString *lastAlertText;
@property (nonatomic, strong) NSString *lastSoundFileName;
@property (nonatomic, strong) NSString *lastBadgeUpdate;

// This gives you the entire push dictionary above the APS level, along with other meta values that may have been sent in the push.
@property (nonatomic, strong) NSDictionary *lastPushMeta;

// Each push received includes a unique engagementID that replates to that specific push.  Here you can find what the last
// such ID was and make use of it if needed.
@property (nonatomic, copy, readonly) NSString *lastEngagementID;


//
// Setup
//
// Methods to be integrated into your app lifecycle to help us configure your app for Push IO
- (void) didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
- (void) didReceiveRemoteNotification:(NSDictionary *)userInfo;
- (void) didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;
- (void) didFailToRegisterForRemoteNotificationsWithError:(NSError *)error;

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 70000
// When engagement is finished, call the fetch completion handler with the indicated fetch result
// Engagement call will occur within five seconds of this call being activated.
- (void) didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionResult:(UIBackgroundFetchResult)fetchResult fetchCompletionHandler:(void (^)(UIBackgroundFetchResult result))handler;
#endif

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
// Note that PushIOManager will call the completion handler, so make sure you do any additional procesisng before this call.
// Engagement call will occur within five seconds of this call beeing activated.
- (void) handleActionWithIdentifier:(NSString *)identifier
              forRemoteNotification:(NSDictionary *)userInfo
                  completionHandler:(void (^)(void))completionHandler;
#endif

//
//
// Simple (Broadcast Push) Registration
//
// Register a device with Push IO so you can broadcast to a user, without them opting into specific categories.
- (void) registerWithPushIO;

// Delete a device from Push IO
- (void) unregisterFromPushIO;

// Tells you if application has successfully registered with PushIO to receive notifications.
- (BOOL) readyToReceivePushNotifications;

//
// Targeted (Segmentation/Category Push) Registration 
//
// Register a list of categories for this device with Push IO. Categories let you opt your user into specific groups
// such as "Birdwatchers" or "BroncosFans"
//
// Any categories not present in the array will be degregistered if already registered with the server.
// An empty array is treated as an Unregister call. 
- (void) registerCategories:(NSArray *)categories;

// Registers a single category, leaving any other categories still registered.
- (void) registerCategory:(NSString *)category;

// Unregisters a single category, leaving all other categories in place.
- (void) unregisterCategory:(NSString *)category;

// Unregisters a group of categories, leaving any categories not in the pased in array still registered.
- (void) unregisterCategories:(NSArray *)categories;

// Unregister all categories for this device from Push IO
- (void) unregisterAllCategories;

// Returns information on the categories for which this app has requested registration.
- (BOOL) isRegisteredForCategory:(NSString *)category;
- (NSArray *) allRegisteredCategories;

//UserID
- (void) registerUserID:(NSString *)userID;
- (void) unregisterUserID;
- (BOOL) isRegisteredForUserID:(NSString *)userID;
- (NSString *) registeredUserID;

//
// Engagement Metric Tracking
//
//
// You can call this from anywhere to track whether your push led to a conversion
- (void) trackEngagementMetric:(PushIOEngagementMetrics)metric;

// You can call this to track a custom metric, and whether it led to a conversion
- (void) trackEngagementCustomMetric:(NSString *)customMetric;

// You don't need to call this method directly, we use it for you.
- (BOOL) trackEngagementOptions:(NSDictionary *)optionsDict withMetric:(PushIOEngagementMetrics)metric;

// Tells you if the application was last launched (or opened from background) as the result of a notification.
- (BOOL) isCurrentSessionAnEngagement;

//
// Other Helpers
//

// Convieneince methods for asking iOS to register for push

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000

// Combines the new iOS8 calls for registering settings and asking the system to register for remote notifications.
- (void) registerForRemoteNotificationWithSettings:(UIUserNotificationSettings *)settings;

#endif
                                                                                       
// Calls either of the registration calls above depending on OS version, registering for Sound/Badge/Alert types.  It does not register for newsstand.
- (void) registerForAllRemoteNotificationTypes;


// Currently in-use Push IO api settings
- (NSString *) pushIOAPIHost;
- (NSString *) pushIOAPIKey;

// If you want to use a different API key from the one in the pushio_config.json, set this property.
@property (nonatomic, retain) NSString *overridePushIOAPIKey;

// A unique ID used by Push IO. You can use this for adding test devices at https://manage.push.io
// This call will always return a non-null value.
- (NSString *) pushIOUUID;

// Returns the version of the framework that you have installed.
- (NSString *) frameworkVersion;

//
// Singleton instance
//
+ (PushIOManager *) sharedInstance;

// Sets the delegate of the shared instance, shortcut method
+ (void) setDelegate:(id <PushIOManagerDelegate>)delegate;


@end


