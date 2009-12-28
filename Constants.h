/*
 *  Constants.h
 *  HoloSim
 *
 *  Created by Veljko Krunic on 8/7/07.
 *  Copyright 2007 Veljko Krunic. All rights reserved.
 *
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// To ensure that constants that are not used do not result in warning
#pragma GCC system_header

/**
 * Animation FPS rate
 */
static const int FRAMES_PER_SECOND = 20;

/**
 * How long is interval between frames of animation
 */
static const double FRAME_INTERVAL = 1.0/FRAMES_PER_SECOND;

/**
 * By default, we don't progress with any speed in animation
 */
static const double DEFAULT_INTERFRAME_DISTANCE = 0;

/**
 * Max duration of animation (in seconds)
 */
static const double MAX_ANIMATION_DURATION = 30;

/**
 * Min duration of animation (in seconds)
 */
static const double MIN_ANIMATION_DURATION = 0.2;

/**
 * Range of durations of animation (in seconds)
 */
static const double ANIMATION_DURATION_RANGE = MAX_ANIMATION_DURATION - MIN_ANIMATION_DURATION;

/**
 * Preference key for mouse wheel clicks to zoom ratio
 */
static const NSString *MOUSE_WHEEL_RATIO_KEY = @"mouseWheelZoomRatio";

/**
 * Min legal value for MOUSE_WHEEL_RATIO_KEY
 */
static const double MIN_MOUSE_WHEEL_RATIO = 0.1;

/**
 * Preference key for mouse rotation speed
 */
static const NSString *MOUSE_ROTATION_SPEED_KEY = @"mouseRotationSpeed";

/**
 * Min legal value for MOUSE_ROTATION_SPEED_KEY
 */
static const double MIN_MOUSE_ROTATION_SPEED = 0.1;

/**
 * Preference key for loop animation control
 */
static const NSString *LOOP_ANIMATION_KEY = @"loopAnimation";

/**
 * Preference key for animation duration control
 */
static const NSString *ANIMATION_SPEED_KEY = @"animationSpeed";

/**
 * Min value for animation duration on scale
 */
static const int MIN_ANIMATION_DURATION_ON_SCALE = 1;

/**
 * Max value for animation duration on scale
 */
static const int MAX_ANIMATION_DURATION_ON_SCALE = 100;

/**
 * Used to determine emulation of mouse wheel movement number of clicks of wheel on single button mouse that emulates multiple buttons
 */
static const int EMULATE_MOUSE_WHEEL = 2;

#endif
