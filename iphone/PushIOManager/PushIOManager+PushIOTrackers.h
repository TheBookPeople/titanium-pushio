//
//  PushIOManager+PushIOTrackers.h
//  PushIOManager
//
//  Copyright (c) 2013 Push IO Inc. All rights reserved.
//

#import "PushIOManager.h"

typedef enum PushIOTrackerTypes {
    PUSHIO_TRACKER_UNKNOWN = 0,
    PUSHIO_TRACKER_CUSTOM = 1,    // If you have a custom type registered with PushIO, use the PushIOCustomTracker class.
    PUSHIO_TRACKER_LOCATION = 2,  // Standard PushIO location tracking.
    } PushIOTrackerTypes;

// Defined at the bottom of this file.
@class PushIOTracker;

@interface PushIOManager (PushIOTrackers)

// Returns all trackers currently stored.
- (NSArray *) allTrackers;

// Returns only trackers of a specific type.
- (NSArray *) trackersForTrackerType:(PushIOTrackerTypes)type;

// Returns only trackers of a specific custom publisher name.
- (NSArray *) trackersForPublisherName:(NSString *)publisherName;

// Can't have more than one tracker per tracker ID
- (PushIOTracker *) trackersWithTrackerID:(NSString *)trackerID;

// Anything that has these as interests, either complete match or this as a subset
- (NSArray *) trackersHavingInterests:(NSArray *)trackerInterests;

// Add a one or more new trackers, will be associated with the publisher matching the tracker type.
- (void) addTracker:(PushIOTracker *)tracker;
- (void) addTrackers:(NSArray *)trackers;

// Remove an existing tracker from the system.
- (void) removeTrackerWithTrackerID:(NSString *)trackerID;

// Removes all trackers that have this set of interests (order does not matter).
- (void) removeTrackersWithInterests:(NSArray *)interests;
- (void) removeTrackersWithInterests:(NSArray *)interests values:(NSArray *)values;


// Clear out all trackers across all publishers
- (void) removeAllTrackers;

// Removes only trackers with the given type
- (void) removeAllTrackersWithType:(PushIOTrackerTypes) type;

// Removes all the trackers passed in
- (void) removeTrackers:(NSArray *)trackers;

// Returns an array of PushIOCustomTrackerPublishers
- (NSArray *) allPublishers;

@end

@interface PushIOTracker : NSObject <NSCoding>

// The type of tracker object this is.
@property (nonatomic, assign) PushIOTrackerTypes trackerType;

// The publisher this tracker belongs to if custom type
@property (nonatomic, copy, readonly) NSString *customPublisherName;

// The tag for this tracker.
@property (nonatomic, copy, readonly) NSString *trackerID;

// All of the interests attached to the tracker.
@property (nonatomic, readonly, strong) NSArray *trackerInterests;

// All of the associated values attached to the tracker.
@property (nonatomic, readonly, strong) NSArray *trackerValues;

@end

