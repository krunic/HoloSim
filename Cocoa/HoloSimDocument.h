//
//  MyDocument.h
//  HoloSim
//
//  Created by Veljko Krunic on 4/8/07.
//  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
//

/* HoloSimDocument */

#import <Cocoa/Cocoa.h>

#import "GPUInterpolatedModel.h"

using namespace hdsim;

/**
 * Document containining model that is used in the Holodeck
 */
@interface HoloSimDocument : NSDocument
{
   /**
    * Rod room window associated with this document
    */
   IBOutlet id rodRoomWindow;

   /**
    * Slider for control of timeslice
    */
   IBOutlet id timeSliceSlider;
   
   /**
    * Distance between two frames of animation in seconds
    */
   double interframeDistance;
   
   /**
    * Speed with which rotation chages
    */
   double mouseRotationSpeed;
   
   /**
    * Mouse zoom speed
    */
   double mouseZoomSpeed;
   
   /**
    * Should we loop animation
    */
   BOOL loopAnimation;
   
   /**
    * This is a pointer to C++ object is where all loading of the code actually happens
    */
   GPUInterpolatedModel *model;
}

/**
 * Get associated C++ model
 *
 * @return Associated model
 */
-(AbstractModel *)model;

/**
 * Callback that should be invoked when the model timeslice changes
 *
 * @param sender Caller of this callback
 */
- (IBAction)timeSliceSliderChanged:(id)sender;

/**
 * Callback that should be invoked when the mouse zoom speed changes
 *
 * @param sender Caller of this callback
 */
- (IBAction)mouseZoomSpeedChanged:(id)sender;

/**
 * Callback that should be invoked when the mouse rotation speed changes
 *
 * @param sender Caller of this callback
 */
- (IBAction)mouseRotationSpeedChanged:(id)sender;

/**
 * Callback that should be invoked when the loop animation changes
 *
 * @param sender Caller of this callback
 */
- (IBAction)loopAnimationChanged:(id)sender;

/**
 * Change animation duration
 *
 * @param duration new duration
 */
- (void)setInterframeDistance:(double)duration;

/**
 * Get distance between two frames
 *
 * @return Distance between two frames
 */
- (double)interframeDistance;

/**
 * Change mouse zoom speed
 *
 * @param speed Speed with which this changes
 */
- (void)setMouseZoomSpeed:(double)speed;

/**
 * Get mouse zoom speed
 *
 * @return Distance between two frames
 */
- (double)mouseZoomSpeed;

/**
 * Change mouse rotation speed
 *
 * @param speed Speed with which this changes
 */
- (void)setMouseRotationSpeed:(double)speed;

/**
 * Get mouse rotation speed
 *
 * @return Distance between two frames
 */
- (double)mouseRotationSpeed;

/**
 * Change animation duration
 *
 * @param loop should we loop
 */
- (void)setLoopAnimation:(BOOL)loop;

/**
 * Get mouse rotation speed
 *
 * @return Distance between two frames
 */
- (BOOL)loopAnimation;

@end
