#include <iostream>
#include <vector>
#include <string>
#include <ctime> // ���� ������
#include <windows.h> //delay �ְ� ���� �� Sleep() ��� ex) Sleep(1000) 1�� delay

#pragma comment(lib,"winmm")
#include <mmsystem.h> // // 2022-11-24 ���� sound ���Կ�

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

// 2022-11-24 ���� sound �߰� ���
#define SOUND_FILE_NAME_START "C:/Users/User/source/repos/Project_final2/Project_final2/sound/start.wav"
#define SOUND_FILE_NAME_SHOOT "C:/Users/User/source/repos/Project_final2/Project_final2/sound/shoot3.wav"
#define SOUND_FILE_NAME_BOOM "C:/Users/User/source/repos/Project_final2/Project_final2/sound/boom.wav"
#define SOUND_FILE_NAME_WIN "C:/Users/User/source/repos/Project_final2/Project_final2/sound/win.wav"
#define SOUND_FILE_NAME_LOSE "C:/Users/User/source/repos/Project_final2/Project_final2/sound/lose.wav"

int score =0 ; // 2022-11-24 ���� scoreboard��

clock_t start_t = clock();
clock_t end_t;

vector<Sphere> spheres; // 3���� ���� ������ ���� ����
vector<Sphere> marbles; // ���������� �̵��ϴ� ������

Attackmarble attackmarble; // spacebar�� ���� �� ���󰡴� ��

Cannon cannon; // ĳ�� 
float shooting_angle; //ĳ���� ����
int repositioned = 0; // ���� �ٸ� ���� �浹���� �� reposition �ƴ��� ����
int attacked_left_num; // attackmarble�� �浹�Ͽ��� �� ���ʿ� �ִ� ���� ��ȣ (spheres vector�� index�� �ش���)

Light light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);

//11.27 ���� repositioning�� �� �̵��Ͽ����ϴ� speed �Է�
float r_moving_speed[2];
int flag;

//2022-11-27 �߰�2
Texture texture;

//���� �� 3���� Ư¡�� �ʱ�ȭ (���� ������ radius�� 20���� �Ͽ���)
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
	//marbles vector�� �� �� 10��
	for (int i = 0; i < 10; i++) {
		Sphere sphererandom(spheres[rand() % 3]);
		marbles.push_back(sphererandom);
	}

	//ĳ�� �� �ʱ� �� 2�� ����
	Sphere sphererandom1(spheres[rand() % 3]); Sphere sphererandom2(spheres[rand() % 3]); 
	sphererandom1.setCenter(0.0f, -260.0f, 0.0f); sphererandom2.setCenter(0.0f, -300.0f, 0.0f); 
	cannon.setCannon(sphererandom1, sphererandom2);

	//2022-11-27 �߰�3
	texture.initializeTexture("Puzzloop.png");

	PlaySound(TEXT(SOUND_FILE_NAME_START), NULL, SND_ASYNC | SND_ALIAS); // 2022-11-24 ���� ȿ����
}

