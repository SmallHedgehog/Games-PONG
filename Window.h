#pragma once

/*
����Window����Ҫ���𴰿ڵĴ�������ʾ����£�������Դ
*/

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <time.h>
#include <math.h>
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
	void ResetGame();			// ������Ϸ
	SDL_Texture* Draw_text(std::string message);	// ��������

// ��Ϸ�߼�
public:
	// �ƶ�С��
	void MoveBall();
	// ������Ƿ������Wall
	bool CheckWallCollisions(GameObject& Object, Directions dir);
	// ������Ƿ����ײ����
	bool CheckBallCollisions(GameObject& Object);
	// �ƶ�С���������ײ�¼��Ĵ����߼�����������������ı�С���ٶȵ�
	void HandleBall();
	// ������ԵĽ���ʹ֮����
	void HandleComputerPaddle();
	// ������ҷ���
	void HandlePlayerScore();
	// ������Է���
	void HandleComputerScore();

private:
	SDL_Window* win;			// ����
	SDL_Renderer* renderer;		// ��Ⱦ��
	SDL_Texture* bg;			// ��������
	TTF_Font* Font;				// ����

	GameObject Ball;			// С��
	GameObject PaddleC;			// �壨���ԣ�
	GameObject PaddleP;			// �壨��ң�

	bool IsStarted;				// ��Ϸ�Ƿ�ʼ
	bool Pause;					// ��ͣ��Ϸ

	int PlayerScore;			// ��ҷ���
	int ComputerScore;			// ���Է���
};

