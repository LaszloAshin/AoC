#include <iostream>
#include <vector>
#include <cmath>

struct Point {
	int x{}, y{};

	Point& operator+=(const Point& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Point& operator/=(int n) {
		x /= n;
		y /= n;
		return *this;
	}

	Point operator*(int a) const {
		Point result(*this);
		result.x *= a;
		result.y *= a;
		return result;
	}

	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
};

int dist(const Point& lhs, const Point& rhs) {
	return std::sqrt(std::pow(lhs.x - rhs.x, 2) + std::pow(lhs.y - rhs.y, 2));
}

struct Light {
	Point p, v;
};

struct BBox {
	Point a, b;
};

struct Field {
	Field() {
		for (;;) {
			Light l;
			if (scanf("position=<%d, %d> velocity=<%d, %d>\n", &l.p.x, &l.p.y, &l.v.x, &l.v.y) != 4) break;
			ls.push_back(l);
		}
	}

	void run() {
		int t = -1;
		for (int prev = entropy();;) {
			move(), ++t;
			const auto e = entropy();
			if (e > prev) break;
			prev = e;
		}
		move(-1);
		print();
		std::cout << t << std::endl;
	}

private:
	void print() const {
		const auto bb = bbox();
		for (int y = bb.a.y; y <= bb.b.y; ++y) {
			for (int x = bb.a.x; x <= bb.b.x; ++x) {
				std::cout << (islight({x, y}) ? '#' : '.');
			}
			std::cout << std::endl;
		}
	}

	bool islight(const Point& p) const {
		for (const auto& l : ls) {
			if (l.p == p) {
				return true;
			}
		}
		return false;
	}

	BBox bbox() const {
		BBox result{ ls.front().p, ls.front().p };
		for (const auto& l : ls) {
			if (l.p.x < result.a.x) {
				result.a.x = l.p.x;
			} else if (l.p.x > result.b.x) {
				result.b.x = l.p.x;
			}
			if (l.p.y < result.a.y) {
				result.a.y = l.p.y;
			} else if (l.p.y > result.b.y) {
				result.b.y = l.p.y;
			}
		}
		return result;
	}

	void move(int amount = 1) {
		for (auto& l : ls) {
			l.p += l.v * amount;
		}
	}

	int entropy() const {
		int result = 0;
		const auto c = center();
		for (const auto& l : ls) {
			result += dist(l.p, c);
		}
		return result;
	}

	Point center() const {
		Point result;
		for (auto& l : ls) {
			result += l.p;
		}
		result /= ls.size();
		return result;
	}

	std::vector<Light> ls;
};

int
main()
{
	Field().run();
}
