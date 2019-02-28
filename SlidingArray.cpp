/*
  File:         SlidingArray.cpp
  Version:      0.0.4
  Date:         05-Jan-2019
  Revision:     28-Feb-2019
  Author:       Jerome Drouin (jerome.p.drouin@gmail.com)

  Editions:	Please go to SlidingArray.h for Edition Notes.

  SlidingArray.h - Library for 'duino
  https://github.com/newEndeavour/SlidingArray

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


#include "Arduino.h"
#include "SlidingArray.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
SlidingArray::SlidingArray(int _size)
{
	// initialize this instance's variables
	error = 1;
	if (_size<=0) error =-1;			// incorrect _size variables
	if (_size>MAX_ARRAY_SIZE) error =-2;		// incorrect _size variables

	//Set initial values	
	size			= _size;		//
	arg		        = (float*) malloc(_size * sizeof(float));
  	if (arg == NULL) size = 0;
  	
	isMom			= 0;			//Empty: moments aren't available...

	ClearArray();					// clear the Args

}


//Destructor
SlidingArray::~SlidingArray()
{
  if (arg != NULL) free(arg);
}


// Public Methods //////////////////////////////////////////////////////////////
// Returns the Version
String SlidingArray::GetVersion()
{
	return VER_SlidingArray;
}


//Returns the Last Version Date
String SlidingArray::GetReleaseDate()
{
	return REL_SlidingArray;
}


//Clears Array contents
void SlidingArray::ClearArray(void)
{
	for (int i=0; i<size; i++) {
		arg[i] = 0.0;		
	}

	//reset pre-calc variables
	sum_xi 		= 0.0;
	sum_xi2 	= 0.0;
	average 	= 0.0;
	variance 	= 0.0;
	stddeviation 	= 0.0;
	count 		= 0;
	cmin		= NAN;
	cmax		= NAN;
	imin		= NAN;
	imax		= NAN;
	isMom		= 0;				//Empty: moments aren't available...
}



//Populates Array with additional data point and drops oldest entry
void SlidingArray::AddArgument_NoMom(float last)
{
	count++;					// Increment count
	if (count>size) {				// 
		for (int i=0; i<size-1; i++) {		// for each arguments in array minus the last
			arg[i] 	 = arg[i+1];		// shift existing arguments one place
		}
		count = size;				// count = min (count, size) as here, count > size
	}
	arg[count-1] = last;			// Last obs takes the last place in the array

	//Moment flag = 0 -> desactivates GetMoment() methods
	isMom		= 0;
	
}


//Populates Array with additional data point and drops oldest entry
//Method also calculates (on the fly):
// - average, 
// - variance, 
// - stddeviation, 
// - min and max 
void SlidingArray::AddArgument(float last)
{
	count++;					// Increment count
	if (count>size) {				// 
		sum_xi 	= 0;
		sum_xi2 = 0;
		cmin 	= MAX_FLOAT_VALUE;			// Reset Min and Max
		cmax 	= MIN_FLOAT_VALUE;			
		for (int i=0; i<size-1; i++) {		// for each arguments in array minus the last
			arg[i] 	 = arg[i+1];		// shift existing arguments one place
			sum_xi  += arg[i];		// update average
			sum_xi2 += arg[i]*arg[i];	// update variance
			if (arg[i]<cmin) cmin = arg[i];	// update min
			if (arg[i]>cmax) cmax = arg[i];	// update max
		}
		count = size;				// count = min (count, size) as here, count > size
	}

	arg[count-1] = last;				// Last obs takes the last place in the array
	sum_xi      += last;				// xi	
	sum_xi2     += (last*last);			// xi^2			

	average      = sum_xi/count;			// finalise average, variance, stddeviation
	variance     = 0;
	stddeviation = 0;
	
	if (count>1) {
		variance     = ((float)sum_xi2/(float)count - ((float)sum_xi*sum_xi/(float)(count*count))) * count/(count-1);
		stddeviation = sqrt(variance);
	}

	if (count>1) {					// Not the first obs
		if (last<cmin) {			// update cmin and imin
			imin = count-1;		
			cmin = last;
		}
	
		if (last>cmax) {			// update cmax and imax	
			imax = count-1;			
			cmax = last;
		}
	} else { 					// first obs
			imin = count-1;		
			cmin = last;
			imax = count-1;			
			cmax = last;
	}

	//Moment flag = 0 AND not the first Add -> Recalc All moments
	if (!isMom && count>1) {
		RecalcAllMoments();
	}
		
}


//Pushes a single data point at position pos (pos e [1,N])
//if pos>count, we call Add_ with the obs
//if pos<=count, we simply replace the existing argument with the obs
//Method also calculates average, min and max on the fly
int SlidingArray::PushArgument(int pos, float obs)
{
	if (pos<1) {
		error = -4;
		return error;
	} else if (pos>count) {
		AddArgument(obs);
	} else {
		sum_xi 		= sum_xi - arg[pos-1] + obs;
		sum_xi2 	= sum_xi2 - (arg[pos-1]*arg[pos-1]) + (obs*obs);

		average	 	= sum_xi / count;
		variance     	= 0;
		stddeviation 	= 0;
		if (count>1) {
			variance     	= ((float)sum_xi2/(float)count - ((float)sum_xi*sum_xi/(float)(count*count))) * count/(count-1);
			stddeviation	= sqrt(variance);
		}

		cmax		= GetMax();	
		cmin 		= GetMin();

		arg[pos-1] 	= obs;				//Update argument
	}	
}

//Return the valueof argument at position pos (if valid pos e [1,N])
float SlidingArray::PullArgument(int pos)
{

	if ((pos>count) || (pos<1)) {
		error = -3;
		return error; 
	} else {
		return 	arg[pos-1];
	}	
}


//*******************************************************************************
//		UNFINISHED
//
//*******************************************************************************
//Sorts Array, in an Ascending order = [1,2,3,4, ... ,Count] (Operation = 0) 
// or a Descending order = [Count, ... ,4,3,2,1] (Operation = 1)
void  SlidingArray::SortArray(int operation)
{
	

}


//*******************************************************************************
//		UNFINISHED
//
//*******************************************************************************
//Trim Array with low_pos args, and high_pos args leaving only middle section
void  SlidingArray::TrimArray(int low_pos, int high_pos)
{
int newcount = count - low_pos - high_pos;
SlidingArray temp(newcount);
//int j = 0;

	//check low_pos and high_pos
	
	//Populate temp array
	for (int i=low_pos; i<count-high_pos; i++) {		// for each arguments in array
		//METHOD 1: object method approach
		temp.AddArgument(arg[i]);				// copy existing args
		
		//METHOD 2: direct argument approach
		//temp.arg[j] = arg[i];
		//j++; 
	}
	
	//Update stats
		
}


//Calculates average from current Array 
float SlidingArray::RecalcAverage(void)
{
	sum_xi  = 0;
	for (int i=0; i<count; i++) {
		sum_xi  += arg[i];		
	}
	average = sum_xi/count;
	return average;
}


//Calculates Variance from current Array and average
float SlidingArray::RecalcVariance(void)
{
	sum_xi2 = 0;
	if (count<2) {
		return NAN;
	} else { 
		for (int i=0; i<count; i++) {
			sum_xi2 += pow( arg[i] - average , 2 );		
		}
		variance = sum_xi2/(count-1);
		return variance;
	}
}


//Calculates Std Deviation from current Variance 
float SlidingArray::RecalcStdDeviation(void)
{
	if (count<2) {
		return NAN;
	} else { 
		stddeviation = sqrt(RecalcVariance());
		return stddeviation;
	}
}


//Returns the calculated Min
float SlidingArray::RecalcMin(void)
{
float lmin  = MAX_FLOAT_VALUE;

	for (int i=0; i<count; i++) {
		lmin  = min (lmin , arg[i]);
		if (arg[i]<lmin) imin = i;		
	}
	cmin = lmin;
	return lmin;
}


//Returns the calculated Max
float SlidingArray::RecalcMax(void)
{
float lmax  = MIN_FLOAT_VALUE;

	for (int i=0; i<count; i++) {
		lmax  = max (lmax , arg[i]);		
		if (arg[i]>lmax) imax = i;		
	}
	cmax = lmax;
	return lmax;
}


//Returns the latest calculated average
float SlidingArray::GetAverage(void)
{
	if (isMom)
		return average;
	else 
		return -1;
}


//Returns Last Variance
float SlidingArray::GetVariance(void)
{
	if (isMom)
		return variance;
	else 
		return -1;
}


//Returns Last StdDeviation
float SlidingArray::GetStdDeviation(void)
{
	if (isMom)
		return stddeviation;
	else 
		return -1;
}


//Returns the latest calculated Min
float SlidingArray::GetMin(void)
{
	if (isMom)
		return cmin;
	else 
		return -1;
}


//Returns the latest calculated Max
float SlidingArray::GetMax(void)
{
	if (isMom)
		return cmax;
	else 
		return -1;
}


//Returns the calculated Min ID
int SlidingArray::GetMinID(void)
{
	if (isMom)
		return imin;
	else 
		return -1;
}


//Returns the calculated Max ID
int SlidingArray::GetMaxID(void)
{
	if (isMom)
		return imax;
	else 
		return -1;
}


//Returns the latest calculated sum_xi
float SlidingArray::GetSum_xi(void)
{
	if (isMom)
		return sum_xi;
	else 
		return -1;
}


//Returns the latest calculated sum_xi2
float SlidingArray::GetSum_xi2(void)
{
	if (isMom)
		return sum_xi2;
	else 
		return -1;
}


//Returns Size
int SlidingArray::GetSize(void)
{
	return size;
}


//Returns Count
int SlidingArray::GetCount(void)
{
	return count;
}


//Returns if Array is Full (Size==Count)
bool SlidingArray::IsFull(void)
{
	return size==count;
}


//Returns if Array Has Arguments (Count!=0)
bool SlidingArray::HasArgs(void)
{
	return count>0;
}





// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

//Recalculates All Moments and updates isMoM to =1
void SlidingArray::RecalcAllMoments(void) 
{
	cmin  = MAX_FLOAT_VALUE;
	cmax  = MIN_FLOAT_VALUE;

	sum_xi 	= 0;
	sum_xi2 = 0;

	for (int i=0; i<count; i++) {
		sum_xi  += arg[i];				// Average
		sum_xi2 += pow( arg[i] - average , 2 );		// Variance
		cmin  = min (cmin , arg[i]);			// Min
		if (arg[i]<cmin) imin = i;			// imin
		cmax  = max (cmax , arg[i]);			// Max	
		if (arg[i]>cmax) imax = i;			// imax
	}

	//update moments & statistics
	average	 	= sum_xi / count;
	variance 	= sum_xi2/(count-1);
	stddeviation 	= sqrt(RecalcVariance());

	//Moment flag = 1
	isMom 		= 1;
	
}



// /////////////////////////////////////////////////////////////////////////////
// END OF FILE