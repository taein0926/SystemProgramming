#ifndef DATE_TIME_UTILS_H
#define DATE_TIME_UTILS_H

#include <time.h>

typedef struct {
	int year;
	int month;
	int day;
} Date;

Date get_current_date();
time_t date_to_timestamp(Date date);
Date timestamp_to_date(time_t timespamp);
int calculate_date_difference(Date date1, Date date2);
Date add_days_to_date(Date date, int days);
char* format_date(Date date, char* buffer, size_t buffer_size);

#endif

