//
//  PushIOManager+PushIOLocation.h
//  PushIOManager
//
//  Copyright (c) 2013 Push IO Inc. All rights reserved.
//

#import <CoreLocation/CoreLocation.h>
#import "PushIOManager.h"
#import "PushiOManager+PushIOTrackers.h"

typedef enum
{
    PUSHIO_LOCATION_ERROR_UNKNOWN = 0,
    PUSHIO_LOCATION_ERROR_USER_DISABLED_LOCATION,
    PUSHIO_LOCATION_ERROR_DEVICE_DOES_NOT_SUPPORT_LOCATION,
    PUSHIO_LOCATION_ERROR_DEVICE_CANNOT_MONITOR_REGIONS,
    PUSHIO_LOCATION_ERROR_LOCATION_TEMP_ERROR
}
PushIOLocationError;

// If you are a delegate of PushIOManager, you will also be signed up for these callbacks also.
@protocol PushIOManagerLocationDelegate <PushIOManagerDelegate>
@optional
// Returns you the current location as changes are detected that exceed the area of interest.
// A null location means location services have been disabled.
- (void) locationDidChange:(CLLocation *)newLocation;

// If there's an error enabling location (either because the user denied it or it just didn't work) this will let you know.
- (void) locationNotMonitored:(PushIOLocationError)locationError detailedCoreLcoationError:(NSError *)error;

// These let you know when the user has entered or left targeted regions so that you can do further actions.
// Note these could be called while the application is in the background.
- (void) userEnteredMonitoredRegion:(NSString *)regionID;
- (void) userLeftMonitoredRegion:(NSString *)regionID;

// Callback when the set of active regions changes in some way, usually when you are registered for more regions than
// the system supports, and the library has decided to switch to a new set of regions for the system to monitor.
- (void) activeRegionsChanged;

@end

@class PushIOLocationTracker;

// Category on PushIOManager.

@interface PushIOManager (PushIOLocation) <CLLocationManagerDelegate>



//      *********** SETUP AND MANAGEMENT OF LOCATION SERVICES FOR ENGAGEMENT TRACKING **************
// Two options are presented for location management, The first where PushIOManager maintains a CLLocationManager.
// In the second option you simply feed PushIOManager locations to record.
// In either option locations, if availaible, are set to PushIO on during any engagement tracking.
// Note that if you are providing locations directly, make sure to provide them BEFORE engagement calls.

// =========== Option (1) - pushIOManager can create and maintian a LocationManager.

// This property provides a human readable reason for the applicatino to be using location services.
// Note that underneath this is using the CLLocationManager "purpose", which is depricated as of iOS6.
// For applications targeting iOS6 and later, instead provide an entry in your info.plist with the location privacy key:
// "Privacy - Location Usage Description" and a string describing the reason for location.
// Call this before you start monitoring for location updates.
@property (nonatomic, copy) NSString *locationPurpose;

// For iOS8+, the internal locatin manager needs to request either Always or InUse location permissions.
// Your applciation must provide one of two
// keys in your info.plist giving the reason why you are requesting location access:
// NSLocationWhenInUseUsageDescription or NSLocationAlwaysUsageDescription
// If neither key is present or the wrong key is present location monitoring will not work.  Whichever key is present determines
// which permission the internal location manager asks for.

// These calls tell PushIO to start monitoring location for push notification engagement tracking.
// Note that the pushIOManager library will maintain a CoreLocation LocationManager, and manage shutting down
// or starting up this location manager when the application suspends/resume.
// You need only call "startDetectingLocation" at the point in your aplication lifecycle when you feel it approprriate
// to generate the alert asking the user for permission to monitor locations.
- (void) startUpdatingLocationForPush;


// You only need to call this if you wish to disable location services while the application is running.
- (void) stopUpdatingLocationForPush;

// By default PushIOManager will use the "significant change" location service for detecting location.
// If you set this property, the "standard location" service will be used with this value set.
// Note that pushIOManager will not register location updates unless a new location is greater in distance from
// the old one than this location accuracy setting - or 200 meters if this value is not set.
@property (nonatomic, assign) CLLocationAccuracy desiredLocationAccuracy;

// This value will keep the PushIOManaged CLLocationManager from giving you an update unless the new distance is
// greater than the distance filter.
// The default value is 200 meters, note that the CLLocation manager defult is "always update" but
// this larger update value is default to keep from registering location changes too often, thus saving
// battery life.  Only override if you really need greater accuracy for pushes or other internal needs.
@property (nonatomic, assign) CLLocationDistance distanceFilter;

