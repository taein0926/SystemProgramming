#include "date_time_utils.h"
#include <stdio.h>

int main() {
	Date today = get_current_date();
	char buffer[20];
	printf("Today's date: %s\n", format_date(today, buffer, sizeof(buffer)));

	time_t timestamp = date_to_timestamp(today);
	Date from_timestamp = timestamp_to_date(timestamp);
	printf("Converted back: %s\n", format_date(from_timestamp, buffer, sizeof(buffer)));

	Date some_date = {2023, 12, 25};
	int difference = calculate_date_difference(today, some_date);
	printf("Days until 2023-12-25: %d days\n", difference);

	Date future_date = add_days_to_date(today, 100);
	printf("100 days from today: %s\n", format_date(future_date, buffer, sizeof(buffer)));

	return 0;
}

