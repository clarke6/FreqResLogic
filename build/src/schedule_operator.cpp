#include <iostream>
#include <string>
#include <vector>
#include "schedule_operator.h"
#include "tsu.h"

ScheduleOperator::ScheduleOperator (std::string input) : file_name_(input), index_(0){
	schedule_ = tsu::FileToMatrix(file_name_, ',', 4);
}

ScheduleOperator::~ScheduleOperator () {};

float ScheduleOperator::Loop () {
	std::string Op_command;
	std::string Op_amount;

// USE THIS SECTION IF DATA IS PARSED USING INTEGER UTC
	time_t now = time(0);
	now = now % 86400;

	for (unsigned int i = index_; i < schedule_.size(); i++) {
		if (std::stoi(schedule_[i][0]) % 86400 == now) {
			Op_command = schedule_[i][1];
			Op_amount = schedule_[i][2];
			if (i == (schedule_.size()-1)) {
				index_ = 0;
			} else {
				index_ = i;
			}
			break;
		}
	}

/* USE THIS SECTION IF DATA IS PARSED USING FORMATTED STRING LOCAL TIME

	time_t now = time(0);
	char time_formatted[100];
	tm now_local = *localtime(&now);
	strftime(time_formatted, sizeof(time_formatted), "%T", &now_local);

 	for (unsigned int i = 0; i < schedule_.size(); i++) {
		if (schedule_[i][3] == time_formatted) {
			Op_command = schedule_[i][1];
			Op_amount = schedule_[i][2];
			if (i == (schedule_.size()-1)) {
				index_ = 0;
			} else {
				index_ = i;
			}
			break;
		}
	}
*/

};

