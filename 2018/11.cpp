#include <cassert>
#include <iostream>

int grid[300][300];

template <int Serial>
static int
power(int x, int y)
{
	const auto rackid = x + 10;
	auto power = rackid * y;
	power += Serial;
	power *= rackid;
	power = (power / 100) % 10;
	power -= 5;
	return power;
}

template <int Serial>
static int
power(int x0, int y0, int n)
{
	int result = 0;
	for (int y = y0; y < y0 + n; ++y) {
		for (int x = x0; x < x0 + n; ++x) {
//			result += power<Serial>(x, y);
			result += grid[y][x];
		}
	}
	return result;
}

enum { MySerial = 4842 };

int
main()
{
/*	assert(power<8>(3, 5) == 4);
	assert(power<57>(122, 79) == -5);
	assert(power<39>(217, 196) == 0);
	assert(power<71>(101, 153) == 4);
	assert(power<18>(33, 45, 3) == 29);
	assert(power<42>(21, 61, 3) == 30);
	assert(power<18>(90, 269, 16) == 113);
	assert(power<42>(232, 251, 12) == 119);*/
	for (int y = 0; y < 300; ++y) {
		for (int x = 0; x < 300; ++x) {
			grid[y][x] = power<MySerial>(x, y);
		}
	}
	int bestx = 0;
	int besty = 0;
	int bestn = 300;
	int bestpow = power<MySerial>(0, 0, 300);
	for (int n = 299; n; --n) {
		std::cout << "n=" << n << std::endl;
		for (int y = 0; y <= 300 - n; ++y) {
			for (int x = 0; x <= 300 - n; ++x) {
				const auto pow = power<MySerial>(x, y, n);
				if (pow > bestpow) {
					bestpow = pow;
					bestx = x;
					besty = y;
					bestn = n;
				}
			}
		}
	}
	std::cout << bestx << ',' << besty << ',' << bestn << std::endl;
}
