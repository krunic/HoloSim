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
