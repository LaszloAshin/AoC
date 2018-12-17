#include <iostream>
#include <vector>

struct Point {
	int x{}, y{};
};

struct Field;

struct Cell {
	static std::unique_ptr<Cell> create(char avatar);

	virtual ~Cell() = default;

	virtual char avatar() const = 0;
	virtual void turn(Field&, const Point&) {}
};

struct Field {
	Field() {
		while (true) {
			std::string s;
			if (!std::getline(std::cin, s)) break;
			width = s.size();
			for (char c : s) {
				field.push_back(Cell::create(c));
			}
		}
	}

	void play_one_round() {
		int y = 0;
		for (auto i = field.begin(); i != field.end(); ++y) {
			for (int x = 0; x < width; ++x, ++i) {
				(*i)->turn(*this, {x, y});
			}
		}
	}

	void print() const {
		for (auto i = field.begin(); i != field.end();) {
			for (int x = 0; x < width; ++x, ++i) {
				std::cout << (*i)->avatar();
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	Cell& at(const Point& p) {
		return *field.at(p.y * width + p.x);
	}

private:
	std::vector<std::unique_ptr<Cell>> field;
	int width{};
};

struct Wall : Cell {
	char avatar() const override { return '#'; }
};

struct Cavern : Cell {
	char avatar() const override { return '.'; }
};

struct Unit : Cell {
	void hit(Unit& other) const {
		other.hitpoint -= attack_power();
	}

private:
	int attack_power() const { return 3; }

	void turn(Field&, const Point&) override {
	}

	int hitpoint = 200;
};

struct Elf : Unit {
	char avatar() const override { return 'E'; }
};

struct Goblin : Unit {
	char avatar() const override { return 'G'; }
};

std::unique_ptr<Cell> Cell::create(char avatar) {
	switch (avatar) {
		case '#': return std::make_unique<Wall>();
		case '.': return std::make_unique<Cavern>();
		case 'E': return std::make_unique<Elf>();
		case 'G': return std::make_unique<Goblin>();
	}
	assert(!"invalid avatar");
}

struct Combat {
	void play() {
		std::cout << "Initially:" << std::endl;
		f.print();
		for (int i = 1; i <= 3; ++i) {
			f.play_one_round();
			std::cout << "After " << i << " round";
			if (i > 1) {
				std::cout << 's';
			}
			std::cout << ':' << std::endl;
			f.print();
		}
	}

private:
	Field f;
};

int
main()
{
	Combat().play();;
}
