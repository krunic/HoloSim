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

/**
 * Application controller. At the moment, it is controlling preference files
 */
@interface AppController : NSObject {
   
   /**
    * Should we loop animation
    */
   BOOL loopAnimation;
   
   /**
    * Distance between two frames
    */
   double interframeDistance;
   
   /**
    * Ratio between mouse movement and rotation of model on screen
    */
   double rotationRatio;
   
   /**
    * Ratio between mouse wheel movement and zoom on the screen
    */
   double zoomRatio;
   
   /**
    * Should we optimize drawing
    */
   BOOL optimizeDrawing;
   
   /**
    * At what number of moxels in model should we start drawing optimizations
    */
   int optimizeDrawingThreshold;
   
   /**
    * Should we log performance to the output file?
    */
   BOOL logPerformance;
   
   /**
    * Preference pane controller
    */
	IBOutlet NSWindowController *preferenceController;
}

/**
 * Show preference panel.
 *
 * @param sender Sender of the message
 */
- (IBAction)showPreferencePanel:(id)sender;

/**
 * Invoked when animation duration is changed
 *
 * @param sender Sender of the message
 */
- (IBAction)animationDurationChanged:(id)sender;

/**
 * Invoked when zoom rotation ratio is changed
 *
 * @param sender Sender of the message
 */
- (IBAction)mouseZoomRatioChanged:(id)sender;

/**
 * Invoked when rotation ratio is changed
 *
 * @param sender Sender of the message
 */
- (IBAction)mouseRotationRatioChanged:(id)sender;

/**
 * Invoked when loop animation setting is changed
 *
 * @param sender Sender of the message
 */
- (IBAction)loopAnimationChanged:(id)sender;

/**
 * Invoked when optimization of the drawing was changed
 *
 * @param sender Sender of the message
 */
- (IBAction)optimizeDrawingChanged:(id)sender;

/**
 * Invoked when optimization of the drawing theshold was changed
 *
 * @param sender Sender of the message
 */
- (IBAction)optimizeDrawingThresholdChanged:(id)sender;

/**
 * Invoked when log performance of rendering changed
 *
 * @param sender Sender of the message
 */
- (IBAction)logPerformanceChanged:(id)sender;

@end
