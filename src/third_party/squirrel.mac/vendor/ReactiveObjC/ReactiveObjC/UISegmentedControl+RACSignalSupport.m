//
//  UISegmentedControl+RACSignalSupport.m
//  ReactiveObjC
//
//  Created by Uri Baghin on 20/07/2013.
//  Copyright (c) 2013 GitHub, Inc. All rights reserved.
//

#import "UISegmentedControl+RACSignalSupport.h"
#if defined(BUILDING_RAC_FRAMEWORK)
#import "extobjc/EXTKeyPathCoding.h"
#else
#import <ReactiveObjC/EXTKeyPathCoding.h>
#endif
#import "UIControl+RACSignalSupportPrivate.h"

@implementation UISegmentedControl (RACSignalSupport)

- (RACChannelTerminal *)rac_newSelectedSegmentIndexChannelWithNilValue:(NSNumber *)nilValue {
	return [self rac_channelForControlEvents:UIControlEventValueChanged key:@keypath(self.selectedSegmentIndex) nilValue:nilValue];
}

@end
