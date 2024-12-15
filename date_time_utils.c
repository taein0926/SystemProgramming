#include "date_time_utils.h"
#include <stdio.h>
#include <stdlib.h>

static int is_leap_year(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

Date get_current_date() {
	time_t now = time(NULL);
	struct tm *tm_now = localtime(&now);

	Date current_date;
	current_date.year = tm_now->tm_year + 1900;
	current_date.month = tm_now->tm_mon + 1;
	current_date.day = tm_now->tm_mday;

	return current_date;
}

time_t date_to_timestamp(Date date) {
	struct tm t = {0};
	t.tm_year = date.year - 1900;
	t.tm_mon = date.month - 1;
	t.tm_mday = date.day;

	return mktime(&t);
}

Date timestamp_to_date(time_t timestamp) {
	struct tm *tm_date = localtime(&timestamp);

	Date date;
	date.year = tm_date->tm_year + 1900;
	date.month = tm_date->tm_mon + 1;
	date.day = tm_date->tm_mday;
	
	return date;
}

int calculate_date_difference(Date date1, Date date2) {
	time_t timestamp1 = date_to_timestamp(date1);
	time_t timestamp2 = date_to_timestamp(date2);

	return (int)difftime(timestamp2, timestamp1) / (60 * 60 * 24);
}

Date add_days_to_date(Date date, int days) {
	time_t timestamp = date_to_timestamp(date);
	timestamp += days * 60 * 60 * 24;

	return timestamp_to_date(timestamp);
}

char* format_date(Date date, char* buffer, size_t buffer_size) {
	snprintf(buffer, buffer_size, "%04d-%02d-%02d", date.year, date.month, date.day);
	return buffer;
}

