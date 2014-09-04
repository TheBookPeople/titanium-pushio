/**
 * push-io
 *
 * Created by Your Name
 * Copyright (c) 2014 Your Company. All rights reserved.
 */

#import "TiModule.h"
#import <PushIOManager/PushIOManager.h>

@interface UkCoTbpPushioModule : TiModule <PushIOManagerDelegate>{
@private
	TiNetworkConnectionState state;
	KrollCallback *pushNotificationCallback;
	KrollCallback *pushNotificationError;
	KrollCallback *pushNotificationSuccess;
}

@end
