#include "Song.h"
#include <iostream>

Object::Object(char t) { type = t; }

Dot::Dot(std::pair<int, int> xy, current_time current_time_event)
	: Object('D') {
	click_b = current_time_event; 
	b_coord = xy;
}

Line::Line(std::pair<int, int> b, std::pair<int, int> e, current_time c_b, current_time c_e)
	: Object('L') {
	b_coord = b;
	e_coord = e;
	click_b = c_b; 
	click_e = c_e;
}

Line_Ret::Line_Ret(std::pair<int, int> b, std::pair<int, int> e, current_time c_b, current_time c_e, current_time c_m)
	: Line(b, e, c_b, c_e) {
	click_m = c_m;
	type += 'R';
}

Curve::Curve(std::pair<int, int> b, std::pair<int, int> m, std::pair<int, int>  e, std::pair<int, int>  r, current_time c_b, current_time c_e)
	: Object('C') {
	b_coord = b;
	e_coord = e; 
	m_coord = m;
	r_coord = r;
	click_b = c_b; 
	click_e = c_e;
}

Curve_Ret::Curve_Ret(std::pair<int, int> b, std::pair<int, int> m, std::pair<int, int>  e, std::pair<int, int>  r, current_time c_b, current_time c_e, current_time c_m)
	: Curve(b, m, e, r, c_b, c_e) {
	click_m = c_m;
	type += 'R';
}

Song::Song(const std::string& Name)
	: name(Name) {
	std::ifstream info;
	std::string str;
	info.open("songs/" + name + "/info.mosu");
	getline(info, str);
	artist = str;
	getline(info, str);
	song_name = str;
	getline(info, str);
	duration = stoi(str);
	bg.loadFromFile("songs/" + name + "/bg.jpg");
	sf::Sprite temp(bg);
	BG = temp;
}

std::shared_ptr<Object> Parce(std::string& str) {
	std::istringstream inp(str);
	std::string type;
	inp >> type;
	if (type == "D") {
		int x, y;
		current_time tme;
		inp >> x >> y >> tme;
		return std::make_shared<Object>(Dot({ x,y }, tme));
	}
	else if (type == "L" || type == "LR") {
		int x1, y1, x2, y2;
		current_time s_tme, e_tme, r_tme;
		inp >> x1 >> y1 >> x2 >> y2 >> s_tme >> e_tme;
		if (type == "L") {
			return std::make_shared<Object>(Line({ x1, y1 }, { x2, y2 }, s_tme, e_tme));
		}
		else {
			inp >> r_tme;
			return std::make_shared<Object>(Line_Ret({ x1, y1 }, { x2, y2 }, s_tme, r_tme, e_tme));
		}
	}
	else if (type == "C" || type == "CR") {
		int x1, y1, x2, y2, x3, y3, xr, yr;
		current_time s_tme, e_tme, r_tme;
		inp >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> s_tme >> e_tme;
		if (type == "C") {
			inp >> xr >> yr;
			return std::make_shared<Object>(Curve({ x1, y1 }, { x2, y2 }, { x3, y3 }, { xr, yr }, s_tme, e_tme));
		}
		else {
			inp >> r_tme >> xr >> yr;
			return std::make_shared<Object>(Curve_Ret({ x1, y1 }, { x2, y2 }, { x3, y3 }, { xr, yr }, s_tme, r_tme, e_tme));
		}
	}
	else {
		std::cout << "Wrong type " << type << std::endl;
	}
}

void Song::Load_from_dir(const std::string& name) {
	std::ifstream beats;
	std::string str;
	beats.open("songs/" + name + "/song.mosu");
	std::cout << name + "/song.mosu" << std::endl;
	while (getline(beats, str)) {
		song.push_back(Parce(str));
	}
	beats.close();
}

void Song::Load_from_dir() {
	Load_from_dir(name);
}

void Song::Delete_from_RAM() {
	song.resize(0);
}

List::List() {
	std::string str;
	std::ifstream list;
	list.open("songs/list.file");
	while (getline(list, str)) {
		songlist.push_back(Song(str));
	}
	list.close();
}