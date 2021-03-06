/*
 *  SlowInSlowOut.fs
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/29/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
 */
 
/**
 * Timeslice to use for the current frame
 */
uniform float timeSlice;

/**
 * Get position on the slow in slow out curve, based on the time
 *
 * @parm t - Time. Must be in [0, 1]
 *
 * @return Position on the curve
 */
float getPositionOnTheCurve(in float t)
{
   if (t < 0.2)
      return t/2.0;
      
   if (t < 0.9)
      return t - 0.1;
    
   return 0.8 + (t - 0.9)/3.0;
}

/**
 * Change depth buffer value based on position in slow in slow out curve
 *
 * Note that we use GL_LESS, so part that would be "compressed" by depth buffer is [gl_FragCoord.z, 1], not [0, gl_FragCoord.z]
 *
 * @param zCoord Z coordinate going in
 *
 * @return Value of the Z coordinate to use instead
 */
float rescaleZCoord(in float zCoord)
{
 	// We are scaling part [z, 1.0]
   float rescaledRodLength = (1.0 - zCoord) * getPositionOnTheCurve(timeSlice);
   
   // To account for numeric errors
   if (rescaledRodLength < 0.0)
   	rescaledRodLength = 0.0;
   
   float newZCoord = 1.0 - rescaledRodLength;

   return clamp(newZCoord, 0.0, 1.0);
}

void main()
{
	 gl_FragDepth = rescaleZCoord(gl_FragCoord.z);
}
