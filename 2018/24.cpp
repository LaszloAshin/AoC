#include <vector>
#include <tuple>
#include <sstream>
#include <numeric>
using namespace std;
enum Side {
	immune_system, infection
};
const char* name(Side s) {
	switch (s) {
	case immune_system: return "Immune System";
	case infection: return "Infection";
	}
	return "<INVALID_SIDE>";
}
enum Type {
	none = 0,
	bludgeoning = 1,
	cold = 2,
	fire = 4,
	radiation = 8,
	slashing = 16
};

string name(Type t) {
	ostringstream oss;
	for (int i = 0; t; ++i) {
		switch (t & (1 << i)) {
		case bludgeoning: oss << "|bludgeoning"; break;
		case cold: oss << "|cold"; break;
		case fire: oss << "|fire"; break;
		case radiation: oss << "|radiation"; break;
		case slashing: oss << "|slashing"; break;
		}
	}
	auto s= oss.str();
	return (s.empty()) ? "none" : s.substr(1);
};

struct Group {
	Side side;
	int num;
	int units;
	int hitpoints;
	Type immune;
	Type weak;
	int damage;
	Type attack;
	int initiative;
	Group* to_be_attacked_by = nullptr;
	Group* to_attack = nullptr;

	int effective_power() const { return units * damage; }

	bool operator<(Group o) const {
		return make_tuple(o.effective_power(), o.initiative) < make_tuple(effective_power(), initiative);
	}

	int compute_damage(const Group& def) const {
		if (attack & def.immune) return 0;
		auto power = effective_power();
		if (attack & def.weak) power *= 2;
		return power;
	}
};

const int boost = 39;
#if 1
vector<Group> is {
Group{immune_system,1,1432 , 7061 , cold, bludgeoning,               41 + boost,  slashing,   17},
Group{immune_system,2,3387 , 9488 , none, bludgeoning,                   27 + boost,  slashing,   20},
Group{immune_system,3,254  , 3249 , fire, none,                          89 + boost,  cold,       1},
Group{immune_system,4,1950 , 8201 , none, none,                              39 + boost,  fire,       15},
Group{immune_system,5,8137 , 3973 , radiation, slashing,                4 + boost,   radiation, 6},
Group{immune_system,6,4519 , 7585 , none, fire,                          15 + boost,  radiation,  8},
Group{immune_system,7,763  , 7834 , Type(radiation | slashing | cold), fire, 91 + boost,  radiation,  18},
Group{immune_system,8,935 , 10231 , Type(slashing | cold), none,               103 + boost, bludgeoning, 12},
Group{immune_system,9,4557 , 7860 , slashing, none,                      15 + boost,  slashing,   11},
Group{immune_system,10,510  , 7363 , none, Type(fire | radiation),              143 + boost, fire,        5},
};

vector<Group> inf {
Group{infection,1,290  ,29776 , none, Type(cold | radiation),           204 , bludgeoning , 16},
Group{infection,2,7268 ,14114 , radiation, bludgeoning,        3   , bludgeoning , 19},
Group{infection,3,801  ,5393  , none,none,                           13  , slashing    , 13},
Group{infection,4,700  ,12182 , none,none,                           29  , cold        , 4},
Group{infection,5,531  ,16607 , slashing, none,                    53  , bludgeoning , 10},
Group{infection,6,23   ,24482 , none, Type(cold | fire),                2095, bludgeoning , 7},
Group{infection,7,8025 ,43789 , radiation,cold,                8   , radiation   , 9},
Group{infection,8,1405 ,53896 , none,none,                           70  , slashing    , 14},
Group{infection,9,566  ,7820  , cold, none,                       26  , cold        , 2},
Group{infection,10,1641 ,7807  , Type(slashing | bludgeoning), fire,  7   , radiation   , 3},
};
#else
vector<Group> is {
{immune_system,1,17  , 5390 , none, Type(radiation| bludgeoning),  4507 + boost ,fire,     2},
{immune_system,2,989 , 1274 , fire, Type(bludgeoning| slashing) ,  25 + boost   ,slashing, 3},
};

