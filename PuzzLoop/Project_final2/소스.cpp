#include <iostream>
#include <vector>
#include <string>
#include <ctime> // 난수 생성용
#include <windows.h> //delay 넣고 싶은 때 Sleep() 사용 ex) Sleep(1000) 1초 delay

#pragma comment(lib,"winmm")
#include <mmsystem.h> // // 2022-11-24 수정 sound 삽입용

#include "Sphere.h"
#include "Light.h"
#include "Cannon.h"
#include "Attackmarble.h"
#include "Texture.h"


using namespace std;

#define WINDOW_X 200
#define WINDOW_Y 200

#define WINDOW_WIDTH 1000		// window's width
#define WINDOW_HEIGHT 640		// window's height

#define boundaryX (WINDOW_WIDTH)/2
#define boundaryY (WINDOW_HEIGHT)/2
#define RADIUS 20.0f

// 2022-11-24 수정 sound 추가 경로
#define SOUND_FILE_NAME_START "C:/Users/User/source/repos/Project_final2/Project_final2/sound/start.wav"
#define SOUND_FILE_NAME_SHOOT "C:/Users/User/source/repos/Project_final2/Project_final2/sound/shoot3.wav"
#define SOUND_FILE_NAME_BOOM "C:/Users/User/source/repos/Project_final2/Project_final2/sound/boom.wav"
#define SOUND_FILE_NAME_WIN "C:/Users/User/source/repos/Project_final2/Project_final2/sound/win.wav"
#define SOUND_FILE_NAME_LOSE "C:/Users/User/source/repos/Project_final2/Project_final2/sound/lose.wav"

int score =0 ; // 2022-11-24 수정 scoreboard용

clock_t start_t = clock();
clock_t end_t;

vector<Sphere> spheres; // 3가지 공의 종류를 넣을 벡터
vector<Sphere> marbles; // 오른쪽으로 이동하는 구슬들

Attackmarble attackmarble; // spacebar를 누른 뒤 날라가는 공

Cannon cannon; // 캐논 
float shooting_angle; //캐논의 각도
int repositioned = 0; // 공이 다른 공에 충돌했을 때 reposition 됐는지 여부
int attacked_left_num; // attackmarble이 충돌하였을 때 왼쪽에 있는 공의 번호 (spheres vector의 index에 해당함)

Light light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);

//11.27 수정 repositioning할 때 이동하여야하는 speed 입력
float r_moving_speed[2];
int flag;

//2022-11-27 추가2
Texture texture;

//빛과 공 3가지 특징을 초기화 (구의 반지름 radius는 20으로 하였음)
void initialize() {
	light.setAmbient(1.0f, 1.0f, 1.0f, 1.0f);
	light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

	Material mtl1;
	mtl1.setEmission(0.1f, 0.1f, 0.1f, 1.0f);
	mtl1.setAmbient(0.4f, 0.4f, 0.1f, 1.0f);
	mtl1.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	mtl1.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl1.setShininess(10.0f);

	Material mtl2(mtl1), mtl3(mtl1);
	mtl2.setAmbient(0.1f, 0.4f, 0.4f, 1.0f);
	mtl3.setAmbient(0.4f, 0.1f, 0.4f, 1.0f);

	Sphere sphere1(20, 20, 20, 1);
	sphere1.setCenter(-boundaryX - RADIUS, 0.8 * boundaryY, 0.0f); 
	sphere1.setVelocity(0.4f, 0.0f, 0.0f);
	sphere1.setMTL(mtl1);
	spheres.push_back(sphere1);

	Sphere sphere2(sphere1);
	sphere2.setID(2);
	sphere2.setCenter(-boundaryX - RADIUS, 0.8 * boundaryY, 0.0f); 
	sphere2.setMTL(mtl2);
	spheres.push_back(sphere2);

	Sphere sphere3(sphere1);
	sphere3.setID(3);
	sphere3.setCenter(-boundaryX - RADIUS, 0.8 * boundaryY, 0.0f); 
	sphere3.setVelocity(0.4f, 0.0f, 0.0f);
	sphere3.setMTL(mtl3);
	spheres.push_back(sphere3);
	
	srand(time(0));
	//marbles vector에 들어갈 구 10개
	for (int i = 0; i < 10; i++) {
		Sphere sphererandom(spheres[rand() % 3]);
		marbles.push_back(sphererandom);
	}

	//캐논에 들어갈 초기 구 2개 설정
	Sphere sphererandom1(spheres[rand() % 3]); Sphere sphererandom2(spheres[rand() % 3]); 
	sphererandom1.setCenter(0.0f, -260.0f, 0.0f); sphererandom2.setCenter(0.0f, -300.0f, 0.0f); 
	cannon.setCannon(sphererandom1, sphererandom2);

	//2022-11-27 추가3
	texture.initializeTexture("Puzzloop.png");

	PlaySound(TEXT(SOUND_FILE_NAME_START), NULL, SND_ASYNC | SND_ALIAS); // 2022-11-24 시작 효과음
}

