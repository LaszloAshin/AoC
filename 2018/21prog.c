#include <stdio.h>

int main() {
//	int r0 = 0;
	int r1 = 0;
	int r4 = 0;
	int r5 = 0;
	int r3 = 0;
	for (;;) {
		r4 = r3 | 0x10000;
		r3 = 1107552;
		for (;;) {
			r5 = r4 & 255;
			r3 += r5;
			r3 &= 0xffffff;
			r3 *= 65899;
			r3 &= 0xffffff;
			if (256 > r4) break;
			r5 = 0;
			for (;;) {
				r1 = r5 + 1;
				r1 *= 256;
				if (r4 > r1) break;
				r5++;
			}
			r4 = r5;
		}
		/*if (r3 == r3)*/ break;
	}
	printf("%d\n", r3);
	// 211808 nem jo
}
