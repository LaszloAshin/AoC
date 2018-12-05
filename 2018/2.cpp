#include <iostream>
#include <vector>

std::string
without_different_characters(const std::string& lhs, const std::string& rhs)
{
	std::string result;
	assert(lhs.size() == rhs.size());
	for (int i = 0; i < lhs.size(); ++i) {
		if (lhs[i] == rhs[i]) result += lhs[i];
	}
	return result;
}

int
main()
{
	std::vector<std::string> ss;
	std::copy(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), std::back_inserter(ss));
	int n2 = 0;
	int n3 = 0;
	for (auto& s : ss) {
		std::cout << s << std::endl;
		int count[26] = {0};
		for (auto c : s) ++count[c - 'a'];
		n2 += std::find(std::begin(count), std::end(count), 2) != std::end(count);
		n3 += std::find(std::begin(count), std::end(count), 3) != std::end(count);
	}
	std::cout << (n2 * n3) << std::endl;

	for (int i = 1; i < ss.size(); ++i) {
		for (int j = 0; j < i; ++j) {
			const auto wdc = without_different_characters(ss[i], ss[j]);
			if (wdc.size() == ss[i].size() - 1) std::cout << wdc << std::endl;
		}
	}
}
