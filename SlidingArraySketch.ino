/*
  File:         SlidingArray1
  Version:      0.0.4
  Date:         19-Dec-2018
  Revision:     28-Feb-2019
  Author:       Jerome Drouin

  https://github.com/newEndeavour/SlidingArray1

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
SlidingArray SlidingArray1(ArraySize);
SlidingArray ControlArray2(ArraySize);
SlidingArray ControlArray3(ArraySize);
SlidingArray ControlArray4(ArraySize);
SlidingArray ControlArray5(ArraySize);

float Observation;
int LoopCount = 0;

//---- setup ----------------------------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial);
    
  for (int i=0;i<10;i++) {
    //Observation        = (1.0) * random(0,100)/100;    
    Observation        = i;    
    SlidingArray1.AddArgument(Observation);
    ControlArray2.AddArgument(SlidingArray1.GetSum_xi());
    ControlArray3.AddArgument(SlidingArray1.GetSum_xi2());
    ControlArray4.AddArgument(SlidingArray1.GetVariance());
    ControlArray5.AddArgument(SlidingArray1.GetStdDeviation());
  }

  //Statistics  
  Serial.print("\n\nInitial Array:\n");

  for (int i=0;i<ArraySize;i++) {
    Serial.print("Arg[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.print(SlidingArray1.PullArgument(i+1),5);
    Serial.print("\t\tSum_xi : ");
    Serial.print(ControlArray2.PullArgument(i+1),5);
    Serial.print("\t\tSum_xi2: ");
    Serial.print(ControlArray3.PullArgument(i+1),5);
    Serial.print("\t\tVar: ");
    Serial.print(ControlArray4.PullArgument(i+1),5);
    Serial.print("\t\tStddev: ");
    Serial.print(ControlArray5.PullArgument(i+1),5);
    Serial.print("\n");
  }

  Serial.print("\n\nStatistics:");
  Serial.print("\nAverage:");
  Serial.print(SlidingArray1.GetAverage(),5);
  Serial.print("\nVariance:");
  Serial.print(SlidingArray1.GetVariance(),5);
  Serial.print("\nStdDeviation:");
  Serial.print(SlidingArray1.GetStdDeviation(),5);
  Serial.print("\nMin:");
  Serial.print(SlidingArray1.GetMin(),5);
  Serial.print("\nMax:");
  Serial.print(SlidingArray1.GetMax(),5);

  
  //Replace second and third observation with arbitrary data
  SlidingArray1.PushArgument(2,22);  //22 takes the 2nd position
  SlidingArray1.PushArgument(3,33);  //33 takes the 3rd position
  
  //Push additional argument at position 20 == Outside range 
  //and so therefore place this element at the end of range switching 
  //all previous arguments one place.
  SlidingArray1.PushArgument(20,111);
    
  //Statistics  
  Serial.print("\n\nAfter Push Procedures:\n");
  for (int i=0;i<ArraySize;i++) {
    Serial.print("Arg[");
    Serial.print(i);
    Serial.print("]:");
    Serial.print(SlidingArray1.PullArgument(i+1),4);
    Serial.print("\n");
  }

  Serial.print("\n\nStatistics:");
  Serial.print("\nAverage:");
  Serial.print(SlidingArray1.GetAverage(),5);
  Serial.print("\nVariance:");
  Serial.print(SlidingArray1.GetVariance(),5);
  Serial.print("\nStdDeviation:");
  Serial.print(SlidingArray1.GetStdDeviation(),5);
  Serial.print("\nMin:");
  Serial.print(SlidingArray1.GetMin(),5);
  Serial.print("\nMax:");
  Serial.print(SlidingArray1.GetMax(),5);

  //
  SlidingArray1.PushArgument(1,22);  //22 takes the 2nd position
  SlidingArray1.PushArgument(ArraySize,111);  //22 takes the 2nd position
  Serial.print("\n\nLast Consistency Check:");
  Serial.print("\nAverage:");
  Serial.print(SlidingArray1.GetAverage(),5);
  Serial.print("\nCalcAverage:");
  Serial.print(SlidingArray1.RecalcAverage(),5);

  //Remove this instruction if you require Loop example
  while(1);  
}


//---- loop ----------------------------------------------------
void loop() {

  LoopCount++;
  
  Observation        = random(0,100);    

  //Sliding Array Fast Add
  SlidingArray1.AddArgument_NoMom(Observation);

  //Statistics  
  Serial.print("\n\n");
  Serial.print(LoopCount);
  Serial.print(")");

  Serial.print("\tAvg:");
  Serial.print(SlidingArray1.RecalcAverage(),5);
  Serial.print("\tVar:");
  Serial.print(SlidingArray1.RecalcVariance(),5);
  Serial.print("\tStddev:");
  Serial.print(SlidingArray1.RecalcStdDeviation(),5);
  Serial.print("\tMin:");
  Serial.print(SlidingArray1.RecalcMin(),5);
  Serial.print("\tMax:");
  Serial.print(SlidingArray1.RecalcMax(),5);
  Serial.print("\n");

  delay(1000);
  
}
