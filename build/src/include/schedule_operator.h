#ifndef SCHEDULE_OPERATOR_H_
#define SCHEDULE_OPERATOR_H_
#include "tsu.h"
#include <string>

class ScheduleOperator {
public:
	ScheduleOperator (std::string input);
	virtual ~ScheduleOperator ();

	tsu::string_matrix schedule_;

	float Loop ();

private:
	std::string file_name_;
	unsigned int index_;
};

#endif