void idle() {
	end_t = clock();

	int preprogress = 10;// 2022-11-24 수정 pre-progress 개수


	if (end_t - start_t > 1000 / 60) {
		if (marbles[0].getCenter()[0] + RADIUS >= boundaryX) exit(0); // 벽에 닿으면 게임 종료

		cannon.move(); // keyboard에 영향을 받은 angle을 반영해 움직임

		// 2022-11-24 수정 pre-progress 시작부분 
		//marbles[0].move(); // 가장 앞쪽에 있는 구슬은 항상 움직이게 설정

		if (repositioned == 0) {// repositioning 안 할 때 움직이도록 설정 11.27 수정 
			if (marbles[0].getCenter()[0] <= -boundaryX + RADIUS * 2 * (preprogress - 1) && marbles.size() == 10) {

				marbles[0].initialmove();

				for (unsigned int i = 0; i < marbles.size() - 1; i++) {
					//한 구슬이 완전히 모습을 드러내면 다음 구슬이 움직이게 설정, preprogress는 8배 빠른 initial move
					if (marbles[i].getCenter()[0] >= -boundaryX + RADIUS) {
						marbles[i + 1].initialmove();
					}
				}

			}
			else {
				for (unsigned int i = 0; i < marbles.size(); i++) {
					marbles[i].move();
				}
			}
		}
		// 2022-11-24 수정 pre-progress 끝부분

		//벽쪽에 attackmarble이 충돌하였을 때 attackmarble을 소멸하게 하고, spacebar를 누를 수 있도록 설정
		if (attackmarble.getEnable() && attackmarble.getCanmove()) attackmarble.move();
		if (attackmarble.getAttackmarble().getCenter()[0] > boundaryX ||
			attackmarble.getAttackmarble().getCenter()[1] > boundaryY ||
			attackmarble.getAttackmarble().getCenter()[0] < -boundaryX ||
			attackmarble.getAttackmarble().getCenter()[1] < -boundaryY) {
			attackmarble.setEnable(false); cannon.setCanshoot(true);
		}

		//10번에 나눠서 이동시키기 11.27 수정
		if (repositioned != 10) {
			for (unsigned int i = 0; i < marbles.size(); i++) {
				//충돌하였을 때
				if (pow((marbles[i].getCenter()[0] - attackmarble.getAttackmarble().getCenter()[0]), 2)
					+ pow((marbles[i].getCenter()[1] - attackmarble.getAttackmarble().getCenter()[1]), 2)
					< 1600.0) {
					attackmarble.setCanmove(false);
					//왼쪽 구에 충돌하였을 때
					if (marbles[i].getCenter()[0] <= attackmarble.getAttackmarble().getCenter()[0] ){// i == marbles.size() - 1) && i != 0) { // 2022-11-24 수정 i = marbles.size()-1, i = 0 경우 고려
						if (repositioned == 0) {
							r_moving_speed[0] = ((marbles[i].getCenter()[0] + RADIUS) - attackmarble.getAttackmarble().getCenter()[0]) / 10;
							r_moving_speed[1] = ((marbles[i].getCenter()[1] - RADIUS * sqrtf(3.0)) - attackmarble.getAttackmarble().getCenter()[1]) / 10;
						} 
						float next_pos[2] = { attackmarble.getAttackmarble().getCenter()[0] + r_moving_speed[0] ,
							attackmarble.getAttackmarble().getCenter()[1] + r_moving_speed[1] };

						attackmarble.getAttackmarble().setCenter(next_pos[0], next_pos[1], 0.0f); 

						repositioned++;
						attacked_left_num = i;
						break;
					}
					//오른쪽 구에 충돌하였을 때
					else {
						if (repositioned == 0 
							//&& i + 1 != marbles.size()
							) {
							r_moving_speed[0] = ((marbles[i].getCenter()[0] - RADIUS) - attackmarble.getAttackmarble().getCenter()[0]) / 10;
							r_moving_speed[1] = ((marbles[i].getCenter()[1] - RADIUS * sqrtf(3.0) ) - attackmarble.getAttackmarble().getCenter()[1]) / 10;
						}

						float next_pos[2] = { attackmarble.getAttackmarble().getCenter()[0] + r_moving_speed[0] ,
							attackmarble.getAttackmarble().getCenter()[1] + r_moving_speed[1] };
						attackmarble.getAttackmarble().setCenter(next_pos[0], next_pos[1], 0.0f); 
						repositioned++;
						attacked_left_num = i + 1;
						break;
					}
				}
			}
		}
		else {

			//reposition 된 뒤에 같은 공이 3개 이상 연속했는지 확인하는 과정 (ID가 같은지 여부를 확인해서 cnt에 더해줌)
			int cnt = 1;
			unsigned int left = attacked_left_num;
			int right = attacked_left_num - 1;
			for (left; left < marbles.size(); left++) {
				if (marbles[left].getID() == attackmarble.getAttackmarble().getID())
					cnt++;
				else break;
			}
			for (right; right >= 0; right--) {
				if (marbles[right].getID() == attackmarble.getAttackmarble().getID()) {
					cnt++;
				}
				else break;
			}
			if (cnt < 3 && flag==0)flag = 6;
			right++;
			if (flag != 5 && cnt >= 3) {
				float next_pos[2] = { attackmarble.getAttackmarble().getCenter()[0],
							attackmarble.getAttackmarble().getCenter()[1] + (1/sqrtf(3))*RADIUS };
				attackmarble.getAttackmarble().setCenter(next_pos[0], next_pos[1], 0.0f);
				flag++;
			}

			if (cnt >= 3 && flag == 5) {
				flag = 0;
				attackmarble.reset(); repositioned = 0;
				cannon.setCanshoot(true);
				score += cnt;

				PlaySound(TEXT(SOUND_FILE_NAME_BOOM), NULL, SND_ASYNC | SND_ALIAS); // 2022-11-24 수정 sound 추가

				// 2022-11-24 수정 pull 조건 및 pull_length 수정(pre-progress 추가하니 이전 방식대로 했을때 공들이 겹치는 현상 발생)
				int prevous_marbles_size = marbles.size();

				//marbles vector에서 조건 충족시키면 지워버리기
				marbles.erase(marbles.begin() + right, marbles.begin() + left);


				//다 지우면 게임 종료하기
				if (marbles.empty()) {
					PlaySound(TEXT(SOUND_FILE_NAME_WIN), NULL, SND_ASYNC | SND_ALIAS); // 2022-11-24 수정 sound 추가
					Sleep(2500);
					exit(0);
				}

				//지운 후에 pull 구현하기

				if (right != 0 && left != prevous_marbles_size) { // 2022-11-24 수정 right != 0 && left != prevous_marbles_size 인 경우 맨앞 맨뒤라 pull 필요없음
					float pull_length = marbles[right - 1].getCenter()[0] - marbles[right].getCenter()[0] - 2 * RADIUS;// 원안 -> pull_length = 2 * RADIUS * (cnt - 1);

					for (int t = right - 1; t >= 0; t--) {
						marbles[t].setCenter(marbles[t].getCenter()[0] - pull_length,
							marbles[t].getCenter()[1],
							0.0f);
					}
				}

			}
			//조건을 만족하지 않아서 사이에 넣는 경우
			else {
				if (flag >= 6 && flag < 11) {
					float next_pos[2] = { attackmarble.getAttackmarble().getCenter()[0] +0.2f*RADIUS,
							attackmarble.getAttackmarble().getCenter()[1] + sqrtf(3)/5 * RADIUS };
					attackmarble.getAttackmarble().setCenter(next_pos[0], next_pos[1], 0.0f);
						for (int t = attacked_left_num - 1; t >= 0; t--) {
							marbles[t].setCenter(marbles[t].getCenter()[0] + 0.4f * RADIUS,
								0.8f * boundaryY,
								0.0f);
						}
					flag++;
				}
				else if (flag == 11) {
					Sphere insertsphere = attackmarble.getAttackmarble();
					/*Sphere insertsphere = spheres[attackmarble.getAttackmarble().getID() - 1];
					if (attacked_left_num == marbles.size())
						insertsphere.setCenter(marbles[attacked_left_num - 1].getCenter()[0] - 2 * RADIUS,
							0.8f * boundaryY,
							0.0f);
					else insertsphere.setCenter(marbles[attacked_left_num].getCenter()[0],
						0.8f * boundaryY,
						0.0f);*/
					//marbles vector에 attackmarble을 넣어줌
					marbles.insert(marbles.begin() + attacked_left_num, insertsphere);
					flag = 0;
					attackmarble.reset(); repositioned = 0;
					cannon.setCanshoot(true);
				}
			}
			
		}
		start_t = end_t;
	}

	glutPostRedisplay();
}