// Last location found by the internal location manager,
// or set by "assignLocation" so that you can use the location found for your own needs as well.
- (CLLocation *) lastLocationFound;

// If this returns YES, PushIOManager location tracking will be maintined in the background.  If no, location
// monitoring will be disabled when the application is sent to the background.
- (BOOL) willMonitorLocationInBackground;

// =========== Option (2) - you can manage your own CoreLocation manager.

// Use this call to tell the pushIOManager when location changes.
// This same location will be passed back to PushIO for engagement tracking when set.
// The location persists for the life of the application, until changed again or the application is shut down.
// You can make a new CLLocation object from lat/long values using the CLLocation initWithLatitude:longitude: method.
// If you call this more often than every ten minutes, the revsied location will be held until twn minutes have passed, then a
// new registration issued - this is to preserve battery life of the users device.

- (void) assignLocation:(CLLocation *)location;
//***********


//      *********** REGION SUPPORT **************


// Provides a region into which the application shuld be notified if the user entered.
// If the passed in region contains a region ID, that will be used and will replace an existing region of the same ID.
// If the passed in region contains a null or empty regionID, then one will be generated.
// Returns the regionID for this monitored region.
- (NSString *)startMonitoringEntryRegion:(CLRegion *)region;

// Pass in the regionID returned by the "startMonitoringEntryRegion" call to end monitoring for that region.
- (void) stopMonitoringEntryRegion:(NSString *)regionID;

// CLears out any monitoring.
- (void) clearAllMonitoredRegions;

// This loads a set of regions stored in a GEOJson format
// http://www.geojson.org/geojson-spec.html#geojson-objects
// Each region is defined in the "features" section as:
//{ "type": "Feature",
//    "geometry": {"type": "Point", "coordinates": [LATTITUDE, LONGITUDE]},
//    "properties": {"radius": "MY_REGION_RADIUS", "identifier" : "MyUniqueRegionID" }
//}
- (void) loadRegionDataFromURL:(NSURL *)regionDataURL forLayerID:(NSString *)layerID;

// Starts loading regions managed on the PushIO system, loads regions defined by the given LayerID.
// Contact PushIO for more information on defining managed reigons.
- (void) startMonitoringRegionsForRemoteLayerID:(NSString *)layerID withPushRegistrationPublisher:(NSString *)pushRegPublisher;


// Gives you back a set of PIORegion objects that describe all of the regions the system currently knows
// to monitor.
@property (nonatomic, retain, readonly) NSArray *monitoredRegions;

// Because there may be more regions than the system itself can monitor, PushIOManager
// swaps out regions from the larger set and just has the system monitor the closest ones.
// Thus this array contains the regions the system has been told to monitor.
@property (nonatomic, readonly) NSArray *activeRegions;



//      *********** LOCATION PUBLISHER **************

// In your pushIO config.json, add in a publisher entry with your location publisher GUID:
// { "publishers" : { "locationGUID" : "XYZPDQ" } }
// Then you can make use of the following calls to add locations to that publisher.

// Returns a tracker for this location you can use to delete this exact item later.
// You may also ignore the tracker, you can obtain it in the allLocationTrackers call.
// The tracker returned will have the trackerID set in case you wish to use for later extraction.
- (PushIOLocationTracker *) addLocationTrackerWithLocation:(CLLocation *)location;

// Replaces all current locations for a publisher with the contents of this array
// The array passed in shoudl hold PushIOLocationTracker objects.
- (void) addLocationTrackers:(NSArray *)locations;

// Returns an array of location tracker objects, from which you can extract location.
- (NSArray *) allLocationTrackers;

@end

@interface PushIOLocationTracker : PushIOTracker

// The location of this tracker.
@property (nonatomic, retain) CLLocation *location;

// Used to build a tracker object.
// The returned location tracker will have the location and trackerID set automatically based on location.
+ (PushIOLocationTracker *) locationTrackerWithLocation:(CLLocation *)location;

// The returned location tracker will have the location, and trackerID set to your custom ID.
// Be sure that the customID is unique across trackers!!!
+ (PushIOLocationTracker *) locationTrackerWithLocation:(CLLocation *)location andCustomID:(NSString *)customID;

@end
