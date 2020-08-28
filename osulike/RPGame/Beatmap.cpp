#include "Beatmap.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <stdio.h>
#define SIZE 80
int a;

std::vector<sf::Sprite> ball;
sf::Sprite cursor, arrow;
float Dist(std::pair<int, int> x, std::pair<int, int> y) {
	return sqrtf(pow(x.first - y.first, 2) + pow(x.second - y.second, 2));
}
Beat_OBJ::Beat_OBJ(std::shared_ptr<Object> obj) {
	if (obj->type == "D") {
		type = "D";
		begin.setPosition(obj->b_coord.first - SIZE, obj->b_coord.second - SIZE);
		begin.setRadius(SIZE);
		begin.setOutlineThickness(-SIZE / 8);
		click_first = obj->click_b;
	}
	else if (obj->type[0] == 'L') {
		type = "L";

		begin.setPosition(obj->b_coord.first - SIZE, obj->b_coord.second - SIZE);
		begin.setRadius(SIZE);
		begin.setOutlineThickness(-SIZE / 8);
		end.setPosition(obj->e_coord.first - SIZE, obj->e_coord.second - SIZE);
		end.setRadius(SIZE);
		end.setOutlineThickness(-SIZE / 8);
		dist = Dist({ begin.getPosition().x, begin.getPosition().y }, { end.getPosition().x, end.getPosition().y });
		al = std::atan2(end.getPosition().x - begin.getPosition().x, end.getPosition().y - begin.getPosition().y) * 180/3.14 + 90;
		int alpha = 1, beta = 1;
		float dy = obj->b_coord.second - obj->e_coord.second;
		float dx = obj->b_coord.first - obj->e_coord.first;
		float sinb, cosb;
		//convex.setOutlineThickness(-SIZE / 8);
		convex.setPointCount(5);
		if (dx == 0) {
			alpha = 1; beta = 0; sinb = 0; cosb = 1;
		}
		else if (dy == 0) {
			alpha = 0; beta = 1; sinb = 1; cosb = 0;
		}
		else {
			sinb = sqrtf(1 / (1 + pow((dy / dx), 2)));
			cosb = sqrtf(1 - pow(sinb, 2));
			if ((dx > 0) && (dy > 0) || (dx < 0) && (dy < 0)) { // I || III
				alpha = -1; beta = 1;
			}
			else if ((dx < 0) && (dy > 0) || (dx > 0) && (dy < 0)) { //II || IV
				alpha = 1; beta = 1;
			}
		}
		convex.setPoint(0, sf::Vector2f(obj->b_coord.first + alpha * SIZE * cosb, obj->b_coord.second + beta * SIZE * sinb));
		convex.setPoint(1, sf::Vector2f(obj->e_coord.first + alpha * SIZE * cosb, obj->e_coord.second + beta * SIZE * sinb));
		convex.setPoint(2, sf::Vector2f(obj->e_coord.first - alpha * SIZE * cosb, obj->e_coord.second - beta * SIZE * sinb));
		convex.setPoint(3, sf::Vector2f(obj->b_coord.first - alpha * SIZE * cosb, obj->b_coord.second - beta * SIZE * sinb));
		convex.setPointCount(4);
		if (obj->type.size() == 2) {
			is_return = true;
			click_middle = obj->click_m;
			type = "LR";
		}
		click_first = obj->click_b;
		click_second = obj->click_e;
	}
	else if (obj->type[0] == 'C') {
		type = "C";
		R = Dist(obj->b_coord, obj->r_coord);
		begin.setPosition(obj->b_coord.first - SIZE, obj->b_coord.second - SIZE);
		begin.setRadius(SIZE);
		begin.setOutlineThickness(-SIZE / 8);
		end.setPosition(obj->e_coord.first - SIZE, obj->e_coord.second - SIZE);
		end.setRadius(SIZE);
		end.setOutlineThickness(-SIZE / 8);

		Triangle cur(Aff(obj->b_coord, obj->r_coord), Aff(obj->m_coord, obj->r_coord), Aff(obj->e_coord, obj->r_coord));
		Evaluate_Tri_Angl(cur, obj->r_coord);

		int count = 0;
		//convex.setOutlineThickness(-SIZE / 8);
		while (cur.first - cur.second > 1.57) {
			cur.first -= 3.14;
		}
		while (cur.second - cur.first > 1.57) {
			cur.first += 3.14;
		}
		while (cur.third - cur.second > 1.57) {
			cur.third -= 3.14;
		}
		while (cur.second - cur.third > 1.57) {
			cur.third += 3.14;
		}
		R_point = obj->r_coord;
		angle_first = cur.first;
		angle_second = cur.third;
		convex.setPointCount((int)abs((cur.first - cur.third) * 10) * 2 + 2);
		if (cur.first > cur.second) {
			for (float i = cur.first; i > cur.third; i -= 0.1) {
				convex.setPoint(count, sf::Vector2f(obj->r_coord.first + (R + SIZE) * cos(i), obj->r_coord.second + (R + SIZE) * sin(i)));
				count++;
			}
			//convex.setPoint(count, sf::Vector2f(obj->r_coord.first + (R + SIZE) * cos(cur.third), obj->r_coord.second + (R + SIZE) * sin(cur.third)));
			//count++;
			for (float i = cur.third; i < cur.first; i += 0.1) {
				convex.setPoint(count, sf::Vector2f(obj->r_coord.first + (R - SIZE) * cos(i), obj->r_coord.second + (R - SIZE) * sin(i)));
				count++;
			}
			//convex.setPoint(count, sf::Vector2f(obj->r_coord.first + (R - SIZE) * cos(cur.first), obj->r_coord.second + (R - SIZE) * sin(cur.first)));
			//count++;
		}
		else {
			for (float i = cur.first; i < cur.third; i += 0.1) {
				convex.setPoint(count, sf::Vector2f(obj->r_coord.first + (R + SIZE) * cos(i), obj->r_coord.second + (R + SIZE) * sin(i)));
				count++;
			}
			//convex.setPoint(count, sf::Vector2f(obj->r_coord.first + (R + SIZE) * cos(cur.third), obj->r_coord.second + (R + SIZE) * sin(cur.third)));
			//count++;
			for (float i = cur.third; i > cur.first; i -= 0.1) {
				convex.setPoint(count, sf::Vector2f(obj->r_coord.first + (R - SIZE) * cos(i), obj->r_coord.second + (R - SIZE) * sin(i)));
				count++;
			}
			//convex.setPoint(count, sf::Vector2f(obj->r_coord.first + (R - SIZE) * cos(cur.first), obj->r_coord.second + (R - SIZE) * sin(cur.first)));
			//count++;
		}
		convex.setPointCount(count);
		if (obj->type.size() == 2) {
			is_return = true;
			click_middle = obj->click_m;
type = "CR";
		}
		click_first = obj->click_b;
		click_second = obj->click_e;
	}
}

