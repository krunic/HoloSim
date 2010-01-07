//
//  AppController.h
//  HoloSim
//
//  Created by Veljko Krunic on 8/6/07.
//  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
//

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

@end
