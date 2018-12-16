#include <iostream>
#include <vector>

int
main()
{
	std::vector<int> circle{0};
	// 478 players; last marble is worth 71240 points
	const int nplayers = 478;
	const int last_marble = 7124000;
	std::vector<long> scores(nplayers);
	int current = 0;
	for (int i = 1; i <= last_marble; ++i) {
		if (!(i % 23)) {
			current = (current - 7 + circle.size()) % circle.size();
			scores.at((i - 1) % nplayers) += i + circle.at(current);
			circle.erase(circle.begin() + current);;
			continue;
		}
		current = (current + 2) % circle.size();
		if (!current) {
			circle.push_back(i);
			current = circle.size() - 1;
		} else {
			circle.insert(circle.begin() + current, i);
		}
		if (!(i & 1023)) {
			std::cout << ((float(i) / last_marble) * 100) << std::endl;
		}
	}
//	std::copy(circle.begin(), circle.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << *std::max_element(scores.begin(), scores.end()) << std::endl;
}
