#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <numeric>

enum class EvType { begin_shift, fall_asleep, wake_up };

const char*
name(EvType t)
{
	switch (t) {
	case EvType::begin_shift: return "BEGIN_SHIFT";
	case EvType::fall_asleep: return "FALL_ASLEEP";
	case EvType::wake_up: return "WAKE_UP";
	}
	assert(!"invalid type");
}

struct Event {
	Event()
	: m_time(read_timestamp())
	, m_id{}
	, m_type{}
	{
		if (!m_time) {
			return;
		}
		std::string event_first_word;
		std::cin >> event_first_word;
		if (event_first_word == "Guard") {
			m_type = EvType::begin_shift;
			char hash;
			std::cin >> hash;
			assert(hash == '#');
			std::cin >> m_id;
		} else if (event_first_word == "falls") {
			m_type = EvType::fall_asleep;
		} else if (event_first_word == "wakes") {
			m_type = EvType::wake_up;
		} else {
			assert(!"invalid event");
		}
		std::string trash;
		std::getline(std::cin, trash);
	}

	operator bool() const { return m_time; }

	friend std::ostream&
	operator<<(std::ostream& os, const Event& e)
	{
		e.print_timestamp();
		return os << " " << e.m_id << " " << name(e.m_type);
	}

	bool operator<(const Event& lhs) const { return m_time < lhs.m_time; }

	int time() const { return m_time; }
	int id() const { return m_id; }
	EvType type() const { return m_type; }
	void fix_id(int id) { assert(!m_id); m_id = id; }

private:
	static int
	read_timestamp()
	{
		tm t;
		memset(&t, 0, sizeof(t));
		// [1518-06-12 23:57] Guard #2633 begins shift
		if (scanf("[%d-%d-%d %d:%d] ", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min) != 5) {
			return 0;;
		}
		//t.tm_year -= 1900 - 400;
		t.tm_year = 1970 - 1900;
		--t.tm_mon;
		return timegm(&t) / 60;
	}

	void
	print_timestamp()
	const
	{
		const time_t ts = m_time * 60;
		tm *const t = gmtime(&ts);
		++(t->tm_mon);
		(t->tm_year) = 1518;
		printf("[%04d-%02d-%02d %02d:%02d]", t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min);
	}

	int m_time;
	int m_id;
	EvType m_type;
};

static int
compute_best_minute(const std::vector<Event>& evs, int sleeper)
{
	int minutes[60] = {0};
	{
		int last_id = evs.front().id();
		int last_sleep_time{};
		for (auto& e : evs) {
			if (e.type() == EvType::fall_asleep) {
				last_sleep_time = e.time();
			} else if (last_sleep_time && last_id == sleeper) {
//				std::cout << last_id << " sleeps from " << last_sleep_time << " to " << e.time() << std::endl;
				for (int i = last_sleep_time; i < e.time(); ++i) {
					++minutes[((i % 60) + 60) % 60];
				}
				last_sleep_time = 0;
			}
			last_id = e.id();
		}
	}
/*	for (int i = 0; i < 60; ++i) {
		std::cout << i << ": " << minutes[i] << std::endl;
	}
	std::cout << "check sleep sum: " << std::accumulate(std::begin(minutes), std::end(minutes), 0) << std::endl;
*/	
	const auto result = std::max_element(std::begin(minutes), std::end(minutes)) - std::begin(minutes);
	std::cout << "best_minute is best by " << minutes[result] << std::endl;
	return result;
}
	
int
main()
{
	std::vector<Event> evs;
	while (const auto e = Event()) {
		evs.push_back(e);
	}
	std::sort(evs.begin(), evs.end());
	std::map<int, int> sleep_time;
	{
		int last_id = evs.front().id();
		int last_sleep_time{};
		for (auto& e : evs) {
			if (e.type() == EvType::fall_asleep) {
				last_sleep_time = e.time();
			} else if (last_sleep_time) {
				sleep_time[last_id] += e.time() - last_sleep_time;
				last_sleep_time = 0;
			}
			if (e.type() == EvType::begin_shift) {
				last_id = e.id();
			} else {
				e.fix_id(last_id);
			}
		}
	}
	std::copy(evs.begin(), evs.end(), std::ostream_iterator<Event>(std::cout, "\n"));
	for (const auto& kvp : sleep_time) {
		std::cout << kvp.first << ": " << kvp.second << std::endl;
	}
	const auto sleeper = std::max_element(sleep_time.begin(), sleep_time.end(), [](std::pair<int, int> lhs, std::pair<int, int> rhs){ return lhs.second < rhs.second; })->first;
	std::cout << "sleeper: " << sleeper << std::endl;
	const auto best_minute = compute_best_minute(evs, sleeper);
	std::cout << "best minute: " << best_minute << std::endl;
	std::cout << "result: " << (sleeper * best_minute) << std::endl;

	for (const auto& kvp : sleep_time) {
		const auto best_minute = compute_best_minute(evs, kvp.first);
		std::cout << "id=" << kvp.first << " best_minute=" << best_minute << " product=" << (kvp.first * best_minute) << std::endl;
	}
}
