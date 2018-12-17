#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

struct Pos {
	int x{}, y{};

	bool operator<(Pos rhs) const {
		return std::make_pair(y, x) < std::make_pair(rhs.y, rhs.x);
	}

	bool operator==(Pos rhs) const {
		return x == rhs.x && y == rhs.y;;
	}
};

std::ostream&
operator<<(std::ostream& os, Pos p) {
	return os << '(' << p.x << ", " << p.y << ')';
}

struct Unit;

struct Map {
	Map();
	Map(const Map&);

	char at(Pos p) const { return rows_.at(p.y).at(p.x); }
	std::vector<Unit>& units() { return units_; }

	bool play_one_round();
	void print() const;
	Unit* find_unit(Pos p);

	using DistanceMap = std::vector<std::vector<int>>;

	DistanceMap computeDistanceMap(Pos p);

	bool is_it_done() const;

	int sumhitpoints() const;

private:
	void floodfill(DistanceMap& dm, Pos p, int d);

	std::vector<std::string> rows_;
	std::vector<Unit> units_;
};

struct ElfKilledException : std::runtime_error {
	ElfKilledException() : std::runtime_error("an elf was killed in the field") {}
};

struct Unit {
	static int elf_hit_power;

	Unit(Map& map, Pos pos, char type) : map_(&map), pos_(pos), type_(type) {}

	Unit(const Unit& u, Map& map) : map_(&map), pos_(u.pos_), type_(u.type_) {}

	Pos pos() const { return pos_; }
	char type() const { return type_; }

	void hit(Unit& other) const {
		other.hitpoint_ -= attack_power();
		if (other.type() == 'E' && other.hitpoint_ <= 0) {
			throw ElfKilledException();
		}
	}

	bool isDead() const { return hitpoint_ <= 0; }

	int hitpoint() const { return hitpoint_; }

	bool turn() {
		if (isDead()) return true;
		if (attack_if_can()) {
			return true;
		}
		auto candidates = collect_in_range();
		if (candidates.empty()) {
			return false;
		}
//		std::cout << "in-ranges for position " << pos() << ": " << std::endl;
//		std::copy(candidates.begin(), candidates.end(), std::ostream_iterator<Pos>(std::cout, "\n"));
		// filter out unreachables
		const auto dm = map_->computeDistanceMap(pos());
		candidates.erase(
			std::remove_if(candidates.begin(), candidates.end(), [&](Pos p){
				return dm.at(p.y).at(p.x) == INT_MAX;
			}),
			candidates.end()
		);
//		std::cout << "Reachables for position " << pos() << ": " << std::endl;
//		std::copy(candidates.begin(), candidates.end(), std::ostream_iterator<Pos>(std::cout, "\n"));
		// keep only nearest
		if (candidates.empty()) {
			return true;
		}
		auto nearest = candidates.front();;
		for (const auto& p : candidates) {
			if (dm.at(p.y).at(p.x) < dm.at(nearest.y).at(nearest.x)) {
				nearest = p;
			}
		}
		const auto distance = dm.at(nearest.y).at(nearest.x);
		assert(distance > 0);
		assert(distance < INT_MAX);
		candidates.erase(
			std::remove_if(candidates.begin(), candidates.end(), [&](Pos p){
				return dm.at(p.y).at(p.x) > distance;
			}),
			candidates.end()
		);
//		std::cout << "Nearest for position " << pos() << ": " << std::endl;
//		std::copy(candidates.begin(), candidates.end(), std::ostream_iterator<Pos>(std::cout, "\n"));
		std::sort(candidates.begin(), candidates.end());
		const auto chosen = candidates.front();
//		std::cout << "Chosen for position " << pos() << ": " << chosen << std::endl;
		// what direction should we step?
		const auto revdm = map_->computeDistanceMap(chosen);
		int dist[4] = {
			revdm.at(pos().y - 1).at(pos().x),
			revdm.at(pos().y).at(pos().x - 1),
			revdm.at(pos().y).at(pos().x + 1),
			revdm.at(pos().y + 1).at(pos().x)
		};
		std::sort(std::begin(dist), std::end(dist));
		const auto mindist = dist[0];
//		std::cout << "the minimal distance is " << mindist << std::endl;
		if (revdm.at(pos().y - 1).at(pos().x) == mindist) {
			--pos_.y;
		} else if (revdm.at(pos().y).at(pos().x - 1) == mindist) {
			--pos_.x;
		} else if (revdm.at(pos().y).at(pos().x + 1) == mindist) {
			++pos_.x;
		} else if (revdm.at(pos().y + 1).at(pos().x) == mindist) {
			++pos_.y;
		} else {
			assert(!"impossible");
		}
		attack_if_can();
		return true;
	}

private:
	bool attack_if_can() {
		Unit* adjacents[4] = {
			map_->find_unit({pos().x, pos().y - 1}),
			map_->find_unit({pos().x - 1, pos().y}),
			map_->find_unit({pos().x + 1, pos().y}),
			map_->find_unit({pos().x, pos().y + 1}),
		};
		int min_hitpoint = INT_MAX;
		for (auto& p : adjacents) {
			if (!p) continue;
			if (p->type() == type()) {
				p = nullptr;
			} else if (p->hitpoint_ < min_hitpoint) {
				min_hitpoint = p->hitpoint_;
			}
		}
		if (min_hitpoint != INT_MAX) {
			for (auto p : adjacents) {
				if (p && p->hitpoint_ == min_hitpoint) {
					hit(*p);
					return true;;
				}
			}
		}
		return false;
	}

