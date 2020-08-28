#include "Song.h"
#include <deque>
using pa = std::pair<std::pair<int, int>, std::pair<int, int>>;
class Beat_OBJ {
public:
	std::string type;
	bool is_return = false;
	sf::CircleShape begin, end;
	sf::ConvexShape convex;
	std::pair<int, int> R_point;
	float angle_first, angle_second;
	float click_first, click_second, click_middle;
	float R, dist;
	float al;
	Beat_OBJ(std::shared_ptr<Object> obj);
};
struct Triangle {
	float first, second, third;
	pa b, m, e;
	Triangle(pa B, pa M, pa E);
};
struct Result {
	int thirty, hundred, fifty, miss;
	Result();
};
void Evaluate_Tri_Angl(Triangle& tri, std::pair<int, int> R);
void Beatmap(Song& beatmap, sf::RenderWindow& window);
std::pair<std::pair<int, int>, std::pair<int, int>> Aff(std::pair<int, int> lhs, std::pair<int, int> rhs);
void Draw(std::deque<Beat_OBJ>& drawable, float time, sf::RenderWindow& window, sf::Sprite bg);
sf::Uint8 opacity(float time);
sf::Uint8 opacity(float b_time, float e_time);
sf::Uint8 opacity_ball(float b_time, float e_time);