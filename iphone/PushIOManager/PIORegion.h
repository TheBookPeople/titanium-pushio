//
//  PIORegion.h
//  PushIOManager
//
//  Created by Kendall Helmstetter Gelner on 7/24/13.
//  Copyright (c) 2013 Push IO LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

@interface PIORegion : NSObject

// Holds radius, lat/long, and identifier
@property (nonatomic, strong) CLRegion *systemRegion;

// Repeat location for quicker sorting.
@property (nonatomic, strong) CLLocation *centerLocation;

// Repeat ID for quick access and easier sorting.
@property (nonatomic, strong) NSString *identifier;

// The system will maintain a count of times a region has been entered.
@property (nonatomic, assign) NSInteger regionEntryCount;

// Optional - present only if regions are loaded from remote layers (defined on PushIO servers)
@property (nonatomic, strong) NSString *layerID;

@end
