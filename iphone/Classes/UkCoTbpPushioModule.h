/**
 * Titatanium PushIO Module
 *
 * Created by The Book People Ltd
 * Copyright (c) 2014 Your Company. All rights reserved.
 *
 * This file is part of Titatanium PushIO Module.
 * Foobar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with Titatanium PushIO Module.  If not, see <http://www.gnu.org/licenses/>.
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
