#include <iostream>
#include <vector>

struct Point {
	int x;
	int y;
};

static int
manhattan(const Point& lhs, const Point& rhs)
{
	return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y);
}

std::ostream&
operator<<(std::ostream& os, const Point& p)
{
	return os << "(" << p.x << ", " << p.y << ")";
}

int
main(int argc, char* argv[])
{
	assert(argc == 2);
	const int margin = atoi(argv[1]);
	std::vector<Point> ps;
	Point max;
	for (;;) {
		Point p;
		if (scanf("%d, %d\n", &p.x, &p.y) != 2) break;
//		std::cout << p << std::endl;
		ps.push_back(p);
		max = Point{ std::max(max.x, p.x), std::max(max.y, p.y) };
	}
	std::vector<int> areas(ps.size(), 0);
	int area10000 = 0;
	for (int y = -margin; y < max.y + margin; ++y) {
		for (int x = -margin; x < max.x + margin; ++x) {
			const Point p{ x, y };
			int min_dist = manhattan(p, ps.front());
			int min_i = 0;
			bool unique = true;
			int distsum = min_dist;;
			for (int i = 1; i < ps.size(); ++i) {
				const auto dist = manhattan(p, ps[i]);
				if (dist < min_dist) {
					min_dist = dist;
					min_i = i;
					unique = true;
				} else if (dist == min_dist) {
					unique = false;
				}
				distsum += dist;
			}
			if (distsum < 10000) ++area10000;
			char c = '.';
			if (unique) {
//				c = (min_dist ? 'a' : 'A') + min_i;
				c = ('A') + min_i;
				++areas[min_i];
			}
//			std::cout << c;
		}
//		std::cout << std::endl;
	}
	for (int i = 0; i < areas.size(); ++i) {
		std::cout << char('A' + i) << ": " << areas[i] << std::endl;
	}
	std::cout << "area10000: " << area10000 << std::endl;;
}