vector<Group> inf {
{infection,1,801  , 4706 , none, radiation,             116, bludgeoning,  1},
{infection,2,4485 , 2961 , radiation, Type( fire | cold),  12 , slashing,     4},
};
#endif

int main() {
	int units;
	int prev_units = INT_MAX;
	while (!is.empty() && !inf.empty()) {
		sort(is.begin(), is.end(), [](Group a, Group b){ return a.num < b.num; });
		sort(inf.begin(), inf.end(), [](Group a, Group b){ return a.num < b.num; });
		puts("Immune system:");
		for (const auto& g : is) printf("Group %d contains %d units\n", g.num, g.units);
		puts("Infection:");
		for (const auto& g : inf) printf("Group %d contains %d units\n", g.num, g.units);
		puts("");
		// target selection
		for (auto& g : inf) g.to_be_attacked_by = nullptr;
		for (auto& g : is) g.to_be_attacked_by = nullptr;
		sort(inf.begin(), inf.end());
		sort(is.begin(), is.end());
		for (auto& atk : inf) {
			vector<Group*> candidates;
			for (auto& def : is) {
				if (def.to_be_attacked_by) continue;
				const auto damage = atk.compute_damage(def);
				printf("Infection group %d would deal defending group %d %d damage\n", atk.num, def.num, damage);
				if (damage) {
					candidates.push_back(&def);
				}
			}
			if (candidates.empty()) continue;
			sort(candidates.begin(), candidates.end(), [atk](Group* a, Group* b){
				return make_tuple(atk.compute_damage(*a), a->effective_power(), a->initiative) > make_tuple(atk.compute_damage(*b), b->effective_power(), b->initiative);
			});
			candidates.front()->to_be_attacked_by = &atk;
		}
		for (auto& atk : is) {
			vector<Group*> candidates;
			for (auto& def : inf) {
				if (def.to_be_attacked_by) continue;
				const auto damage = atk.compute_damage(def);
				printf("Immune System group %d would deal defending group %d %d damage\n", atk.num, def.num, damage);
				if (damage) {
					candidates.push_back(&def);
				}
			}
			if (candidates.empty()) continue;
			sort(candidates.begin(), candidates.end(), [atk](Group* a, Group* b){
				return make_tuple(atk.compute_damage(*a), a->effective_power(), a->initiative) > make_tuple(atk.compute_damage(*b), b->effective_power(), b->initiative);
			});
			candidates.front()->to_be_attacked_by = &atk;
		}
		puts("");
		// attacking
		vector<Group*> atkers;
		for (auto& def : is) {
			if (def.to_be_attacked_by) {
				def.to_be_attacked_by->to_attack = &def;
				atkers.push_back(def.to_be_attacked_by);
			}
		}
		for (auto& def : inf) {
			if (def.to_be_attacked_by) {
				def.to_be_attacked_by->to_attack = &def;
				atkers.push_back(def.to_be_attacked_by);
			}
		}
		sort(atkers.begin(), atkers.end(), [](Group* b, Group* a){
			return a->initiative < b->initiative;
		});
		for (Group* g : atkers) {
			if (!g->units) continue;
			const auto damage = g->compute_damage(*g->to_attack);
			const auto kills = min(g->to_attack->units, damage / g->to_attack->hitpoints);
			printf("%s group %d attacks defending group %d, killing %d units\n", name(g->side), g->num, g->to_attack->num, kills);
			printf("  def units: %d  damage: %d  hp: %d\n", g->to_attack->units, damage, g->to_attack->hitpoints);
			g->to_attack->units -= kills;
		}
		puts("");
		is.erase(remove_if(is.begin(), is.end(), [](Group& g){ return !g.units; }), is.end());
		inf.erase(remove_if(inf.begin(), inf.end(), [](Group& g){ return !g.units; }), inf.end());
		units = 
			accumulate(is.begin(), is.end(), 0, [](int init, Group& g){ return init + g.units; })
			+ accumulate(inf.begin(), inf.end(), 0, [](int init, Group& g){ return init + g.units; });
		if (units == prev_units) {
			puts("no progress");
			return 1;
		}
		prev_units = units;
	}
	printf("winner is %s, with units %d\n", inf.empty() ? "Immune System": "Infection", units);
}
