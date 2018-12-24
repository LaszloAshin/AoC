#include <iostream>
#include <vector>

static const char types[] = ".=|";

enum Type {
	TYPE_ROCKY = 0,
	TYPE_WET = 1,
	TYPE_NARROW = 2
};

enum Tool {
	TOOL_TORCH = 0,
	TOOL_CLIMBING = 1,
	TOOL_NEITHER = 2
};

struct ToolState {
	bool enqueued = false;
	int dist = INT_MAX;
};

struct Region {
	int erosion{};
	Type type{};
	ToolState tools[3];
};

struct ToVisitItem {
	int x, y, z;
};

#if 1
const int depth = 7740;
const int targetx = 12;
const int targety = 763;
#else
const int depth = 510;
const int targetx = 10;
const int targety = 10;
#endif
const int width = targetx * 3;
const int height = targety * 3;
std::vector<std::vector<Region>> map(height, std::vector<Region>(width));

bool operator<(ToVisitItem a, ToVisitItem b) {
	return map.at(a.y).at(a.x).tools[a.z].dist < map.at(b.y).at(b.x).tools[b.z].dist;
}

std::vector<ToVisitItem> toVisit;

bool can_enter(Type type, Tool tool) {
#define MIX(type, tool) (((type) << 4) | (tool))
	switch (MIX(type, tool)) {
	case MIX(TYPE_ROCKY, TOOL_CLIMBING):
	case MIX(TYPE_ROCKY, TOOL_TORCH):
		return true;
	case MIX(TYPE_WET, TOOL_CLIMBING):
	case MIX(TYPE_WET, TOOL_NEITHER):
		return true;
	case MIX(TYPE_NARROW, TOOL_TORCH):
	case MIX(TYPE_NARROW, TOOL_NEITHER):
		return true;
	}
	return false;
#undef MIX
}

int main() {
	int risk = 0;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int geoidx;
			if (x == 0 && y == 0) {
				geoidx = 0;
			} else if (x == targetx && y == targety) {
				geoidx = 0;
			} else if (y == 0) {
				geoidx = x * 16807;
			} else if (x == 0) {
				geoidx = y * 48271;
			} else {
				geoidx = map[y][x-1].erosion * map[y-1][x].erosion;
			}
			const int erolev = (geoidx + depth) % 20183;
			auto& region = map.at(y).at(x);
			region.erosion = erolev;
			region.type = Type(erolev % 3);
//			printf(" %c ", (!x && !y) ? 'M' : (x == targetx && y == targety) ? 'T' : types[region.type]);
			printf("%c", (!x && !y) ? 'M' : (x == targetx && y == targety) ? 'T' : types[region.type]);
			if (y <= targety && x <= targetx) {
				risk += region.type;
			}
		}
		puts("");
	}
	printf("risk: %d\n", risk);
	map[0][0].tools[0].dist = 0;
	map[0][0].tools[0].enqueued = true;
	toVisit.push_back({0, 0, 0});
	while (!toVisit.empty()) {
		std::sort(toVisit.begin(), toVisit.end());
		const auto i = toVisit.front();
		toVisit.erase(toVisit.begin());
		auto& region = map.at(i.y).at(i.x);
		auto& thistool = region.tools[i.z];

		auto& neightool1 = region.tools[(i.z + 1) % 3];
		neightool1.dist = std::min(neightool1.dist, thistool.dist + 7);
		if (!neightool1.enqueued) {
			toVisit.push_back({i.x, i.y, (i.z + 1) % 3});
			neightool1.enqueued = true;
		}

		auto& neightool2 = region.tools[(i.z + 2) % 3];
		neightool2.dist = std::min(neightool2.dist, thistool.dist + 7);
		if (!neightool2.enqueued) {
			toVisit.push_back({i.x, i.y, (i.z + 2) % 3});
			neightool2.enqueued = true;
		}

		if (i.y > 0) {
			auto& neighregion = map.at(i.y - 1).at(i.x);
			if (can_enter(neighregion.type, Tool(i.z))) {
				auto& tool = neighregion.tools[i.z];
				tool.dist = std::min(tool.dist, thistool.dist + 1);
				if (!tool.enqueued) {
					toVisit.push_back({i.x, i.y - 1, i.z});
					tool.enqueued = true;
				}
			}
		}
		if (i.x > 0) {
			auto& neighregion = map.at(i.y).at(i.x - 1);
			if (can_enter(neighregion.type, Tool(i.z))) {
				auto& tool = neighregion.tools[i.z];
				tool.dist = std::min(tool.dist, thistool.dist + 1);
				if (!tool.enqueued) {
					toVisit.push_back({i.x - 1, i.y, i.z});
					tool.enqueued = true;
				}
			}
		}
		if (i.y < height - 1) {
			auto& neighregion = map.at(i.y + 1).at(i.x);
			if (can_enter(neighregion.type, Tool(i.z))) {
				auto& tool = neighregion.tools[i.z];
				tool.dist = std::min(tool.dist, thistool.dist + 1);
				if (!tool.enqueued) {
					toVisit.push_back({i.x, i.y + 1, i.z});
					tool.enqueued = true;
				}
			}
		}
		if (i.x < width - 1) {
			auto& neighregion = map.at(i.y).at(i.x + 1);
			if (can_enter(neighregion.type, Tool(i.z))) {
				auto& tool = neighregion.tools[i.z];
				tool.dist = std::min(tool.dist, thistool.dist + 1);
				if (!tool.enqueued) {
					toVisit.push_back({i.x + 1, i.y, i.z});
					tool.enqueued = true;
				}
			}
		}
	}
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			printf("%02d ", map.at(y).at(x).tools[0].dist);
		}
		puts("");
	}
	std::cout << map.at(targety).at(targetx).tools[0].dist << std::endl;
}
