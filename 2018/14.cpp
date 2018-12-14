#include <iostream>
#include <vector>

int
main()
{
	std::vector<int> r{ 3, 7 };
	int a = 0, b = 1;
	int input = 846010000;
	while (r.size() < input + 10) {
		int newrec = r[a] + r[b];
		int nr10 = newrec / 10;
		int nr1 = newrec % 10;
		if (nr10) r.push_back(nr10);
		r.push_back(nr1);
		a = (a + 1 + r[a]) % r.size();
		b = (b + 1 + r[b]) % r.size();
//		std::copy(r.begin(), r.end(), std::ostream_iterator<int>(std::cout, " "));
//		std::cout << std::endl;
	}
	for (int i = input; i < input + 10; ++i) {
		std::cout << r[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < r.size() - 6; ++i) {
		if (r[i] == 0 && r[i+1] == 8 && r[i+2] == 4 && r[i+3] == 6 && r[i+4] == 0 && r[i+5] == 1) {
			std::cout << i << std::endl;
			return 0;
		}
	}
}