void idle() {
	end_t = clock();

	int preprogress = 10;// 2022-11-24 ���� pre-progress ����


	if (end_t - start_t > 1000 / 60) {
		if (marbles[0].getCenter()[0] + RADIUS >= boundaryX) exit(0); // ���� ������ ���� ����

		cannon.move(); // keyboard�� ������ ���� angle�� �ݿ��� ������

		// 2022-11-24 ���� pre-progress ���ۺκ� 
		//marbles[0].move(); // ���� ���ʿ� �ִ� ������ �׻� �����̰� ����

		if (repositioned == 0) {// repositioning �� �� �� �����̵��� ���� 11.27 ���� 
			if (marbles[0].getCenter()[0] <= -boundaryX + RADIUS * 2 * (preprogress - 1) && marbles.size() == 10) {

				marbles[0].initialmove();

				for (unsigned int i = 0; i < marbles.size() - 1; i++) {
					//�� ������ ������ ����� �巯���� ���� ������ �����̰� ����, preprogress�� 8�� ���� initial move
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
		// 2022-11-24 ���� pre-progress ���κ�

		//���ʿ� attackmarble�� �浹�Ͽ��� �� attackmarble�� �Ҹ��ϰ� �ϰ�, spacebar�� ���� �� �ֵ��� ����
		if (attackmarble.getEnable() && attackmarble.getCanmove()) attackmarble.move();
		if (attackmarble.getAttackmarble().getCenter()[0] > boundaryX ||
			attackmarble.getAttackmarble().getCenter()[1] > boundaryY ||
			attackmarble.getAttackmarble().getCenter()[0] < -boundaryX ||
			attackmarble.getAttackmarble().getCenter()[1] < -boundaryY) {
			attackmarble.setEnable(false); cannon.setCanshoot(true);
		}

		//10���� ������ �̵���Ű�� 11.27 ����
		if (repositioned != 10) {
			for (unsigned int i = 0; i < marbles.size(); i++) {
				//�浹�Ͽ��� ��
				if (pow((marbles[i].getCenter()[0] - attackmarble.getAttackmarble().getCenter()[0]), 2)
					+ pow((marbles[i].getCenter()[1] - attackmarble.getAttackmarble().getCenter()[1]), 2)
					< 1600.0) {
					attackmarble.setCanmove(false);
					//���� ���� �浹�Ͽ��� ��
					if (marbles[i].getCenter()[0] <= attackmarble.getAttackmarble().getCenter()[0] ){// i == marbles.size() - 1) && i != 0) { // 2022-11-24 ���� i = marbles.size()-1, i = 0 ��� ���
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
					//������ ���� �浹�Ͽ��� ��
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

			//reposition �� �ڿ� ���� ���� 3�� �̻� �����ߴ��� Ȯ���ϴ� ���� (ID�� ������ ���θ� Ȯ���ؼ� cnt�� ������)
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

				PlaySound(TEXT(SOUND_FILE_NAME_BOOM), NULL, SND_ASYNC | SND_ALIAS); // 2022-11-24 ���� sound �߰�

				// 2022-11-24 ���� pull ���� �� pull_length ����(pre-progress �߰��ϴ� ���� ��Ĵ�� ������ ������ ��ġ�� ���� �߻�)
				int prevous_marbles_size = marbles.size();

				//marbles vector���� ���� ������Ű�� ����������
				marbles.erase(marbles.begin() + right, marbles.begin() + left);


				//�� ����� ���� �����ϱ�
				if (marbles.empty()) {
					PlaySound(TEXT(SOUND_FILE_NAME_WIN), NULL, SND_ASYNC | SND_ALIAS); // 2022-11-24 ���� sound �߰�
					Sleep(2500);
					exit(0);
				}

				//���� �Ŀ� pull �����ϱ�

				if (right != 0 && left != prevous_marbles_size) { // 2022-11-24 ���� right != 0 && left != prevous_marbles_size �� ��� �Ǿ� �ǵڶ� pull �ʿ����
					float pull_length = marbles[right - 1].getCenter()[0] - marbles[right].getCenter()[0] - 2 * RADIUS;// ���� -> pull_length = 2 * RADIUS * (cnt - 1);

					for (int t = right - 1; t >= 0; t--) {
						marbles[t].setCenter(marbles[t].getCenter()[0] - pull_length,
							marbles[t].getCenter()[1],
							0.0f);
					}
				}

			}
			//������ �������� �ʾƼ� ���̿� �ִ� ���
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
					//marbles vector�� attackmarble�� �־���
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

//2024-11-24 ���� ���ھ�
void displayCharacters(void* font, string str, float x, float y) {
	glRasterPos2f(x, y);
	for (int i = 0; i < str.size(); i++)
		glutBitmapCharacter(font, str[i]);
}

//2022-11-27 ���� 4
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
			
			PlaySound(TEXT(SOUND_FILE_NAME_SHOOT), NULL, SND_ASYNC | SND_ALIAS); // 2022-11-24 ���� sound �߰�
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

	//marbles�� �̵� ���
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 2022-11-27 ���� 5
	drawSquareWithTexture();

	//2024-11-24 ���� ���ھ�
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
	light.draw(); // �� ���

	for (unsigned int i = 0; i < marbles.size(); i++) marbles[i].draw(); // marbles �� �׸���
	if (attackmarble.getEnable()) attackmarble.draw();  //attackmarble �׸���
	cannon.drawline(); cannon.drawmarbles(); //ĳ���� ������, ĳ�� �ִ� �� 2�� �׸���
	
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
