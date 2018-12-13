#include <iostream>
#include <map>
#include <iomanip>

int
main()
{
	scanf("initial state: ");
	std::string s;
	std::cin >> s;
	std::cout << s << std::endl;
	std::map<std::string, char> rules;
	for (;;) {
		char old[6], new_;
		if (scanf("%s => %c\n", old, &new_) != 2) break;
		rules[old] = new_;
	}
	for (const auto& rule : rules) {
		std::cout << rule.first << " => " << rule.second << std::endl;
	}
	int offset{};
	std::cout << " 0: " << s << std::endl;
	for (long g = 1; g <= 50000; ++g) {
		while (s.size() > 3 && s[0] == '.' && s[1] == '.' && s[2] == '.' & s[3] == '.') {
			s.erase(s.begin());
			--offset;
		}
		while (s[0] != '.' || s[1] != '.' || s[2] != '.') {
			s = '.' + s;
			++offset;
		}
		while (s[s.size()-1] != '.' || s[s.size()-2] != '.' || s[s.size()-3] != '.') {
			s += '.';
		}
		std::string ng = "..";
		for (int i = 0; i <= s.size() - 5; ++i) {
			const auto fragment = s.substr(i, 5);
			bool found = false;
			for (const auto& rule : rules) {
				if (rule.first == fragment) {
					ng += rule.second;
					found = true;
					break;
				}
			}
			if (!found) {
				ng += '.';
			}
		}
		ng += "..";
		s = std::move(ng);
		if (!(g % 1000)) {
			static int oldoffset = 0;
			static int oldsum = 0;
			std::cout << std::setw(2) << g << ": " << s;
			int sum = 0;
			for (int i = 0; i < s.size(); ++i) {
				if (s[i] == '#') {
					sum += i - offset;
				}
			}
//			std::cout << " offset=" << offset << "(" << (offset - oldoffset) << ") sum=" << sum << "(" << (sum - oldsum) << ")" << std::endl;
			std::cout << "sum=" << sum << "(" << (sum - oldsum) << ")" << std::endl;
			oldoffset = offset;
			oldsum = sum;
		}
	}
}