	std::vector<Pos> collect_in_range() {
		std::vector<Pos> result;
		for (const auto& u : map_->units()) {
			if (u.isDead()) continue;
			if (u.type() == type()) continue;
			const auto p = u.pos();
			if (map_->at({p.x, p.y - 1}) == '.') result.push_back({p.x, p.y - 1});
			if (map_->at({p.x - 1, p.y}) == '.') result.push_back({p.x - 1, p.y});
			if (map_->at({p.x + 1, p.y}) == '.') result.push_back({p.x + 1, p.y});
			if (map_->at({p.x, p.y + 1}) == '.') result.push_back({p.x, p.y + 1});
		}
		return result;
	}

	int attack_power() const { return (type() == 'E') ? elf_hit_power : 3; }

	int hitpoint_ = 200;
	Map* map_;
	Pos pos_;
	char type_;
};

int Unit::elf_hit_power = 3;

Map::Map() {
	for (int y = 0;; ++y) {
		std::string s;
		if (!std::getline(std::cin, s)) break;
		for (int x = 0; x < int(s.size()); ++x) {
			if (s[x] == '.' || s[x] == '#') continue;
			units_.emplace_back(*this, Pos{x, y}, std::exchange(s[x], '.'));
		}
		rows_.push_back(std::move(s));
	}
}

Map::Map(const Map& m) : rows_(m.rows_) {
	for (const auto& u : m.units_) {
		units_.emplace_back(u, *this);
	}
}

bool Map::play_one_round() {
	bool result = true;
	for (auto& u : units_) {
		if (!u.turn()) {
			result = false;
			break;
		}
	}
	units_.erase(
		std::remove_if(units_.begin(), units_.end(), [](const Unit& u){
			return u.isDead();
		}),
		units_.end()
	);
	std::sort(
		units_.begin(), units_.end(),
		[](const Unit& lhs, const Unit& rhs){ return lhs.pos() < rhs.pos(); }
	);
	return result;
}

void Map::print() const {
	auto i = units_.begin();
	auto j = i;
	for (int y = 0; y < int(rows_.size()); ++y) {
		for (int x = 0; x < int(rows_.at(y).size()); ++x) {
			const Pos p{x, y};
			if (p == i->pos()) {
				if (i->isDead()) {
					std::cout << 'X';
				} else {
					std::cout << i->type();
				}
				++i;
			} else {
				std::cout << at(p);
			}
		}
		std::cout << "  ";
		for (; j != i; ++j) {
			std::cout << ' ' << j->type() << '(' << j->hitpoint() << ')';;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

Unit* Map::find_unit(Pos p) {
	const auto i = std::find_if(
		units_.begin(), units_.end(),
		[p](const Unit& u){ return !u.isDead() && p == u.pos(); }
	);
	return (i != units_.end()) ? &*i : nullptr;
}

Map::DistanceMap Map::computeDistanceMap(Pos p) {
	DistanceMap result(rows_.size(), std::vector<int>(rows_.front().size(), INT_MAX));
	floodfill(result, p, 0);
/*	for (const auto& row : result) {
		for (const auto& p : row) {
			if (p == INT_MAX) {
				std::cout << 'x';
			} else {
				std::cout << char('@' + p);
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/
	return result;
}

void Map::floodfill(DistanceMap& dm, Pos p, int d) {
	auto& cp = dm.at(p.y).at(p.x);
	if (cp <= d) return;
	if (at(p) != '.') return;
	if (d && find_unit(p)) return;
	cp = d;
	floodfill(dm, {p.x, p.y - 1}, d + 1);
	floodfill(dm, {p.x - 1, p.y}, d + 1);
	floodfill(dm, {p.x + 1, p.y}, d + 1);
	floodfill(dm, {p.x, p.y + 1}, d + 1);
}

bool Map::is_it_done() const {
	if (units_.size() < 2) return true;
	for (auto& u : units_) {
		if (u.type() != units_.front().type()) {
			return false;
		}
	}
	return true;
}

int Map::sumhitpoints() const {
	return std::accumulate(units_.begin(), units_.end(), 0, [](int a, const auto& u){
		return a + u.hitpoint();
	});
}

struct Combat {
	void play() {
		std::cout << "Initially:" << std::endl;
		map_.print();
		int i = 1;
		for (; !map_.is_it_done(); ++i) {
			if (!map_.play_one_round()) {
				break;
			}
			std::cout << "After " << i << " round";
			if (i > 1) {
				std::cout << 's';
			}
			std::cout << ':' << std::endl;
			map_.print();
		}
		const auto full_rounds = i - 1;
		std::cout << "Combat ends after " << full_rounds << " full rounds\n";
		const auto hp = map_.sumhitpoints();
		const auto outcome = full_rounds * hp;
		std::cout << "Outcome: " << full_rounds << " * " << hp << " = " << outcome << std::endl;
	}

private:
	Map map_;
};

int
main()
{
	Combat c;
	for (int i = 3;; ++i) {
		Unit::elf_hit_power = i;
		try {
			Combat(c).play();
			break;
		} catch (const ElfKilledException&) {
		}
	}
}
