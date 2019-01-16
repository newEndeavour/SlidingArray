/*
  SlidingArray.h v.01 - Library for 'duino
  https://github.com/JeromeDrouin/SlidingArray
  http://playground.arduino.cc/Main/SlidingArray

  Copyright (c) 2018 Jerome Drouin  All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


// ensure this library description is only included once
#ifndef SlidingArray_h
#define SlidingArray_h

#include "Arduino.h"

// defines
#define MAX_ARRAY_SIZE 		10000
#define MAX_FLOAT_VALUE 	0xFFFFFFFF
#define MIN_FLOAT_VALUE 	-3.4028235E+38


// library interface description
class SlidingArray
{
  // user-accessible "public" interface
  public:
  // methods
	SlidingArray(void);
	explicit SlidingArray(int _arraySize);
	~SlidingArray();

	void  ClearArray();
	void  PopulateArray(float lastObservation);
	void  SortArray(int operation);
	void  TrimArray(int low_pos, int high_pos);

	int   PushArgument(int pos, float Observation);
	float PullArgument(int pos);

	int   GetSize(void);
	int   GetCount(void);
	bool  IsFull(void);
	bool  HasArgs(void);

	float GetAverage(void);
	float GetLastAverage(void);
	float GetVariance(void);
	float GetStdDeviation(void);
	float GetMin(void);
	float GetMax(void);
	float GetLastMin(void);
	float GetLastMax(void);

  // library-accessible "private" interface
  //private:

  protected:
  // variables
	int 	 error;
	int      size;		// Size of Array
	int      count;		// current Size of Array
	float    sum;		// current sum or args for average. Calculated automatically after each Populate
	float    average;	// current average. Calculated automatically after each Populate
	float    cmin;		// current minimum. Calculated automatically after each Populate
	float    cmax;		// current maximum. Calculated automatically after each Populate
	float *	 arg;		// Array values
	
  // methods

};

#endif
// END OF FILE