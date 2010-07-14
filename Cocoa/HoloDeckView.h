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
#import <Foundation/Foundation.h>

#import "AbstractDrawingCode.h"
#import "MouseAdapter.h"
#import "HoloSimDocument.h"

using namespace hdsim;

/**
 * Room view that is used to show 3D view of the room in the Holodeck
 */
@interface HoloDeckView : NSView
{
   /**
    * Model associated with this view. 
    */
   IBOutlet HoloSimDocument *model;
   
   /**
    * FOV slider associated with this view. 
    */
   IBOutlet id fovSlider;
    
   /**
    * FOV slider associated with x rotation in this view.                                                                                                                
    */
   IBOutlet id xSlider;

   /**
    * FOV slider associated with x rotation in this view.                                                                                                                
    */
   IBOutlet id ySlider;

   /**
    * FOV slider associated with x rotation in this view.                                                                                                                
    */
   IBOutlet id zSlider;
   
   /**
    * Slider for model timeslice
    */
   IBOutlet id timeSliceSlider;
   
   /**
    * play/pause button. 
    */
   IBOutlet id playPauseButton;
   
   /**
    * Label that is showing frames per second
    */
   IBOutlet id framesPerSecondCounterLabel;
   
   /**
    * Moxel per second counter label
    */
   IBOutlet id meanMoxelsPerSecondCounterLabel;
   
   /**
    * Minimum number of moxels per second ever achieved
    */
   IBOutlet id minMoxelsPerSecondAchievedCounterLabel;
   
   /**
    * Percentage of time spent in moxel rendering label
    */
   IBOutlet id percentageLabel;
   
   /**
    * Number of moxels in the model
    */
   IBOutlet id numMoxelsLabel;
   
   /**
    * Max number of moxels per second achieved
    */
   IBOutlet id maxMoxelsPerSecondAchievedCounterLabel;
   
   /**
    * Last frame rendering statistics
    */
   IBOutlet id lastFrameMoxelsPerSecondCounterLabel;
   
   /**
    * Tracking rectangle for mouse view
    */
   NSTrackingRectTag trackingRectangle;
   
   /**
    * Custom animation timer
    */
   NSTimer *openGLAnimationTimer;
    
   /**
    * Is context initialized
    */
   Boolean contextInitalized;
   
   /**
    * Is animation supposed to be continues
    */
   Boolean loopAnimation;
   
   /**
    * Should we progress with animation to next frame
    */
   BOOL animationRunning;
   
   /**
    * Where did mouse drag started
    */
   NSPoint mouseDragStart;
   
   /**
    * Where is current point of the drag
    */
   NSPoint mouseDragCurrent;
    
   /**
    * This is a pointer to C++ object is where all OpenGL drawing would happen for portability reasons
    */
   AbstractDrawingCode *drawer;
   
   /**
    * Controller adapter that is controlling model in this view
    */
   MouseAdapter *mouseAdapter;
   
@private
   
   /**
    * OpenGL context for the custom view
    */
   NSOpenGLContext *openGLContext; 
   
   /**
    * Pixel format when we are inited with separate pixel format
    */
   NSOpenGLPixelFormat *pixelFormat;
   
   /**
    * Minimal number of moxels per second achieved
    */
   double minMoxelsPerSecond;
   
   /**
    * Maximum number of moxels per second achieved
    */
   double maxMoxelsPerSecond;
}

/**
 * Callback to be called when fovSlider has changed
 *
 * @param sender Object that made a call
 */
- (IBAction)fovSliderChanged:(id)sender;

/**
 * Callback to be called when xSlider has changed
 *
 * @param sender Object that made a call
 */
- (IBAction)xSliderChanged:(id)sender;

/**
 * Callback to be called when ySlider has changed
 *
 * @param sender Object that made a call
 */
- (IBAction)ySliderChanged:(id)sender;

/**
 * Callback to be called when zSlider has changed
 *
 * @param sender Object that made a call
 */
- (IBAction)zSliderChanged:(id)sender;

/**
 * Callback to be invoked when start animation was requested
 *
 * @param sender Who invoked this callback
 */
-(IBAction)toggleAnimation:(id)sender;

/**
 * Callback to be invoked when model got changed
 *
 * @param sender Who invoked this callback
 */
-(IBAction)notifyModelChanged:(id)sender;

/**
 * Initialize with appropriate pixel format
 */
-(id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format; 

/**
 * Call update method of the OpenGL context class
 */
-(void)update; 

/**
 * Lock focus for the current window
 */
-(void)lockFocus;

/**
 * Pixel format getter
 */
-(NSOpenGLPixelFormat *)pixelFormat;

/**
 * Set pixel format
 */
-(void)setPixelFormat:(NSOpenGLPixelFormat*)format;

/**
 * Set OpenGL context
 */
-(void)setOpenGLContext:(NSOpenGLContext*)context; 

/**
 * Get OpenGL context
 */
-(NSOpenGLContext*)openGLContext;

@end