Triangle::Triangle(pa B, pa M, pa E) {
	b = B; m = M; e = E;
}

void Evaluate_Tri_Angl(Triangle& tri, std::pair<int, int> R) {
	R.first;
	R.second;
	float r = Dist(tri.b.first, R);
	R.first += r;
	float rb, rm, re;
	rb = Dist(tri.b.first, R);
	rm = Dist(tri.m.first, R);
	re = Dist(tri.e.first, R);
	tri.first = acos(1 - rb * rb / (2 * r*r));
	if (tri.b.first.second < R.second) {
		tri.first *= -1;
	}
	tri.second = acos(1 - rm * rm / (2 * r*r));
	if (tri.m.first.second < R.second) {
		tri.second *= -1;
	}
	tri.third = acos(1 - re * re / (2 * r*r));
	if (tri.e.first.second < R.second) {
		tri.third *= -1;
	}
}

Result::Result() {
	thirty = hundred = fifty = miss = 0;
}

pa Aff(std::pair<int, int> lhs, std::pair<int, int> rhs) {
	int alpha = 1, beta = 1;
	float dy = rhs.second - lhs.second;
	float dx = rhs.first - lhs.first;
	float cosa = sqrtl(1 / (1 + pow((dy / dx), 2)));
	float sina = sqrtf(1 - pow(cosa, 2));

	if ((dx >= 0) && (dy > 0)) { // I 
		alpha = 1; beta = 1;
	}
	else if ((dx < 0) && (dy >= 0)) { //II
		alpha = -1; beta = 1;
	}
	else if ((dx <= 0) && (dy < 0)) { //III
		alpha = -1; beta = -1;
	}
	else if ((dx > 0) && (dy <= 0)) { //IV
		alpha = 1; beta = -1;
	}
	return { { lhs.first + alpha * SIZE * cosa, lhs.second + beta * SIZE * sina},
	 { lhs.first - alpha * SIZE * cosa, lhs.second - beta * SIZE * sina} };
}

