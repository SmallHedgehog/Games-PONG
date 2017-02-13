#pragma once

/*
����Window����Ҫ���𴰿ڵĴ�������ʾ����£�������Դ
*/

#include <SDL_image.h>
#include <iostream>
#include "GameObject.h"
#include "Defines.h"
#include "Enums.h"

using namespace std;

class Window
{
public:
	Window(void);
	~Window(void);

public:
	void Start();				// ������Ϣѭ��
	void Refresh();				// refresh����Ļ��
	void Update();				// ����ͼ����Ϣ
	void LoadResouceFile();		// ������Դ�ļ�
	SDL_Texture* Load_image(std::string FilePath);	// ����ͼ��
	void Init();				// ��ʼ����Ϸ����

// ��Ϸ�߼�
public:
	void MoveBall();
	bool CheckWallCollisions(GameObject& Object, Directions dir);

private:
	SDL_Window* win;			// ����
	SDL_Renderer* renderer;		// ��Ⱦ��
	SDL_Texture* bg;			// ��������

	GameObject Ball;			// С��
	GameObject PaddleC;			// �壨���ԣ�
	GameObject PaddleP;			// �壨��ң�

	bool IsStarted;				// ��Ϸ�Ƿ�ʼ
};

