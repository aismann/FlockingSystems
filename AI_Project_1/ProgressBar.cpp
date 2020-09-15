#include "ProgressBar.h"
#include "Math.h"

const float ProgressBar::HEIGHT = 10.f;

ProgressBar::ProgressBar(sf::Vector2f _pos, float _width, sf::Color _color):
	width(_width),
	pos(_pos)
{

	// Vertex math
	this->v_frame[0].position = _pos + sf::Vector2f(0.f, -HEIGHT);
	this->v_frame[1].position = _pos + sf::Vector2f(_width, -HEIGHT);
	this->v_frame[2].position = _pos + sf::Vector2f(_width, HEIGHT);
	this->v_frame[3].position = _pos + sf::Vector2f(0.f, HEIGHT);

	this->computeFill(1.0f);

	// Color
	for (int i = 0; i < 4; ++i) {
		this->v_fill[i].color = _color;
		this->v_frame[i].color = _color;
	}
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::onDraw(sf::RenderTarget& _target, sf::RenderStates _state)
{
	_target.draw(&v_frame[0], 4, sf::LineStrip, _state);
	_target.draw(&v_fill[0], 4, sf::Quads, _state);
}

void ProgressBar::setFillLevel(float _lvl)
{
	_lvl = fe::math::max(0.f, _lvl);
	_lvl = fe::math::min(1.f, _lvl);

	computeFill(_lvl);
}

void ProgressBar::computeFill(float _fill)
{
	this->v_fill[0].position = pos + sf::Vector2f(0.f, -HEIGHT);
	this->v_fill[1].position = pos + sf::Vector2f(_fill * width, -HEIGHT);
	this->v_fill[2].position = pos + sf::Vector2f(_fill * width, HEIGHT);
	this->v_fill[3].position = pos + sf::Vector2f(0.f, HEIGHT);
}
