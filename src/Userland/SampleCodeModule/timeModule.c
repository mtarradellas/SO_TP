#include "timeModule.h"
#include "stdint.h"
#include "SYSCall.h"

unsigned int getHour() {
	unsigned int t;
	systemCall((uint64_t)READ, (uint64_t)TIME, (uint64_t)&t, (uint64_t)HOUR, 0, 0);
	if (t<3) {
		t = 24 - t;
	}
	else t -= 3;
	return t;
}

unsigned int getMinute() {
	unsigned int t;
	systemCall((uint64_t)READ, (uint64_t)TIME, (uint64_t)&t, (uint64_t)MINUTE, 0, 0);
	return t;
}

unsigned int getSecond() {
	unsigned int t;
	systemCall((uint64_t)READ, (uint64_t)TIME, (uint64_t)&t, (uint64_t)SECOND, 0, 0);
	return t;
}

void wait(int n) {
	systemCall((uint64_t) WAIT, (uint64_t) &n, 0, 0, 0, 0);
}