void Beatmap(Song& beatmap, sf::RenderWindow& window) {
	sf::Font font;
	font.loadFromFile("Verdana.ttf");
	sf::Text text("", font, 40);
	sf::Text combo_text("", font, 40);
	sf::Text percentage("", font, 40);
	sf::Text end("", font, 80);
	text.setColor(sf::Color::White);
	combo_text.setColor(sf::Color::White);
	percentage.setColor(sf::Color::White);
	text.setPosition(10, 10);
	combo_text.setPosition(10, 60);
	percentage.setPosition(10, 110);


	Result result;
	int score = 0, combo = 0;

	window.setMouseCursorVisible(false);
	window.clear();

	sf::Texture temp1;
	temp1.loadFromFile("images/add/cursor.png");
	sf::Texture temp2, temp3;
	temp2.loadFromFile("images/add/ball.png");
	temp2.setSmooth(true);
	temp3.loadFromFile("images/add/reverse.png");
	temp3.setSmooth(true);
	arrow.setTexture(temp3);
	arrow.setOrigin(64, 64);
	cursor.setTexture(temp1);
	cursor.setOrigin(cursor.getTexture()->getSize().x / 2, cursor.getTexture()->getSize().y / 2);
	if (ball.size() == 0) {
		ball.resize(10);
		for (int i = 0; i < 10; i++) {
			ball[i].setTexture(temp2);
			ball[i].setTextureRect(sf::IntRect(i * 118, 0, 118, 118));
			ball[i].setScale(sf::Vector2f(160.0f / 118, 160.0f / 118));
		}
	}
	sf::Clock clock;
	sf::Music music;
	sf::Event event;
	std::deque<Beat_OBJ> drawable;
	std::deque<std::shared_ptr<Object>> clickable;
	music.openFromFile("songs/" + beatmap.name + "/audio.wav");
	music.setVolume(100);
	beatmap.Load_from_dir();
	beatmap.BG.setScale(1920.0 / beatmap.bg.getSize().x, 1080.0 / beatmap.bg.getSize().y);
	beatmap.BG.setTexture(beatmap.bg);
	beatmap.BG.setPosition(0, 0);
	int current = 0;
	float total_time = -2000;
	while (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		window.clear();
		window.draw(beatmap.BG);
		cursor.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			cursor.setScale({ 1.5f, 1.5f });
		}
		else {
			cursor.setScale({ 1.0f, 1.0f });
		}
		window.draw(cursor);
		window.display();
	}
	clock.restart();
	//music.setPlayingOffset(sf::Time(sf::milliseconds (-2000000) ));
	music.play();
	float accuracy = 0;
	while (music.getStatus() != sf::SoundSource::Status::Stopped) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			beatmap.Delete_from_RAM();
			return;
		}
		total_time = music.getPlayingOffset().asMilliseconds();
		while (current < beatmap.song.size() && beatmap.song[current]->click_b - total_time <= 1500) {
			clickable.push_back(beatmap.song[current]);
			drawable.push_back(Beat_OBJ(beatmap.song[current]));
			current++;
		}
		while (!clickable.empty() && clickable.front()->click_b - total_time < -300) {
			if (clickable.front()->type != "D") {
				if (clickable.front()->click_e - total_time >= -300) {
					break;
				}
			}
			result.miss++;
			combo = 0;
			clickable.pop_front();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!clickable.empty()) {
				auto it = clickable.front();
				auto pos = sf::Mouse::getPosition(window);
				if (it->type == "D") {
					if (pow((pos.x - it->b_coord.first), 2) + pow((pos.y - it->b_coord.second), 2) < pow(SIZE, 2)) {
						if (abs(total_time - it->click_b) < 150) {
							result.thirty++;
							combo++;
							score += 300 * combo;
						}
						else if (abs(total_time - it->click_b) < 200) {
							result.hundred++;
							combo++;
							score += 100 * combo;
						}
						else if (abs(total_time - it->click_b) < 300) {
							result.fifty++;
							combo++;
							score += 50 * combo;
						}
						else {
							combo = 0;
							result.miss++;
						}
						clickable.pop_front();
					}
				}
				else if (it->type == "L" || it->type == "C") {
					if (it->stage == 0) {
						if (pow((pos.x - it->b_coord.first), 2) + pow((pos.y - it->b_coord.second), 2) < pow(SIZE, 2)) {
							if (abs(total_time - it->click_b) < 300) {
								it->result += 10;
								it->stage = 1;
							}
						}
					}
					if (it->stage == 1) {
						if (abs(total_time - it->click_e) < 20) {
							if (pow((pos.x - it->e_coord.first), 2) + pow((pos.y - it->e_coord.second), 2) < pow(2 * SIZE, 2)) {
								it->result += 1;
								it->stage = 2;
							}
						}
						else if (total_time > it->click_e + 20) {
							it->stage++;
						}
					}
					if (it->stage == 2) {
						if (it->result == 11) {
							result.thirty++;
							combo++;
							score += 300 * combo;
						}
						else if (it->result == 10) {
							result.hundred++;
							combo++;
							score += 100 * combo;
						}
						else if (it->result == 1) {
							result.fifty++;
							combo++;
							score += 50 * combo;
						}
						else {
							combo = 0;
							result.miss++;
						}
						clickable.pop_back();
					}
				}
				else if (it->type == "LR" || it->type == "CR") {
					if (it->stage == 0) {
						if (pow((pos.x - it->b_coord.first), 2) + pow((pos.y - it->b_coord.second), 2) < pow(SIZE, 2)) {
							if (abs(total_time - it->click_b) < 300) {
								it->result += 1;
								it->stage++;
							}
						}
					}
					if (it->stage == 1) {
						if (abs(total_time - it->click_m) < 20) {
							if (pow((pos.x - it->e_coord.first), 2) + pow((pos.y - it->e_coord.second), 2) < pow(2 * SIZE, 2)) {
								it->result += 1;
								it->stage++;
							}
						}
						else if (total_time > it->click_e + 20) {
							it->stage++;
						}
					}
					if (it->stage == 2) {
						if (abs(total_time - it->click_e) < 300) {
							if (pow((pos.x - it->b_coord.first), 2) + pow((pos.y - it->b_coord.second), 2) < pow(2 * SIZE, 2)) {
								it->result += 1;
								it->stage = 3;
							}
						}
						else if (total_time > it->click_e + 20) {
							it->stage = 3;
						}
					}
					if (it->stage == 3) {
						if (it->result == 3) {
							result.thirty++;
							combo++;
							score += 300 * combo;
						}
						else if (it->result == 2) {
							result.hundred++;
							combo++;
							score += 100 * combo;
						}
						else if (it->result == 1) {
							result.fifty++;
							combo++;
							score += 50 * combo;
						}
						else {
							combo = 0;
							result.miss++;
						}
						clickable.pop_back();
					}
				}
			}
			combo_text.setString("Your combo:" + std::to_string(combo) + "x");
			text.setString("Your score: " + std::to_string(score));
			if (result.miss + result.thirty + result.hundred + result.fifty == 0) {
				accuracy = 100.00;
			}
			else {
				accuracy = (float)(300 * result.thirty + 100 * result.hundred + 50 * result.fifty ) * 100 / (300 * (result.miss + result.thirty + result.hundred + result.fifty));
			}
			percentage.setString(std::to_string(accuracy)+ "%");
		}
		window.clear();
		window.draw(beatmap.BG);
		window.draw(text);
		window.draw(combo_text);
		window.draw(percentage);
		Draw(drawable, total_time, window, beatmap.BG);
	}
	if (accuracy == 100) {
		end.setString("Congratulations! Your rank is SS!!!");
	}
	else if (((float)result.thirty / (result.miss + result.thirty + result.hundred + result.fifty)) > 0.9) {
		end.setString("Congratulations! Your rank is S!!");
	}
	else if (((float)result.thirty / (result.miss + result.thirty + result.hundred + result.fifty)) > 0.8 && result.miss == 0
		|| ((float)result.thirty / (result.miss + result.thirty + result.hundred + result.fifty)) > 0.9 && result.miss != 0) {
		end.setString("Congratulations! Your rank is A!");
	}
	else if (((float)result.thirty / (result.miss + result.thirty + result.hundred + result.fifty)) > 0.7 && result.miss == 0
		|| ((float)result.thirty / (result.miss + result.thirty + result.hundred + result.fifty)) > 0.8 && result.miss != 0) {
		end.setString("Congratulations! Your rank is B");
	}
	else if (((float)result.thirty / (result.miss + result.thirty + result.hundred + result.fifty)) > 0.6) {
		end.setString("Congratulations! Your rank is C");
	}
	else {
		end.setString("Congratulations! Your rank is D??????");
	}
	text.setColor(sf::Color::White);
	percentage.setColor(sf::Color::White);
	combo_text.setColor(sf::Color::White);
	end.setColor(sf::Color::White);

	text.setPosition(800, 180);
	combo_text.setPosition(800, 230);
	percentage.setPosition(800, 280);
	end.setPosition(300, 100);
	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window.clear();
		window.draw(end);
		window.draw(text);
		window.draw(combo_text);
		window.draw(percentage);
		window.display();
	}
	beatmap.Delete_from_RAM();	
	return;
}

