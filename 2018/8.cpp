#include <iostream>
#include <iterator>
#include <vector>
#include <numeric>

template <class I>
std::pair<int, I>
summetas(I first, I last)
{
	auto nchildren = *first++;
	const auto nmetas = *first++;
	int result = 0;
	while (nchildren--) {
		const auto r = summetas(first, last);
		first = r.second;
		result += r.first;
	}
	return std::make_pair(std::accumulate(first, first + nmetas, result), first + nmetas);
}

template <class I>
std::pair<int, I>
summetas2(I first, I last)
{
	auto nchildren = *first++;
	auto nmetas = *first++;
	if (!nchildren) return std::make_pair(std::accumulate(first, first + nmetas, 0), first + nmetas);
	std::vector<int> childvalues;
	while (nchildren--) {
		const auto r = summetas2(first, last);
		first = r.second;
		childvalues.push_back(r.first);
	}
	int result = 0;
	for (; nmetas--; ++first) {
		if (*first > 0 && *first <= childvalues.size()) {
			result += childvalues.at(*first - 1);
		}
	}
	return std::make_pair(result, first);
}

int
main()
{
	std::vector<int> v;
	std::copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(), std::back_inserter(v));
	std::cout << summetas(v.cbegin(), v.cend()).first << std::endl;
	std::cout << summetas2(v.cbegin(), v.cend()).first << std::endl;
}