//2024-11-24 수정 스코어
void displayCharacters(void* font, string str, float x, float y) {
	glRasterPos2f(x, y);
	for (int i = 0; i < str.size(); i++)
		glutBitmapCharacter(font, str[i]);
}

//2022-11-27 수정 4
void drawSquareWithTexture() {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);		glVertex2f(-boundaryX / 2.0f, -boundaryY / 2.0f);
	glTexCoord2f(0.0f, 1.0f);		glVertex2f(-boundaryX / 2.0f, boundaryY / 2.0f);
	glTexCoord2f(1.0f, 1.0f);		glVertex2f(boundaryX / 2.0f, boundaryY / 2.0f);
	glTexCoord2f(1.0f, 0.0f);		glVertex2f(boundaryX / 2.0f, -boundaryY / 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void keyboardDown(unsigned char key, int x, int y) {
	if (key == 'q') cannon.setAngle(cannon.getAngle() + 0.05f);
	if (key == 'w') cannon.setAngle(cannon.getAngle() - 0.05f);
	if (key == ' ') {
		if (cannon.getCanshoot()) {
			
			PlaySound(TEXT(SOUND_FILE_NAME_SHOOT), NULL, SND_ASYNC | SND_ALIAS); // 2022-11-24 수정 sound 추가
			attackmarble.setAttackmarble(cannon.getCurrentsphere());
			attackmarble.setShooting_angle(cannon.getAngle());
			cannon.getNextsphere().setCenter(40 * cos(cannon.getAngle()), -300.0f + 40 * sin(cannon.getAngle()), 0.0f);
			cannon.setCurrentsphere(cannon.getNextsphere());
			Sphere sphererandom(spheres[rand() % 3]); sphererandom.setCenter(0.0f, -boundaryY + RADIUS, 0.0f);
			cannon.setNextsphere(sphererandom);
			cannon.setCanshoot(false); 
			attackmarble.setCanmove(true);
			attackmarble.setEnable(true);
		}
	}
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-boundaryX, boundaryX, -boundaryY, boundaryY, -100.0, 100.0);

	//marbles의 이동 경로
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 2022-11-27 수정 5
	drawSquareWithTexture();

	//2024-11-24 수정 스코어
	string Score = "Score: ";
	Score += to_string(score);
	displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, Score, -300.f, -300.f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(2.5f);
	glBegin(GL_LINES);
	glVertex2f(-boundaryX, 0.8 * boundaryY);
	glVertex2f(boundaryX, 0.8 * boundaryY);
	glEnd();


	glEnable(GL_LIGHTING);
	light.draw(); // 빛 사용

	for (unsigned int i = 0; i < marbles.size(); i++) marbles[i].draw(); // marbles 다 그리기
	if (attackmarble.getEnable()) attackmarble.draw();  //attackmarble 그리기
	cannon.drawline(); cannon.drawmarbles(); //캐논의 유도선, 캐논에 있는 구 2개 그리기
	
	glDisable(GL_LIGHTING);

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("PM project");
	initialize();

	// register callbacks
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardDown);
	glutIdleFunc(idle);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
