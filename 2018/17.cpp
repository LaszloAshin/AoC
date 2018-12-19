#include <iostream>
#include <vector>

struct Point {
	int x{}, y{};
};

std::ostream& operator<<(std::ostream& os, Point p) {
	return os << '(' << p.x << ',' << p.y << ')';
}

struct Bbox {
	Point lo{500,0}, hi{500,0};

	void add(Point p) {
		if (p.x < lo.x) {
			lo.x = p.x;
		} else if (p.x > hi.x) {
			hi.x = p.x;
		}
		if (p.y < lo.y) {
			lo.y = p.y;
		} else if (p.y > hi.y) {
			hi.y = p.y;
		}
	}
};

struct Line {
	char dir;
	int constant;
	int begin;
	int end;
};

using Rows = std::vector<std::string>;

int miny = INT_MAX;
int count = 0;

bool flow(Rows& r, Point p) {
	if (p.x < 0 || p.x >= int(r.front().size()) || p.y >= int(r.size())) {
		return false;
	}
	if (r[p.y][p.x] != '.') {
		return r[p.y][p.x] == '#' || r[p.y][p.x] == '~';
	}
	r[p.y][p.x] = '|'; if (p.y >= miny) ++count;
	if (!flow(r, {p.x, p.y+1})) {
		return false;
	}
	bool still = true;
	int minx = p.x;
	for (int x = p.x - 1; x >= 0 && r[p.y][x] != '#'; --x) {
		assert(r[p.y][x] == '.');
		r[p.y][x] = '|'; if (p.y >= miny) ++count;
		if (!flow(r, {x, p.y + 1})) {
			still = false;
			break;
		}
		minx = x;
	}
	int maxx = p.x;
	for (int x = p.x + 1; x < int(r[p.y].size()) && r[p.y][x] != '#'; ++x) {
		assert(r[p.y][x] == '.');
		r[p.y][x] = '|'; if (p.y >= miny) ++count;
		if (!flow(r, {x, p.y + 1})) {
			still = false;
			break;
		}
		maxx = x;
	}
	if (still) {
		for (int x = minx; x <= maxx; ++x) {
			assert(r[p.y][x] == '|');
			r[p.y][x] = '~';
		}
	}
	return still;
}

int
main()
{
	std::vector<Line> lines;
	Bbox bb;
	for (;;) {
		char c1, c2;
		int constant, begin, end;
		if (scanf("%c=%d, %c=%d..%d\n", &c1, &constant, &c2, &begin, &end) != 5) break;
		assert((c1 == 'x' && c2 == 'y') || (c1 == 'y' && c2 == 'x'));
		lines.push_back({(c1 == 'x') ? '|' : '-', constant, begin, end});
		if (c1 == 'x') {
			bb.add({constant, begin});
			bb.add({constant, end});
			if (begin < miny) miny = begin;
		} else {
			bb.add({begin, constant});
			bb.add({end, constant});
			if (constant < miny) miny = constant;
		}
	}
	std::cout << "miny = " << miny << std::endl;
	--bb.lo.x;
	++bb.hi.x;
	assert(!bb.lo.y);
	std::cout << "bbox: " << bb.lo << ", " << bb.hi << std::endl;
	const auto width = bb.hi.x - bb.lo.x + 1;
	const auto height = bb.hi.y - bb.lo.y + 1;
	std::cout << "dim: " << Point{width, height} << std::endl;
	Rows rows(height, std::string(width, '.'));
	for (const auto& l : lines) {
		for (int i = l.begin; i <= l.end; ++i) {
			if (l.dir == '-') {
				rows.at(l.constant).at(i - bb.lo.x) = '#';
			} else {
				rows.at(i).at(l.constant - bb.lo.x) = '#';
			}
		}
	}
	rows.at(0).at(500 - bb.lo.x) = '+';
	flow(rows, {500 - bb.lo.x, 1});
//	std::copy(rows.begin(), rows.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
	std::cout << count << std::endl;
	int still = 0;
	for (const auto& r : rows) {
		for (const auto& c : r) {
			if (c == '~') ++still;
		}
	}
	std::cout << "still: " << still << std::endl;
}
