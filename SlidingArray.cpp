/*
  SlidingArray.h v.01 - Library for 'duino
  https://github.com/newEndeavour/SlidingArray
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
	///*
	Serial.print("\nsize:");
	Serial.print(_size);
	Serial.print("\nsizeof(float):");
	Serial.print(sizeof(float));
	Serial.print("\nmem:");
	Serial.print(_size * sizeof(float));
	//*/
  	if (arg == NULL) size = 0;
  	
	ClearArray();					// clear the Args

	//DEBUG - Pardon the mess as we are tidying this place ...
	/*
	Serial.print("\nsize:");
	Serial.print(size);
	Serial.print("\ncount:");
	Serial.print(count);
	Serial.print("\ncmin:");
	Serial.print(cmin);
	Serial.print("\ncmax:");
	Serial.print(cmax);
	Serial.print("\navg:");
	Serial.print(average);
	delay(2000);
	*/
}


//Destructor
SlidingArray::~SlidingArray()
{
  if (arg != NULL) free(arg);
}


// Public Methods //////////////////////////////////////////////////////////////
//Clears Array contents
void SlidingArray::ClearArray(void)
{
	for (int i=0; i<size; i++) {
		arg[i] = 0.0;		
	}

	//reset pre-calc variables
	sum 	= 0.0;
	average = 0.0;
	count 	= 0;
	cmin	= NAN;
	cmax	= NAN;
}


//
//		UNFINISHED
//
//
//Sorts Array, in an Ascending order = [1,2,3,4, ... ,Count] (Operation = 0) 
// or a Descending order = [Count, ... ,4,3,2,1] (Operation = 1)
void  SlidingArray::SortArray(int operation)
{
	

}


//
//		UNFINISHED
//
//
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
		temp.PopulateArray(arg[i]);				// copy existing args
		
		//METHOD 2: direct argument approach
		//temp.arg[j] = arg[i];
		//j++; 
	}
	
	//Update stats
		
	
	
}


//Populates Array with additional data point and drops oldest entry
//Method also calculates average, min and max on the fly
void SlidingArray::PopulateArray(float lastObservation)
{
	count++;					// Increment count
	if (count>size) {				// 
		sum = 0;
		cmin = MAX_FLOAT_VALUE;			// Reset Min and Max
		cmax = MIN_FLOAT_VALUE;			
		for (int i=0; i<size-1; i++) {		// for each arguments in array minus the last
			arg[i] = arg[i+1];		// shift existing arguments one place
			sum   += arg[i];		// update average
			cmin = min(arg[i],cmin);	// update min
			cmax = max(arg[i],cmax);	// update max
		}
	}

	count	     = min(size, count);		// current count
	arg[count-1] = lastObservation;			// Last observation takes the last place in the array
	sum         += lastObservation;			// 	
	average      = sum/count;			// finalise average
	cmin 	     = min(lastObservation,cmin);	// update min	
	cmax 	     = max(lastObservation,cmax);	// update max	

	/*
	//DEBUG - Pardon the mess as we are tidying this place ...
	Serial.print("\n\ncount:");
	Serial.print(count);
	Serial.print("\nsize:");
	Serial.print(size);
	Serial.print("\nlobs:");
	Serial.print(lastObservation,6);
	Serial.print("\n");

	for (int i=0; i<count; i++) {
		Serial.print("arg[");
		Serial.print(i);
		Serial.print("]:");
		Serial.print(arg[i],6);
		Serial.print("\t");
	}
	Serial.print("\ncmin:");
	Serial.print(cmin,6);
	Serial.print("\ncmax:");
	Serial.print(cmax,6);
	Serial.print("\navg:");
	Serial.print(average,6);
	Serial.print("\n");
	delay(1000);
	*/
	
}


//Pushes a single data point at position pos (pos e [1,N])
//if pos>count, we call PopulateArray with the Observation
//if pos<=count, we simply replace the existing argument with the Observation
//Method also calculates average, min and max on the fly
int SlidingArray::PushArgument(int pos, float Observation)
{
	if (pos<1) {
		error = -4;
		return error;
	} else if (pos>count) {
		PopulateArray(Observation);
	} else {
		sum 		= sum - arg[pos-1] + Observation;
		average	 	= sum / count;

		cmax		= GetMax();	
		cmin 		= GetMin();

		arg[pos-1] 	= Observation;				//Update argument
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


//Calculates average from current Array 
float SlidingArray::GetAverage(void)
{
float sum  = 0;

	for (int i=0; i<count; i++) {
		sum  += arg[i];		
	}
	average = sum/count;
	return average;
}


//Calculates Variance from current Array and average
float SlidingArray::GetVariance(void)
{
float sum2 = 0;

	if (count<2) {
		return NAN;
	} else { 
		for (int i=0; i<count; i++) {
			sum2 += pow( arg[i] - average , 2 );		
		}
		return sum2/(count-1);
	}
}

//Calculates Std Deviation from current Variance 
float SlidingArray::GetStdDeviation(void)
{
	if (count<2) {
		return NAN;
	} else { 
		return sqrt(GetVariance());
	}
}


//Returns the calculated Min
float SlidingArray::GetMin(void)
{
float lmin  = MAX_FLOAT_VALUE;

	for (int i=0; i<count; i++) {
		lmin  = min (lmin , arg[i]);		
	}
	cmin = lmin;
	return lmin;
}


//Returns the calculated Max
float SlidingArray::GetMax(void)
{
float lmax  = MIN_FLOAT_VALUE;

	for (int i=0; i<count; i++) {
		lmax  = max (lmax , arg[i]);		
	}
	cmax = lmax;
	return lmax;
}


//Returns the latest calculated average
float SlidingArray::GetLastAverage(void)
{
	return average;
}

//Returns the latest calculated Min
float SlidingArray::GetLastMin(void)
{
	return cmin;
}


//Returns the latest calculated Max
float SlidingArray::GetLastMax(void)
{
	return cmax;
}



// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library


// /////////////////////////////////////////////////////////////////////////////
// END OF FILE