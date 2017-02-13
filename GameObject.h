#pragma once

/*
// 表示游戏中的对象

#include <SDL.h>


// 没有使用类来表示游戏对象，只使用一个结构体来表示游戏对象，来存储游戏对象的速度和位置


struct Entity
{
	SDL_Rect screen_location;		// 在屏幕上的位置
	SDL_Rect geometry;				// 自身的属性(大小)
	SDL_Texture* texture;			// 纹理

	int x_speed;					// x方向上的速度
	int y_speed;					// y方向上的速度
};
typedef struct Entity ENTITY;
*/

#include <SDL.h>

class GameObject
{
public:
	GameObject(void);
	~GameObject(void);

public:
	// 设置游戏对象在屏幕上的位置
	void setLocation(SDL_Rect& location);
	void setLocation_XY(int x, int y);
	// 设置自身属性（大小）
	void setGeometry(SDL_Rect& geometry);
	void setGeometry(int width, int height);
	// 设置纹理
	void setTexture(SDL_Texture* texture);
	// 设置游戏对象的速度
	void setSpeed_x(int x_speed);
	void setSpeed_y(int y_speed);

public:
	SDL_Rect getLocation() const;
	SDL_Rect getGeometry() const;
	SDL_Texture* getTexture() const;
	int getSpeed_x() const;
	int getSpeed_y() const;
	// 返回引用型数据方便修改
	SDL_Rect& GainLocation();

private:
	SDL_Rect screen_location;		// 在屏幕上的位置
	SDL_Rect geomotry;				// 自身属性（大小）
	SDL_Texture* texture;			// 纹理

	int x_speed;					// x方向上的速度
	int y_speed;					// y方向上的速度
};

