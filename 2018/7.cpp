#include <cstdio>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>

static void
part1(std::vector<std::pair<char, char>> deps, std::set<char> repo)
{
	while (!repo.empty()) {
		auto possibles = repo;
		for (const auto& dep : deps) {
			possibles.erase(dep.second);
		}
		auto next = *possibles.begin();
		std::cout << next;
		repo.erase(next);
		deps.erase(std::remove_if(deps.begin(), deps.end(), [next](const std::pair<char, char>& dep){ return dep.first == next; }), deps.end());
	}
	std::cout << std::endl;
}

struct Worker {
	int time{};
	char task{};
};

static void
part2(std::vector<std::pair<char, char>> deps, std::set<char> repo)
{
	int time = 0;
	Worker workers[5];
	while (!repo.empty() || std::accumulate(std::begin(workers), std::end(workers), 0, [](int a, Worker w){ return a + w.time; })) {
		for (auto& w : workers) {
			if (w.time) --w.time;
		}
		auto possibles = repo;
		for (const auto& dep : deps) {
			possibles.erase(dep.second);
		}
		for (auto& w : workers) {
			if (w.time) continue;
			if (w.task) {
				const auto t = w.task;
				deps.erase(std::remove_if(deps.begin(), deps.end(), [t](const std::pair<char, char>& dep){ return dep.first == t; }), deps.end());
				w.task = '\0';
			}
			if (possibles.empty()) continue;
			auto next = *possibles.begin();
			w.time = 60 + next - 'A';
			w.task = next;
			repo.erase(w.task);
			possibles.erase(w.task);
		}
		std::cout << (1000 + time);
		for (auto w : workers) {
			std::cout << ' ' << (w.task ? w.task : '.');
		}
		std::cout << std::endl;
		++time;
	}
	std::cout << time << std::endl;
}

int
main()
{
	std::vector<std::pair<char, char>> deps;
	std::set<char> repo;
	for (;;) {
		char prereq, target;
		if (scanf("Step %c must be finished before step %c can begin.\n", &prereq, &target) != 2) break;
		std::cout << prereq << " -> " << target << std::endl;
		deps.push_back(std::make_pair(prereq, target));
		repo.insert(prereq);
		repo.insert(target);
	}
	part1(deps, repo);
	part2(deps, repo);
}
