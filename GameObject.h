#pragma once

/*
// ��ʾ��Ϸ�еĶ���

#include <SDL.h>


// û��ʹ��������ʾ��Ϸ����ֻʹ��һ���ṹ������ʾ��Ϸ�������洢��Ϸ������ٶȺ�λ��


struct Entity
{
	SDL_Rect screen_location;		// ����Ļ�ϵ�λ��
	SDL_Rect geometry;				// ���������(��С)
	SDL_Texture* texture;			// ����

	int x_speed;					// x�����ϵ��ٶ�
	int y_speed;					// y�����ϵ��ٶ�
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
	// ������Ϸ��������Ļ�ϵ�λ��
	void setLocation(SDL_Rect& location);
	void setLocation_XY(int x, int y);
	// �����������ԣ���С��
	void setGeometry(SDL_Rect& geometry);
	void setGeometry(int width, int height);
	// ��������
	void setTexture(SDL_Texture* texture);
	// ������Ϸ������ٶ�
	void setSpeed_x(int x_speed);
	void setSpeed_y(int y_speed);

public:
	SDL_Rect getLocation() const;
	SDL_Rect getGeometry() const;
	SDL_Texture* getTexture() const;
	int getSpeed_x() const;
	int getSpeed_y() const;
	// �������������ݷ����޸�
	SDL_Rect& GainLocation();

private:
	SDL_Rect screen_location;		// ����Ļ�ϵ�λ��
	SDL_Rect geomotry;				// �������ԣ���С��
	SDL_Texture* texture;			// ����

	int x_speed;					// x�����ϵ��ٶ�
	int y_speed;					// y�����ϵ��ٶ�
};

