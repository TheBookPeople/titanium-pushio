//
//  PushIOCustomTrackerPublisher.h
//  PushIOManager
//
//  Copyright (c) 2013 Push IO Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "PushIOTracker+PushIOCustomTracker.h"

@interface PushIOCustomTrackerPublisher : NSObject

@property (nonatomic, copy, readonly) NSString *publisherName;
@property (nonatomic, copy, readonly) NSString *publisherID;

+ (PushIOCustomTrackerPublisher *) customTrackerPublisherWithPublisherName:(NSString *)publisherName;


- (PushIOCustomTracker *) customTrackerWithInterests:(NSArray *)interests values:(NSArray *)trackerValues;
- (PushIOCustomTracker *) customTrackerWithInterests:(NSArray *)interests value:(NSString *)trackerValue;
- (PushIOCustomTracker *) customTrackerWithInterest:(NSString *)interest values:(NSArray *)trackerValues;
- (PushIOCustomTracker *) customTrackerWithInterest:(NSString *)interest value:(NSString *)trackerValue;


@end
