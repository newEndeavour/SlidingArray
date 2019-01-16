/*
  File:         SlidingArray
  Version:      0.0.1
  Date:         19-Dec-2018
  Revision:     07-Jan-2019
  Author:       Jerome Drouin

  https://github.com/newEndeavour/SlidingArray

  Copyright (c) 2018-2019 Jerome Drouin  All rights reserved.  

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

#include <SlidingArray.h>

const int ArraySize = 10;               // Declare a Dynamic Array with Size 10 elements
SlidingArray SlidingArray(ArraySize);

float Observation;
int LoopCount = 0;

//---- setup ----------------------------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial);
    
  for (int i=0;i<100;i++) {
    Observation        = (1.0) *random(0,100)/100;    
    SlidingArray.PopulateArray(Observation);
  }

  //Statistics  
  Serial.print("\n\nInitial Array:\n");

  for (int i=0;i<ArraySize;i++) {
    Serial.print("Arg[");
    Serial.print(i);
    Serial.print("]:");
    Serial.print(SlidingArray.PullArgument(i+1),4);
    Serial.print("\n");
  }
  
  //Replace second and third observation with arbitrary data
  SlidingArray.PushArgument(2,22);  //22 takes the 2nd position
  SlidingArray.PushArgument(3,33);  //33 takes the 3rd position
  
  //Push additional argument at position 20 == Outside range 
  //and so therefore place this element at the end of range switching 
  //all previous arguments one place.
  SlidingArray.PushArgument(20,111);
    
  //Statistics  
  Serial.print("\n\nAfter Push Procedures:\n");
  for (int i=0;i<ArraySize;i++) {
    Serial.print("Arg[");
    Serial.print(i);
    Serial.print("]:");
    Serial.print(SlidingArray.PullArgument(i+1),4);
    Serial.print("\n");
  }

  Serial.print("\n\nStatistics:");
  Serial.print("\nAverage:");
  Serial.print(SlidingArray.GetAverage(),5);

  Serial.print("\nVariance:");
  Serial.print(SlidingArray.GetVariance(),5);

  Serial.print("\nStdDeviation:");
  Serial.print(SlidingArray.GetStdDeviation(),5);

  Serial.print("\nMin:");
  Serial.print(SlidingArray.GetMin(),5);

  Serial.print("\nMax:");
  Serial.print(SlidingArray.GetMax(),5);

  //
  SlidingArray.PushArgument(1,22);  //22 takes the 2nd position
  SlidingArray.PushArgument(ArraySize,111);  //22 takes the 2nd position
  Serial.print("\n\nLast Consistency Check:");
  Serial.print("\nAverage:");
  Serial.print(SlidingArray.GetLastAverage(),5);
  Serial.print("\nCalcAverage:");
  Serial.print(SlidingArray.GetAverage(),5);

  //Remove this instruction if you require Loop example
  while(1);  
}


//---- loop ----------------------------------------------------
void loop() {

  LoopCount++;
  
  Observation        = random(0,100);    

  //Running Average
  SlidingArray.PopulateArray(Observation);

  //Statistics  
  Serial.print("\n\n");
  Serial.print(LoopCount);
  Serial.print(")");

  Serial.print("\tAvg:");
  Serial.print(SlidingArray.GetLastAverage(),5);

  Serial.print("\tMin:");
  Serial.print(SlidingArray.GetLastMin(),5);

  Serial.print("\tMax:");
  Serial.print(SlidingArray.GetLastMax(),5);

  delay(500);
  
}
