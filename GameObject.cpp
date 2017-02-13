#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject(void):
	texture(nullptr)
{
	x_speed = 0;
	y_speed = 0;
}

void GameObject::setLocation(SDL_Rect& location)
{
	screen_location = location;
}

void GameObject::setLocation_XY(int x, int y)
{
	screen_location.x = x;
	screen_location.y = y;
}

void GameObject::setGeometry(SDL_Rect& geometry)
{
	this->geomotry = geometry;
}

void GameObject::setGeometry(int width, int height)
{
	SDL_Rect rect = {
		0, 0, width, height
	};
	this->geomotry = rect;
}

void GameObject::setTexture(SDL_Texture* texture)
{
	this->texture = texture;
	// �����������ԣ���С��
	if (texture != nullptr)
	{
		// ��ȡ������Ϣ
		int width = 0, height = 0;
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
		/*
		SDL_Rect rect = {
			0, 0, width, height
		};
		setGeometry(rect);
		*/
		setGeometry(width, height);
	}
}

void GameObject::setSpeed_x(int x_speed)
{
	this->x_speed = x_speed;
}

void GameObject::setSpeed_y(int y_speed)
{
	this->y_speed = y_speed;
}

SDL_Rect GameObject::getLocation() const
{
	return this->screen_location;
}

SDL_Rect GameObject::getGeometry() const
{
	return this->geomotry;
}

// ���������������ݷ����޸�
SDL_Rect& GameObject::GainLocation()
{
	return this->geomotry;
}

SDL_Texture* GameObject::getTexture() const
{
	return this->texture;
}

int GameObject::getSpeed_x() const
{
	return x_speed;
}

int GameObject::getSpeed_y() const
{
	return y_speed;
}

GameObject::~GameObject(void)
{
	if (texture)
		SDL_DestroyTexture(texture);
}
