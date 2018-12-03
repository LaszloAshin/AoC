#include <iostream>
#include <set>
#include <vector>
#include <numeric>

int
main()
{
	std::vector<int> freqs;
	std::copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(), std::back_inserter(freqs));
	std::cout << "a: " << std::accumulate(freqs.begin(), freqs.end(), 0) << std::endl;

	std::set<int> seen_freqs;
	int result = 0;
	seen_freqs.insert(result);
	for (int i = 0;; i = (i + 1) % freqs.size()) {
		result += freqs[i];
		if (seen_freqs.find(result) != seen_freqs.end()) {
			std::cout << "b: " << result << std::endl;
			return 0;
		}
		seen_freqs.insert(result);
	}
}
