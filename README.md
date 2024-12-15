# Date Utility Library
본 프로젝트는 Static Linking, dlsym을 사용한 Dynamic Linking을 각각 구현하는 것을 목표로 하였습니다.

날짜 유틸리티 라이브러리는 아래의 기능들을 제공합니다.

- 현재 날짜 출력
- Date structs와 Unix timestamp 간의 값 전환
- 두 날짜 간의 일수 계산
- 현재 날짜에서 특정 일수를 더한 날짜 계산
- 날짜를 특정 형식으로 출력

## 개발 환경

OS: Ubuntu 20.04 LTS

Compiler: GCC 11.4.0

## 라이브러리 사용법

### 라이브러리 Building

1. Object file 컴파일:
   ```bash
   gcc -c -o date_time_utils.o date_time_utils.c
   ```
2. Static library 생성:
   ```bash
   ar rcs libdate_time_utils.a date_time_utils.o
   ```
3. Shared library 생성:
   ```bash
   gcc -shared -o libdate_time_utils.so date_time_utils.o
   ```

### 라이브러리 Usage

`date_time_utils.h`을 프로젝트에 포함시키고 `libdate_time_utils.a`(static) 또는 `libdate_time_utils.so`(shared)와 linking합니다.

## 코드 설명

### date_time_utils.h

date_time_utils에 사용되는 함수의 프로토타입과 구조체를 선언합니다.

```c
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

```

### date_time_utils.c

date_time_utils의 함수들을 정의합니다.

```c
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

```

### dynamic_loader.c

date_time_utils 라이브러리를 동적으로 로드하고 특정 함수를 검색해 런타임에 사용합니다.

실행 결과:

![image](https://github.com/user-attachments/assets/15a6e133-d7be-46fc-944a-98cdacc27c55)


```c
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
```

### test_date_time_utils.c

date_time_utils라이브러리를 사용하는 예시입니다.

실행 결과:

![image](https://github.com/user-attachments/assets/475c6065-4a90-45ac-b8b1-6abb58fe6aae)


```c
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

```
