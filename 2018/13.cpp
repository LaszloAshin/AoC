#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <optional>

struct Field {
	Field() {
		for (std::string s; std::getline(std::cin, s); m_field.push_back(std::move(s)));
		for (int y = 0; y < int(m_field.size()); ++y) {
			for (int x = 0; x < int(m_field[y].size()); ++x) {
				switch (m_field[y][x]) {
				case '<':
					m_field[y][x] = '-';
					m_carts.emplace_back(x, y, Direction::LEFT);
					break;
				case '>':
					m_field[y][x] = '-';
					m_carts.emplace_back(x, y, Direction::RIGHT);
					break;
				case '^':
					m_field[y][x] = '|';
					m_carts.emplace_back(x, y, Direction::UP);
					break;
				case 'v':
					m_field[y][x] = '|';
					m_carts.emplace_back(x, y, Direction::DOWN);
					break;
				}
			}
		}
	}

	~Field() {
		const auto v = std::find_if(m_carts.begin(), m_carts.end(), [](const auto& c){ return !c.isRemoved(); });
		if (v != m_carts.end()) {
			std::cout << "Last vehicle standing: " << v->x() << "," << v->y() << std::endl;
		} else {
			std::cout << "No carts remained\n";
		}
	}

	void print() const {
		for (int y = 0; y < int(m_field.size()); ++y) {
			for (int x = 0; x < int(m_field[y].size()); ++x) {
				const auto cart = find_cart(x, y);
				std::cout << (cart ? cart->arrow() : m_field[y][x]);
			}
			std::cout << std::endl;
		}
	}

	bool
	tick()
	{
		std::sort(m_carts.begin(), m_carts.end(), [](const Cart& l, const Cart& r){ return std::make_pair(l.y(), l.x()) < std::make_pair(r.y(), r.x()); });
		for (auto& cart : m_carts) {
			if (cart.isRemoved()) continue;
			cart.move(*this);
			if (count_carts(cart.x(), cart.y()) > 1) {
				std::cout << "collision: " << cart.x() << "," << cart.y() << std::endl;
				for (auto& c : m_carts) {
					if (c.x() == cart.x() && c.y() == cart.y()) {
						if (c.isRemoved()) continue;
						c.remove();
						std::cout << "REMOVE(" << c.x() << ',' << c.y() << ')'<< std::endl;
					}
				}
			}
		}
		//m_carts.erase(std::remove_if(m_carts.begin(), m_carts.end(), [](const auto& c){ return c.isRemoved(); }), m_carts.end());
		return std::count_if(m_carts.begin(), m_carts.end(), [](const auto& c){ return !c.isRemoved(); }) > 1;;
	}

private:
	enum class Turn { LEFT, STRAIGHT, RIGHT };
	enum class Direction { UP, RIGHT, DOWN, LEFT };
	struct Cart {
		Cart(int x, int y, Direction d)
		: m_x(x)
		, m_y(y)
		, m_dir(d)
		{}

		int x() const { return m_x; }
		int y() const { return m_y; }

		char arrow() const {
			switch (m_dir) {
			case Direction::UP: return '^';
			case Direction::RIGHT: return '>';
			case Direction::DOWN: return 'v';
			case Direction::LEFT: return '<';
			}
		}

		void move(const Field& f) {
			switch (m_dir) {
			case Direction::UP:
				--m_y;
				if (f.m_field[y()][x()] == '/') {
					m_dir = Direction::RIGHT;
				} else if (f.m_field[y()][x()] == '\\') {
					m_dir = Direction::LEFT;
				}
				break;
			case Direction::RIGHT:
				++m_x;
				if (f.m_field[y()][x()] == '/') {
					m_dir = Direction::UP;
				} else if (f.m_field[y()][x()] == '\\') {
					m_dir = Direction::DOWN;
				}
				break;
			case Direction::DOWN:
				++m_y;
				if (f.m_field[y()][x()] == '/') {
					m_dir = Direction::LEFT;
				} else if (f.m_field[y()][x()] == '\\') {
					m_dir = Direction::RIGHT;
				}
				break;
			case Direction::LEFT:
				--m_x;
				if (f.m_field[y()][x()] == '/') {
					m_dir = Direction::DOWN;
				} else if (f.m_field[y()][x()] == '\\') {
					m_dir = Direction::UP;
				}
				break;
			}
			if (f.m_field[y()][x()] != '+') return;
			m_dir = static_cast<Direction>((static_cast<int>(m_dir) + static_cast<int>(m_nextTurn) - 1) & 3);
			m_nextTurn = static_cast<Turn>((static_cast<int>(m_nextTurn) + 1) % 3);
		}

		bool isRemoved() const { return m_removed; }
		void remove() { m_removed = true; }

	private:
		int m_x, m_y;
		Direction m_dir;
		Turn m_nextTurn{Turn::LEFT};
		bool m_removed{};
	};

	static bool iscart(char c) { return c == '>' || c == 'v' || c == '<' || c == '^'; }

	std::optional<Cart> find_cart(int x, int y) const {
		for (const auto& cart : m_carts) {
			if (!cart.isRemoved() && cart.x() == x && cart.y() == y) {
				return cart;
			}
		}
		return {};
	}

	int count_carts(int x, int y) const {
		return std::count_if(m_carts.begin(), m_carts.end(), [x, y](const auto& c){ return !c.isRemoved() && c.x() == x && c.y() == y; });
	}

	std::vector<std::string> m_field;
	std::vector<Cart> m_carts;
};

int
main()
{
	Field f;
	f.print();
	for (int i = 1; f.tick(); ++i) {
//		f.print();
		std::cout << i << std::endl;
	}
}
