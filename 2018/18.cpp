#include <iostream>
#include <vector>

int main() {
	std::vector<std::string> rows;
	std::copy(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), std::back_inserter(rows));
	std::cout << "Initial state:" << std::endl;
	std::copy(rows.begin(), rows.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
	std::cout << std::endl;
	for (int i = 0; i < 1000000000; ++i) {
		std::vector<std::string> rows2;
		for (int y = 0; y < int(rows.size()); ++y) {
			rows2.emplace_back();
			for (int x = 0; x < int(rows[y].size()); ++x) {
				int trees = 0;
				int lumberyards = 0;
				if (y > 0) {
					if (x > 0) {
						if (rows[y-1][x-1] == '|') ++trees;
						if (rows[y-1][x-1] == '#') ++lumberyards;
					}
					if (rows[y-1][x] == '|') ++trees;
					if (rows[y-1][x] == '#') ++lumberyards;
					if (x < int(rows.front().size()) - 1) {
						if (rows[y-1][x+1] == '|') ++trees;
						if (rows[y-1][x+1] == '#') ++lumberyards;
					}
				}
				if (x > 0) {
					if (rows[y][x-1] == '|') ++trees;
					if (rows[y][x-1] == '#') ++lumberyards;
				}
				if (x < int(rows.front().size()) - 1) {
					if (rows[y][x+1] == '|') ++trees;
					if (rows[y][x+1] == '#') ++lumberyards;
				}
				if (y < int(rows.size()) - 1) {
					if (x > 0) {
						if (rows[y+1][x-1] == '|') ++trees;
						if (rows[y+1][x-1] == '#') ++lumberyards;
					}
					if (rows[y+1][x] == '|') ++trees;
					if (rows[y+1][x] == '#') ++lumberyards;
					if (x < int(rows.front().size()) - 1) {
						if (rows[y+1][x+1] == '|') ++trees;
						if (rows[y+1][x+1] == '#') ++lumberyards;
					}
				}
				switch (rows[y][x]) {
					case '.':
						rows2.back().push_back((trees >= 3) ? '|' : '.');
						break;
					case '|':
						rows2.back().push_back((lumberyards >= 3) ? '#' : '|');
						break;
					case '#':
						rows2.back().push_back((trees >= 1 && lumberyards >= 1) ? '#' : '.');
						break;
				}
			}
		}
		rows = std::move(rows2);
//		if (!(i & 65535)) {
/*			std::cout << "After " << (i + 1) << " minute(s):" << std::endl;
			std::copy(rows.begin(), rows.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
			std::cout << std::endl;
			std::cout << "Progress: " << (float(i) / 1000000000 * 100) << '%' << std::endl;*/
			int trees = 0;
			int lumberyards = 0;
			for (const auto& row : rows) {
				for (const auto& acre : row) {
					if (acre == '|') ++trees;
					if (acre == '#') ++lumberyards;
				}
			}
			std::cout << (i + 1) << ": " << (trees * lumberyards) << std::endl;
//		}
	}
	int trees = 0;
	int lumberyards = 0;
	for (const auto& row : rows) {
		for (const auto& acre : row) {
			if (acre == '|') ++trees;
			if (acre == '#') ++lumberyards;
		}
	}
	std::cout << (trees * lumberyards) << std::endl;
}
