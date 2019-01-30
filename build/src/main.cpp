 #include <iostream>
#include "include/tsu.h"
#include <vector>
#include <cmath>
#include "include/logger.h"

using namespace std;

int main () {

	cout << "\nRunning Frequency Response Loop\n" << endl;

	bool EventDetected = false;
	bool NegDeviation = false;
	bool OneShot0 = true;
	bool OneShot1 = true;
	bool EventResponse_T = false;
	float Actual_HZ = 0;
	float Prev_HZ = 0;
	float Delta_HZ = 0;
	float Total_HZ = 0;
	float MoveAvg_HZ = 60.00945663; //First value from spreadsheet data
	float EventStart_HZ = 0;
	float EventMin_HZ = 0;
	float OldEventMin_HZ = 0;
	float EventDelta_HZ = 0;
	float OldEventDelta_HZ = 0;
	float EventDelta_F = 0.026; //No explanation for this (floor delta?)
	float EventMaxDelta_HZ = 0;
	float EventTrigDelta_HZ = 0;
	unsigned int EventDurationSec = 0;
	float EventDelta_T = 10;
	float EventDelta_T_HZ = 0;
	float Event_HZ_Per_Sec = 0;
	float kW_LowLimit = 60 - EventDelta_F; //This variable is never defined and always "null" in the data
	unsigned int start_time = 0;
	unsigned int delta_time = 0;
	string log_path = "/home/pi/Leighton/FreqResLogic/data/LOGS/";

	vector<string> HZ_Data = tsu::FileToVector ("../data/Actual_HZ.csv", '\n');

	for (unsigned int i = 0; i < HZ_Data.size(); i++) {
		Actual_HZ = stof(HZ_Data.at(i));
		
		//Rung 1
		Delta_HZ = Actual_HZ - Prev_HZ;
		Prev_HZ = Actual_HZ;
		if ((Actual_HZ >= MoveAvg_HZ) || (Actual_HZ >= kW_LowLimit)) {
			Delta_HZ = 0;
		}

		//Rung 2
		if ((Actual_HZ < kW_LowLimit) && (Delta_HZ <= 0)) {
			if (EventDetected == false) {
				NegDeviation = true;
			}
		}

		//Rung 3
		OldEventMin_HZ = EventMin_HZ;
		if (NegDeviation == true) {
			if (OneShot0 == true) {
				OneShot0 = false;
				EventStart_HZ = abs(Delta_HZ) + Actual_HZ;
				EventMin_HZ = 99;
			}
			if (Actual_HZ < OldEventMin_HZ) {
				EventMin_HZ = Actual_HZ;
			}
			if (Actual_HZ - OldEventMin_HZ > 0.001) {
				NegDeviation = false;
				EventDurationSec = 0;
				EventDelta_HZ = 0;
			}
		} else {
			OneShot0 = true;
		}

		//Rung 4
		OldEventDelta_HZ = EventDelta_HZ;
		if ((Actual_HZ < MoveAvg_HZ) && (NegDeviation == true)) {
			EventDelta_HZ = EventStart_HZ - Actual_HZ;
			EventMaxDelta_HZ = EventDelta_HZ;
			EventDurationSec += 1;
			
			if (EventDurationSec <= EventDelta_T) {
				EventDelta_T_HZ = OldEventDelta_HZ;
			}
			
			Event_HZ_Per_Sec = OldEventDelta_HZ / EventDurationSec;
		}

		//Rung 5 appears to be irrelevant to us

		//Rung 6
		if (((NegDeviation == true) && (EventDelta_HZ >= EventDelta_F)
			&& (EventDurationSec <= EventDelta_T))
			|| ((EventResponse_T == true) && (delta_time < 180))) {
			EventDetected = true;
			
			if (OneShot1 == true) {
				OneShot1 = false;
				EventTrigDelta_HZ = EventDelta_HZ;
			}
			
			if (EventResponse_T == false) {
				EventResponse_T = true;
				start_time = i;
			} else if (EventResponse_T == true) {
				delta_time = i - start_time;
			}
		} else {
			EventDetected = false;
			OneShot1 = true;
			EventResponse_T = false;
		}
		
		if (i == 0) {
			Total_HZ = MoveAvg_HZ;
		} else {
			Total_HZ = Total_HZ + Actual_HZ;
			MoveAvg_HZ = Total_HZ / (i+1);
		}

		Logger ("FreqResData2", log_path)
			<< EventDetected << ","
			<< MoveAvg_HZ << ","
			<< Event_HZ_Per_Sec << ","
			<< EventMin_HZ << ","
			<< EventStart_HZ << ",";
	}

	return 0;
};