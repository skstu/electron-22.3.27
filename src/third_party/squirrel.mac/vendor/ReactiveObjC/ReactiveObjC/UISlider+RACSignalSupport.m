//
//  UISlider+RACSignalSupport.m
//  ReactiveObjC
//
//  Created by Uri Baghin on 20/07/2013.
//  Copyright (c) 2013 GitHub, Inc. All rights reserved.
//

#import "UISlider+RACSignalSupport.h"
#if defined(BUILDING_RAC_FRAMEWORK)
#import "extobjc/EXTKeyPathCoding.h"
#else
#import <ReactiveObjC/EXTKeyPathCoding.h>
#endif
#import "UIControl+RACSignalSupportPrivate.h"

@implementation UISlider (RACSignalSupport)

- (RACChannelTerminal *)rac_newValueChannelWithNilValue:(NSNumber *)nilValue {
	return [self rac_channelForControlEvents:UIControlEventValueChanged key:@keypath(self.value) nilValue:nilValue];
}

@end
