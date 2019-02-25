/*
  File:         SlidingArray.h
  Version:      0.0.3
  Date:         05-Jan-2019
  Revision:     25-Feb-2019
  Author:       Jerome Drouin (jerome.p.drouin@gmail.com)

  SlidingArray.h - Library for 'duino
  https://github.com/JeromeDrouin/SlidingArray

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

  Editions:
  - 0.0.1	: First version
  - 0.0.2	: Addition of imin, imax members variables MinID and MaxID member methods
  - 0.0.3	: Addition of variance, stddeviation

*/


// ensure this library description is only included once
#ifndef SlidingArray_h
#define SlidingArray_h

#include "Arduino.h"

// DEFINES /////////////////////////////////////////////////////////////
#define VER_SlidingArray	"0.0.3"		//
#define REL_SlidingArray	"25Feb2019"	//

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

	float GetSum_xi(void);
	float GetSum_xi2(void);

	float RecalcAverage(void);
	float RecalcVariance(void);
	float RecalcStdDeviation(void);
	float RecalcMin(void);
	float RecalcMax(void);

	float GetAverage(void);
	float GetVariance(void);
	float GetStdDeviation(void);
	float GetMin(void);
	float GetMax(void);

	int   GetMaxID(void);
	int   GetMinID(void);

	String GetVersion();
	String GetReleaseDate();

  // library-accessible "private" interface
  //private:

  protected:
  // variables
	int 	 error;
	int      size;		// Size of Array
	int      count;		// current Size of Array
	float    sum_xi;	// current sum or args for average. Calculated automatically after each Populate
	float    sum_xi2;	// current sum or args^2 for variance. Calculated automatically after each Populate
	float    average;	// current average. Calculated automatically after each Populate
	float    variance;	// current variance. Calculated automatically after each Populate
	float    stddeviation;	// current stddeviation. Calculated automatically after each Populate
	float    cmin;		// current minimum. Calculated automatically after each Populate
	float    cmax;		// current maximum. Calculated automatically after each Populate
	int 	 imax;		// The array position of Max
	int 	 imin;		// The array position of Min
	float *	 arg;		// Array values
	
  // methods

};

#endif
// END OF FILE