#include <stdio.h>
#include <dlfcn.h>
#include "date_time_utils.h"

typedef Date (*GetCurrentDateFunc)();
typedef char* (*FormatDateFunc)(Date, char*, size_t);

int main() {
	void* handle = dlopen("./libdate_time_utils.so", RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "Error: %s\n", dlerror());
		return 1;
	}

	dlerror();

	GetCurrentDateFunc get_current_date = (GetCurrentDateFunc)dlsym(handle, "get_current_date");
	const char* error = dlerror();
	if (error) {
		fprintf(stderr, "Error loading get_current_date: %s\n", error);
		dlclose(handle);
		return 1;
	}

	FormatDateFunc format_date = (FormatDateFunc)dlsym(handle, "format_date");
	error = dlerror();
	if (error) {
		fprintf(stderr, "Error loadingi format_date: %s\n", error);
		dlclose(handle);
		return 1;
	}

	Date today = get_current_date();
	char buffer[20];
	format_date(today, buffer, sizeof(buffer));
	printf("Today's date: %s\n", buffer);

	dlclose(handle);
	return 0;
}

