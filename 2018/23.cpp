#include <iostream>
#include <vector>
#include <numeric>

struct Pos {
	long x, y, z;
};

int dist(Pos a, Pos b) {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);;
}

std::ostream& operator<<(std::ostream& os, Pos p) {
	return os << "{" << p.x << ", " << p.y << ", " << p.z << "}";
}

struct Nanobot {
	Pos p;
	long r;
};

std::ostream& operator<<(std::ostream& os, Nanobot b) {
	return os << "{" << b.p << ", " << b.r << "}";
}

std::vector<Nanobot> bots;

long count_bots_at(Pos p) {
	return std::count_if(bots.begin(), bots.end(), [p](Nanobot b){ return dist(p, b.p) <= b.r; });
}

int main() {
	while (true) {
		long x, y, z, r;
		if (scanf("pos=<%ld,%ld,%ld>, r=%ld\n", &x, &y, &z, &r) != 4) break;
		bots.push_back({{x, y, z}, r});
	}
	std::cout << "count: " << bots.size() << "\n";
	const auto strongest = std::max_element(bots.begin(), bots.end(), [](Nanobot l, Nanobot r){ return l.r < r.r; });
	std::cout << "strongest: " << *strongest << "\n";
	const auto n = std::count_if(bots.begin(), bots.end(), [strongest](Nanobot b){ return dist(strongest->p, b.p) <= strongest->r; });
	std::cout << "n: " << n << "\n";
/*	Pos min = bots.front().p;
	int minr = bots.front().r;
	Pos max = min;
	int maxr = minr;
	for (const auto& b : bots) {
		if (b.p.x < min.x) {
			min.x = b.p.x;
		} else if (b.p.x > max.x) {
			max.x = b.p.x;
		}
		if (b.p.y < min.y) {
			min.y = b.p.y;
		} else if (b.p.y > max.y) {
			max.y = b.p.y;
		}
		if (b.p.z < min.z) {
			min.z = b.p.z;
		} else if (b.p.z > max.z) {
			max.z = b.p.z;
		}
		if (b.r < minr) {
			minr = b.r;
		} else if (b.r > maxr) {
			maxr = b.r;
		}
	}
	std::cout << "bbox min: " << min << ", max: " << max << "\n";
	std::cout << "r min: " << minr << ", max: " << maxr << "\n";*/
	const Pos origo{0, 0, 0};
//	std::cout << "in range of origo: " << std::count_if(bots.begin(), bots.end(), [origo](Nanobot b){ return dist(origo, b.p) <= b.r; }) << "\n";
	long max2 = 0; // 872?
	for (const auto& b : bots) {
		const Pos ps[] = {
			{ b.p.x - b.r, b.p.y, b.p.z },
			{ b.p.x + b.r, b.p.y, b.p.z },
			{ b.p.x, b.p.y - b.r, b.p.z },
			{ b.p.x, b.p.y + b.r, b.p.z },
			{ b.p.x, b.p.y, b.p.z - b.r },
			{ b.p.x, b.p.y, b.p.z + b.r },
		};
		for (const auto& p : ps) {
			const auto n = count_bots_at(p);
			if (n > max2) max2 = n;
		}
	}
	std::cout << "max nanobots: " << max2 << std::endl;
	long dmin = INT_MAX;
	long countmax = 0;
	Pos thepoint;
	for (const auto& b : bots) {
		const Pos ps[] = {
			{ b.p.x - b.r, b.p.y, b.p.z },
			{ b.p.x + b.r, b.p.y, b.p.z },
			{ b.p.x, b.p.y - b.r, b.p.z },
			{ b.p.x, b.p.y + b.r, b.p.z },
			{ b.p.x, b.p.y, b.p.z - b.r },
			{ b.p.x, b.p.y, b.p.z + b.r },
		};
		for (const auto& p : ps) {
			const auto n = count_bots_at(p);
			if (n == max2) {
				const auto d = dist(origo, p);
				if (d < dmin) dmin = d;
				++countmax;
				std::cout << p << ", " << b.p << ", d: " << d << std::endl;
				thepoint = p;
			}
		}
	}
	std::cout << "thepoint: " << thepoint << std::endl;
	std::cout << "dmin: " << dmin << "  countmax: " << countmax << std::endl;
	std::cout << "133022408 was too high\n";
/*	int atm = 100;
	int maxxx = 0;
	for (int x = -atm; x <= atm; ++x) {
		for (int y = -atm; y <= atm; ++y) {
			for (int z = -atm; z <= atm; ++z) {
				if (!x && !y && !z) continue;
				Pos p{thepoint.x + x, thepoint.y + y, thepoint.z + z};
				const int m =  count_bots_at(p);
				if (m > maxxx) maxxx = m;
				if (m == 872) std::cout << p << " xyz: " << Pos{x, y, z} << std::endl;
			}
		}
	}
	std::cout << "maxxx: " << maxxx << std::endl;*/
	for (;;) {
		const Pos p1{thepoint.x - 1, thepoint.y - 1, thepoint.z};
		const Pos p2{thepoint.x - 1, thepoint.y, thepoint.z - 1};
		const Pos p3{thepoint.x, thepoint.y - 1, thepoint.z - 1};
		const auto n1 = count_bots_at(p1);
		const auto n2 = count_bots_at(p2);
		const auto n3 = count_bots_at(p3);
		if (max2 > std::max({n1, n2, n3})) break;
		if (n1 > std::max({max2, n2, n3})) {
			thepoint = p1;
			max2 = n1;
		} else if (n2 > std::max({max2, n1, n3})) {
			thepoint = p2;
			max2 = n2;
		} else if (n3 > std::max({max2, n1, n2})) {
			thepoint = p3;
			max2 = n3;
		} else if (n1 == max2) {
			thepoint = p1;
		} else if (n2 == max2) {
			thepoint = p2;
		} else if (n3 == max2) {
			thepoint = p3;
		} else {
			assert(!"nem jo");
		}
	}
	std::cout << "after climbing: " << max2 << "  p: " << thepoint << "  d: " << dist(origo, thepoint) <<  std::endl;
}
