#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using current_time = uint32_t;

class Object {
public:
	Object(char t);
	current_time click_b, click_e, click_m;
	std::pair<int, int> b_coord, r_coord, e_coord, m_coord;
	std::string type;
	int result = 0, stage = 0;
};

class Dot : public Object {
public:
	Dot(std::pair<int, int> xy, current_time current_time_event);
};

class Line : public Object {
public:
	Line(std::pair<int, int> b, std::pair<int, int> e, current_time c_b, current_time c_e);
};

class Line_Ret : public Line {
public:
	Line_Ret(std::pair<int, int> b, std::pair<int, int> e, current_time c_b, current_time c_e, current_time c_m);
};

class Curve : public Object {
public:
	Curve(std::pair<int, int> b, std::pair<int, int> m, std::pair<int, int>  e, std::pair<int, int>  r, current_time c_b, current_time c_e);
};

class Curve_Ret : public Curve {
public:
	Curve_Ret(std::pair<int, int> b, std::pair<int, int> m, std::pair<int, int>  e, std::pair<int, int>  r, current_time c_b, current_time c_e, current_time c_m);
};

class Song {
public:
	int duration;
	std::string artist, song_name;
	sf::Texture bg;
	sf::Sprite BG;
	std::string name;
	std::vector<std::shared_ptr<Object>> song;
	Song(const std::string& Name);
	void Load_from_dir(const std::string& name);
	void Load_from_dir();
	void Delete_from_RAM();
};

class List {
public:
	std::vector<Song> songlist;
	List();
};