void Draw(std::deque<Beat_OBJ>& drawable, float time, sf::RenderWindow& window, sf::Sprite bg) {
	int outdated = 0;
	sf::Sprite arr = arrow;
	sf::CircleShape app;
	for (auto it = drawable.rbegin(); it != drawable.rend(); it++) {
		auto x = *it;
		if (time < x.click_first && time > x.click_first - 650) {
			float size = 300 - (float)(650 + time - x.click_first) * 220 / 650;
			app.setRadius(size);
			app.setPosition(x.begin.getPosition().x - size + SIZE, x.begin.getPosition().y - size + SIZE);
			app.setFillColor({ 0, 0, 0, 0 });
			app.setOutlineColor({ 255, 255, 255, 255 });
			app.setOutlineThickness(-SIZE / 8);
		}
		if (x.type == "D") {
			if (time - x.click_first > 700) {
				outdated++;
				continue;
			}
			app.setOutlineColor({ 0,255,0, opacity(time - x.click_first) });
			x.begin.setFillColor({ 255,0,0, opacity(time - x.click_first) });
			x.begin.setOutlineColor({ 255,255,255, opacity(time - x.click_first) });
			window.draw(x.begin);
			window.draw(app);
		}
		else if (x.type[0] == 'L') {
			if (time - x.click_second > 700) {
				outdated++;
				continue;
			}
			int i = (time > x.click_first) ? (int)((time - x.click_first) / 50) % 10 : 0;
			app.setOutlineColor({ 0,255,0, opacity(time - x.click_first) });
			x.begin.setFillColor({ 255,0,0, opacity(time - x.click_first, time - x.click_second) });
			x.begin.setOutlineColor({ 255,255,255, opacity(time - x.click_first, time - x.click_second) });
			x.end.setFillColor({ 255,0,0, opacity(time - x.click_first, time - x.click_second) });
			x.end.setOutlineColor({ 255,255,255, opacity(time - x.click_first, time - x.click_second) });
			x.convex.setOutlineColor({ 255,255,255, opacity(time - x.click_first, time - x.click_second) });
			x.convex.setFillColor({ 255,0,0, opacity(time - x.click_first, time - x.click_second) });
			ball[i].setOrigin(sf::Vector2f{ 59, 59 });
			ball[i].setRotation(-x.al);
			ball[i].setColor({ 255,255,255, opacity_ball(time - x.click_first, time - x.click_second) });
			if (x.type == "L") {
				arrow.setColor({ 255, 255, 255, 0 });
				ball[i].setPosition(sf::Vector2f{ (float)(x.begin.getPosition().x + SIZE + (time - x.click_first) / (x.click_second - x.click_first) * x.dist * sin((x.al - 90) * 3.14 / 180)),
				(float)(x.begin.getPosition().y + SIZE + (time - x.click_first) / (x.click_second - x.click_first) * x.dist * cos((x.al - 90) * 3.14 / 180)) });
			}
			else {
				arrow.setColor({ 255, 255, 255, opacity(time - x.click_first, time - x.click_second) });
				arrow.setPosition(x.end.getPosition().x + SIZE, x.end.getPosition().y + SIZE);
				arrow.setRotation(-x.al);
				if (time <= x.click_middle) {
					ball[i].setPosition(sf::Vector2f{ (float)(x.begin.getPosition().x + SIZE + (time - x.click_first) / (x.click_middle - x.click_first) * x.dist * sin((x.al - 90) * 3.14 / 180)),
					(float)(x.begin.getPosition().y + SIZE + (time - x.click_first) / (x.click_middle - x.click_first) * x.dist * cos((x.al - 90) * 3.14 / 180)) });
				}
				else {
					ball[i].setPosition(sf::Vector2f{ (float)(x.end.getPosition().x + SIZE - (time - x.click_middle) / (x.click_second - x.click_middle) * x.dist * sin((x.al - 90) * 3.14 / 180)),
					(float)(x.end.getPosition().y + SIZE - (time - x.click_middle) / (x.click_second - x.click_middle) * x.dist * cos((x.al - 90) * 3.14 / 180)) });
				}
			}
			window.draw(x.convex);
			window.draw(x.begin);
			window.draw(x.end);
			window.draw(app);
			window.draw(arr);
			window.draw(ball[i]);
		}
		else if (x.type[0] == 'C') {
			int i = (time > x.click_first) ? (int)((time - x.click_first) / 50) % 10 : 0;
			ball[i].setOrigin(sf::Vector2f{ 59, 59 });
			if (time - x.click_second > 700) {
				outdated++;
				continue;
			}
			if (x.type == "CR") {
				arrow.setColor({ 255, 255, 255, opacity(time - x.click_first, time - x.click_second) });
				arrow.setPosition(x.end.getPosition().x + SIZE, x.end.getPosition().y + SIZE);
				arrow.setRotation(90-x.angle_second);
				if (time <= x.click_middle) {
					ball[i].setPosition(x.R_point.first + x.R*cos(x.angle_first - (time - x.click_first) / (x.click_middle - x.click_first) * (x.angle_first - x.angle_second)),
						x.R_point.second + x.R*sin(x.angle_first - (time - x.click_first) / (x.click_middle - x.click_first) * (x.angle_first - x.angle_second)));
					ball[i].setRotation(90 + (x.angle_first - (time - x.click_first) / (x.click_middle - x.click_first) * (x.angle_first - x.angle_second)) * 180.0 / 3.14);
				}
				else {
					ball[i].setPosition(x.R_point.first + x.R*cos(x.angle_second + (time - x.click_middle) / (x.click_second - x.click_middle) * (x.angle_first - x.angle_second)),
						x.R_point.second +x.R*sin(x.angle_second + (time - x.click_middle) / (x.click_second - x.click_middle) * (x.angle_first - x.angle_second)));
					ball[i].setRotation(90 + (x.angle_second + (time - x.click_middle) / (x.click_second - x.click_middle) * (x.angle_first - x.angle_second)) * 180.0 / 3.14);
				}
			}
			else {
				arrow.setColor({ 255, 255, 255, 0 });
				ball[i].setPosition(x.R_point.first + x.R*cos(x.angle_first - (time - x.click_first) / (x.click_second - x.click_first) * (x.angle_first - x.angle_second)),
					x.R_point.second + x.R*sin(x.angle_first - (time - x.click_first) / (x.click_second - x.click_first) * (x.angle_first - x.angle_second)));
				ball[i].setRotation(90 + (x.angle_first - (time - x.click_first) / (x.click_second - x.click_first) * (x.angle_first - x.angle_second)) * 180.0 / 3.14);
			}
			app.setOutlineColor({ 0,255,0, opacity(time - x.click_first) });
			x.begin.setFillColor({ 255,0,0, opacity(time - x.click_first, time - x.click_second) });
			x.begin.setOutlineColor({ 255,255,255, opacity(time - x.click_first, time - x.click_second) });
			x.end.setFillColor({ 255,0,0, opacity(time - x.click_first, time - x.click_second) });
			x.end.setOutlineColor({ 255,255,255, opacity(time - x.click_first, time - x.click_second) });
			x.convex.setOutlineColor({ 255,255,255, opacity(time - x.click_first, time - x.click_second) });
			x.convex.setFillColor({ 255,0,0, opacity(time - x.click_first, time - x.click_second) });
			ball[i].setColor({ 255,255,255, opacity_ball(time - x.click_first, time - x.click_second) });
			window.draw(x.convex);
			window.draw(x.begin);
			window.draw(x.end);
			window.draw(app);
			window.draw(arr);
			window.draw(ball[i]);
		}
	}
	cursor.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		cursor.setScale({ 1.5f, 1.5f });
	}
	else {
		cursor.setScale({ 1.0f, 1.0f });
	}
	window.draw(cursor);
	for (; outdated > 0; outdated--) {
		drawable.pop_front();
	}
	
	window.display();
}

sf::Uint8 opacity(float time_r) {
	if (time_r < -650) {
		return 0;
	}
	else if (time_r < -280) {
		return (int)((float)((time_r + 650 ) / 135 + 0,5) * 255);
	}
	else if (time_r < 0) {
		return 255;
	}
	else if (time_r < 450) {
		return (int)((float)(450 - time_r) / 450 * 255);
	}
	else {
		return 0;
	}
}

sf::Uint8 opacity(float b_time, float e_time) {
	if (b_time < -650) {
		return 0;
	}
	else if (b_time < -280) {
		return (int)((float)((b_time + 650) / 135 + 0, 5) * 255);
	}
	else if (e_time < 0) {
		return 255;
	}
	else if (e_time < 450) {
		return (int)((float)(450 - e_time) / 450 * 255);
	}
	else {
		return 0;
	}
}

sf::Uint8 opacity_ball(float b_time, float e_time) {
	if (b_time < 0 || e_time > 0) {
		return 0;
	}
	else {
		return 255;
	}
}