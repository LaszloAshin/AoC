#include <iostream>
#include <map>
#include <vector>

static const std::map<std::string, void(*)(int[4], int, int, int)> opcodes {
	{ "addr", [](int r[6], int a, int b, int c){ r[c] = r[a] + r[b]; } },
	{ "addi", [](int r[6], int a, int b, int c){ r[c] = r[a] +   b ; } },
	{ "mulr", [](int r[6], int a, int b, int c){ r[c] = r[a] * r[b]; } },
	{ "muli", [](int r[6], int a, int b, int c){ r[c] = r[a] *   b ; } },
	{ "banr", [](int r[6], int a, int b, int c){ r[c] = r[a] & r[b]; } },
	{ "bani", [](int r[6], int a, int b, int c){ r[c] = r[a] &   b ; } },
	{ "borr", [](int r[6], int a, int b, int c){ r[c] = r[a] | r[b]; } },
	{ "bori", [](int r[6], int a, int b, int c){ r[c] = r[a] |   b ; } },
	{ "setr", [](int r[6], int a, int  , int c){ r[c] = r[a]       ; } },
	{ "seti", [](int r[6], int a, int  , int c){ r[c] =   a        ; } },
	{ "gtir", [](int r[6], int a, int b, int c){ r[c] =   a  > r[b]; } },
	{ "gtri", [](int r[6], int a, int b, int c){ r[c] = r[a] >   b ; } },
	{ "gtrr", [](int r[6], int a, int b, int c){ r[c] = r[a] > r[b]; } },
	{ "eqir", [](int r[6], int a, int b, int c){ r[c] =   a  == r[b]; } },
	{ "eqri", [](int r[6], int a, int b, int c){ r[c] = r[a] ==   b ; } },
	{ "eqrr", [](int r[6], int a, int b, int c){ r[c] = r[a] == r[b]; } },
};

static const std::map<std::string, void(*)(int, int, int)> disasm {
	{ "addr", [](int a, int b, int c){ printf("r%d = r%d + r%d;\n", c, a, b); } },
	{ "addi", [](int a, int b, int c){ printf("r%d = r%d + %d;\n", c, a, b); } },
	{ "mulr", [](int a, int b, int c){ printf("r%d = r%d * r%d;\n", c, a, b); } },
	{ "muli", [](int a, int b, int c){ printf("r%d = r%d * %d;\n", c, a, b); } },
	{ "banr", [](int a, int b, int c){ printf("r%d = r%d & r%d;\n", c, a, b); } },
	{ "bani", [](int a, int b, int c){ printf("r%d = r%d & %d;\n", c, a, b); } },
	{ "borr", [](int a, int b, int c){ printf("r%d = r%d | r%d;\n", c, a, b); } },
	{ "bori", [](int a, int b, int c){ printf("r%d = r%d | %d;\n", c, a, b); } },
	{ "setr", [](int a, int  , int c){ printf("r%d = r%d;\n", c, a); } },
	{ "seti", [](int a, int  , int c){ printf("r%d = %d;\n", c, a); } },
	{ "gtir", [](int a, int b, int c){ printf("r%d = %d > r%d;\n", c, a, b); } },
	{ "gtri", [](int a, int b, int c){ printf("r%d = r%d > %d;\n", c, a, b); } },
	{ "gtrr", [](int a, int b, int c){ printf("r%d = r%d > r%d;\n", c, a, b); } },
	{ "eqir", [](int a, int b, int c){ printf("r%d = %d == r%d;\n", c, a, b); } },
	{ "eqri", [](int a, int b, int c){ printf("r%d = r%d == %d;\n", c, a, b); } },
	{ "eqrr", [](int a, int b, int c){ printf("r%d = r%d == r%d;\n", c, a, b); } },
};

struct Instruction {
	std::string opcode;
	int a, b, c;
};

int main() {
	int ipidx;
//#ip 0
	assert(scanf("#ip %d\n", &ipidx) == 1);
	std::vector<Instruction> program;
	for (int i = 0;; ++i) {
		char opcode[5];
		int a, b, c;
		if (scanf("%s %d %d %d\n", opcode, &a, &b, &c) != 4) break;
		program.push_back({opcode, a, b, c});
		printf("label_%02d: ", i); disasm.find(opcode)->second(a, b, c);
	}
	for (int r0 = 16134795;; ++r0) {
		int r[6] = {r0,0,0,0,0,0};
		int ip = 0;
		int i = 0;
		for (;; ++i) {
			if (ip < 0 || ip >= int(program.size())) break;
			const auto& instr = program.at(ip);
			r[ipidx] = ip;
			opcodes.find(instr.opcode)->second(r, instr.a, instr.b, instr.c);
			if (ip == 28) {
//				std::cout << "r3=" << r[3] << std::endl;
			}
			ip = r[ipidx];
			++ip;
//			std::cout << "iter=" << i << " ip=" << ip << ' ';
//			std::copy(r, r + 6, std::ostream_iterator<int>(std::cout, " "));
//			std::cout << std::endl;
		}
		std::cout << r0 << " -> " << i << std::endl;
	  	printf("r0=%d r1=%d r3=%d r4=%d r5=%d\n", r[0], r[1], r[3], r[4], r[5]);
		break;
	}
}
