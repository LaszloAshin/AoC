#include <stdio.h>

int main() {
	int r0 = 16134795;
	int r3 = 0;
	int r4 = 0;
	int r5 = 0;
	do {
		r4 = r3 | 65536;
		r3 = 1107552;
		for (;;) {
			r3 += (r4 & 255);
			r3 &= 0xffffff;
			r3 *= 65899;
			r3 &= 0xffffff;
			if (r4 < 256) break;
			r5 = 1;
			while (r4 >= (r5 * 256)) {
				r5++;
			}
			r4 = r5 - 1;
		}
		printf("%d\n", r3);
	} while (/*r3 != r0*/1);
	printf("r0=%d r3=%d r4=%d r5=%d\n", r0, r3, r4, r5);
}
