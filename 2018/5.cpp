#include <iostream>
#include <iterator>

static int
react(std::string s)
{
	for (int i = 0; i < s.size() - 1; ++i) {
		if (s[i] != s[i+1] && std::tolower(s[i]) == std::tolower(s[i+1])) {
			s.erase(s.begin() + i, s.begin() + i + 2);
			i = -1;
		}
	}
	return s.size();
}

static std::string
clean(std::string s, char c)
{
	c = std::tolower(c);
	std::string result;
	for (int i = 0; i < s.size(); ++i) {
		if (std::tolower(s[i]) != c) {
			result.push_back(s[i]);
		}
	}
//	std::cout << c << " " << result << " " << react(result) << std::endl;
	return result;
}

int
main()
{
	std::string s;
	std::copy(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>(), std::back_inserter(s));
	s.erase((s.rbegin() + 1).base());
	std::cout << react(s) << std::endl;
	int len[26];
	for (int i = 0; i < 26; ++i) {
		len[i] = react(clean(s, 'a' + i));
	}
	std::cout << *std::min_element(std::begin(len), std::end(len)) << std::endl;;
}
