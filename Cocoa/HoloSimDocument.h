/*
 * HoloSim, visualization and control of the moxel based environment.
 *
 * Copyright (C) 2010 Veljko Krunic
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#import <Cocoa/Cocoa.h>

#import "AppController.h"
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
    * App controller
    */
   IBOutlet id appController;
   
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
    * Should we optimize drawing
    */
   BOOL optimizeDrawing;
   
   /**
    * Threshold for optimiing drawing
    */
   int optimizeDrawingThreshold;
   
   /**
    * Log performance
    */
   BOOL logPerformance;
   
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
 * Get associated app controller
 * 
 * @return Application controller
 */
- (AppController *)appController;

/**
 * Set associated app controller
 *
 * @param controller Associated app controller
 */
- (void)setAppController:(AppController *)controller;

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

/**
 * Change are we optimizing drawing
 *
 * @param optimize Are we optimizing drawing
 */
- (void)setOptimizeDrawing:(BOOL)optimize;

/**
 * Get are we optimizing drawing
 *
 * @return Are we optimizing drawing
 */
- (BOOL)optimizeDrawing;

/**
 * Change optimizing drawing threshold
 *
 * @param threshold New threshold
 */
- (void)setOptimizeDrawingThreshold:(int)threshold;

/**
 * Get optimize drawing threshold
 *
 * @return Threshold
 */
- (int)optimizeDrawingThreshold;

/**
 * Change are we logging performance
 *
 * @param log Are we loging performance
 */
- (void)setLogPerformance:(BOOL)log;

/**
 * Get are we logging performance
 *
 * @return Are we logging performance
 */
- (BOOL)logPerformance;

@end
