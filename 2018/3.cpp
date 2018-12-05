#include <iostream>
#include <cstdio>
#include <set>
#include <iterator>

int
main()
{
	uint16_t cells[1000 * 1000] = {0};
	uint16_t ids[1000 * 1000] = {0};
	std::set<uint16_t> non_overlapped;
	for (;;) {
		int id, x0, y0, w, h;
		if (scanf("#%d @ %d,%d: %dx%d\n", &id, &x0, &y0, &w, &h) != 5) break;
		non_overlapped.insert(id);
		for (int y = y0; y < y0 + h; ++y) {
			for (int x = x0; x < x0 + w; ++x) {
				const auto offs = y * 1000 + x;
				++cells[offs];
				if (ids[offs]) {
					non_overlapped.erase(ids[offs]);
					non_overlapped.erase(id);
				}
				ids[offs] = id;
			}
		}
	}
	std::cout << std::count_if(std::begin(cells), std::end(cells), [](uint16_t c){ return c >= 2; }) << std::endl;
	std::copy(non_overlapped.begin(), non_overlapped.end(), std::ostream_iterator<uint16_t>(std::cout, "\n"));
}
