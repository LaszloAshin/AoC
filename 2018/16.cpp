#include <iostream>

/*
    1111110000000000
    5432109876543210  nalam -> valojaban
15: 0000000000010000  4 -> 15
 3: 00000000001.0000  5 -> 3
10: 0000000100..0000  8 -> 10
 9: 0000010.00000000 10 -> 9
14: 00010.0000000000 12 -> 14
 0: 010.0.0000000000 14 -> 0
 5: 1.0.0.0000.00000 15 -> 5
11: ..1.0.0000.00000 13 -> 11
 7: ....1.0000..0000 11 -> 7
 4: ......1000..0000  9 -> 4
12: ..00...000..0100  2 -> 12
13: ........00..1.00  3 -> 13
 6: 000000.00000.001  0 -> 6
 2: 000000000000..1.  1 -> 2
 1: .0....0.10.....0  7 -> 1
 8: 000......1......  6 -> 8
*/
void exec(int r[4], int op, int a, int b, int c) {
	switch (op) {
		case 6: // addr 0
			r[c] = r[a] + r[b];
			break;
		case 2: // addi 1
			r[c] = r[a] + b;
			break;
		case 12: // mulr 2
			r[c] = r[a] * r[b];
			break;
		case 13: // muli 3
			r[c] = r[a] * b;
			break;
		case 15: // banr 4
			r[c] = r[a] & r[b];
			break;
		case 3: // bani 5
			r[c] = r[a] & b;
			break;
		case 8: // borr 6
			r[c] = r[a] | r[b];
			break;
		case 1: // bori 7
			r[c] = r[a] | b;
			break;
		case 10: // setr 8
			r[c] = r[a];
			break;
		case 4: // seti 9
			r[c] = a;
			break;
		case 9: // gtir 10
			r[c] = a > r[b];
			break;
		case 7: // gtri 11
			r[c] = r[a] > b;
			break;
		case 14: // gtrr 12
			r[c] = r[a] > r[b];
			break;
		case 11: // eqir 13
			r[c] = a == r[b];
			break;
		case 0: // eqri 14
			r[c] = r[a] == b;
			break;
		case 5: // eqrr 15
			r[c] = r[a] == r[b];
			break;
	}
}


int main_part1() {
	int allcount = 0;
	uint16_t possibilities[16] = {
		0xffff, 0xffff, 0xffff, 0xffff,
		0xffff, 0xffff, 0xffff, 0xffff,
		0xffff, 0xffff, 0xffff, 0xffff,
		0xffff, 0xffff, 0xffff, 0xffff,
	};
	for (;;) {
		int rb0, rb1, rb2, rb3;
		if (scanf("Before: [%d, %d, %d, %d]\n", &rb0, &rb1, &rb2, &rb3) != 4) break;
		int realop, a, b, c;
		if (scanf("%d %d %d %d\n", &realop, &a, &b, &c) != 4) break;
		int ra0, ra1, ra2, ra3;
		if (scanf("After:  [%d, %d, %d, %d]\n", &ra0, &ra1, &ra2, &ra3) != 4) break;
		const int after[4] = { ra0, ra1, ra2, ra3 };
		int count = 0;
		for (int maybeop = 0; maybeop < 16; ++maybeop) {
			int r[4] = { rb0, rb1, rb2, rb3 };
			exec(r, maybeop, a, b, c);
			if (!memcmp(r, after, sizeof(r))) {
				++count;
			} else {
				possibilities[realop] &= ~(1 << maybeop);
			}
		}
		if (count >= 3) ++allcount;
	}
	std::cout << allcount << std::endl;
	for (int i = 0; i < 16; ++i) {
		printf("%2d: ", i);
		for (int j = 15; j >= 0; --j) {
			printf("%d", !!(possibilities[i] & (1 << j)));
		}
		puts("");
	}
	return 0;
}

int main() {
	int r[4] = {0};
	while (true) {
		int op, a, b, c;
		if (scanf("%d %d %d %d\n", &op, &a, &b, &c) != 4) break;
		exec(r, op, a, b, c);
	}
	std::cout << r[0] << std::endl;
}
