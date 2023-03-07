#pragma comment(lib, "legacy_stdio_definitions.lib")
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <gl/GL.H>
#include <gl/GLU.H>
#include <gl/glut.h>
#include <gl/glaux.h>


#define CUBE_SIZE 40.0f  // 큐브 크기

#define rb_center_x 500.0f   // 로봇중심 x좌표

#define rb_center_y 150.0f    // 로봇중심 y좌표

#define RB_BODY_SIZE 120.0f // 로봇몸통 크기

#define RB_ARTICUL_GAP 30.0f   // 로봇관절 간격

#define RB_LEG_LENGTH 120.0f   // 로봇다리 길이

#define RB_ARM_LENGTH 200.0f   // 로봇팔 길이

#define VIEW_SIZE 2000.0f


static double time = 0;  //time변수

float RB_HEAD_SIZE = 50.F;

float center_Y = 0.0f;  // 화면 중심 Y축

float left_arm_X = 0.0f;  // 왼팔 X축

float right_arm_X = 0.0f;  // 오른팔 X축

float left_thigh_X = 0.0f;  // 왼쪽 허벅지 X축

float right_thigh_X = 0.0f;  // 오른쪽 허벅지 X축

float left_calf_X = 0.0f;  // 왼쪽 종아리 X축

float right_calf_X = 0.0f;  // 오른쪽 종아리 X축

float body_Y = 0.0f;  // 몸통 Y축

float head_Y = 0.0f;  // 머리 Y축

static int camera = 1;

float y_fDistance = -2.5f;
float z_fDistance = -2.5f;
float g_fSpinX = 3.0f;
float g_fSpinY = -3.0f;


GLuint g_textureID[2];  // 텍스처 공간 생성

void TextureMapping_Load(void) {  // 텍스처 매핑 이미지 로드 함수

    AUX_RGBImageRec* pTextureImage;  // 파일 읽기 함수

    pTextureImage = auxDIBImageLoad("ground.bmp");  // bmp형식 사진을 로드

    glGenTextures(1, &g_textureID[0]);  // 0번 텍스처 공간
    glBindTexture(GL_TEXTURE_2D, g_textureID[0]);   // 0번 텍스처
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // 텍스처 매개변수 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // 텍스처 매개변수 설정
    glTexImage2D(GL_TEXTURE_2D, 0, 3, pTextureImage->sizeX, pTextureImage->sizeY, 0,
        GL_RGB, GL_UNSIGNED_BYTE, pTextureImage->data);  // 2D 텍스처 이미지 정의

    if (pTextureImage) {
        if (pTextureImage->data) free(pTextureImage->data);  // 텍스처 이미지에 저장된게 있을시
        free(pTextureImage);   // 데이터 메모리 해제
    }

    pTextureImage = auxDIBImageLoad("fence.bmp");   // bmp형식 사진을 로드

    glGenTextures(1, &g_textureID[1]);   // 1번 텍스처 공간
    glBindTexture(GL_TEXTURE_2D, g_textureID[1]);   // 1번 텍스처
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // 텍스처 매개변수 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 텍스처 매개변수 설정
    glTexImage2D(GL_TEXTURE_2D, 0, 3, pTextureImage->sizeX, pTextureImage->sizeY, 0,
        GL_RGB, GL_UNSIGNED_BYTE, pTextureImage->data);  // 2D 텍스처 이미지 정의

    if (pTextureImage) {
        if (pTextureImage->data) free(pTextureImage->data);      // 텍스처 이미지에 저장된게 있을시
        free(pTextureImage);  // 데이터 메모리 해제
    }
}


int InitGL(void) {

    TextureMapping_Load();  // 이미지 로드 함수

    glShadeModel(GL_SMOOTH);  // 색상 방법 선택

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // 색상 초기화

    glClearDepth(1.0f);  // 깊이 버퍼 삭제

    glEnable(GL_DEPTH_TEST);  // Z버퍼에 저장

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // 원근 보정을 픽셀별로 수행

    return true;    // true 리턴
}


void glInit(void) {   // GL초기화함수.

    glEnable(GL_DEPTH_TEST);   // 깊이 버퍼 사용

    glEnable(GL_NORMALIZE);  // 정규화

    glEnable(GL_SMOOTH);  // 각 버텍스의 색상을 부드럽게

    glEnable(GL_LIGHTING);  // OpenGL조명

    GLfloat ambientLight[] = { 0.3f,0.3f,0.3f,1.0f };  // 조명의 특징을 정의

    GLfloat diffuseLight[] = { 0.7f,0.7f,0.7f,1.0f };  // 조명의 특징을 정의

    GLfloat specular[] = { 1.0f,1.0f,1.0f,1.0f };  // 조명의 특징을 정의

    GLfloat specref[] = { 1.0f,1.0f,1.0f,1.0f };  // 조명의 특징을 정의

    GLfloat position[] = { 400.0f,300.0f,700.0f,1.0f };  // 조명의 특징을 정의


    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);  // GL_AMBIENT 조명의 성질 설정

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);  // GL_DIFFUSE 조명의 성질 설정

    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);  // GL_SPECULAR 조명의 성질 설정

    glLightfv(GL_LIGHT0, GL_POSITION, position);  // 조명의 위치 설정

    glEnable(GL_LIGHT0); // 조명 0을 사용

    glEnable(GL_COLOR_MATERIAL);    // 재질의 색을 폴리곤색으로

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  // 주변광, 발산광 재질에 대해서만

    glMateriali(GL_FRONT, GL_SHININESS, 128);  //재질의 밝기 설정


    glClearColor(0.0, 0.0, 0.0, 0.0); // 배경


    glMatrixMode(GL_PROJECTION);  // 투영행렬

    glLoadIdentity();  // 초기화

    glOrtho(-VIEW_SIZE, VIEW_SIZE, -VIEW_SIZE, VIEW_SIZE, -VIEW_SIZE, VIEW_SIZE);   // 투영적용
}


// =============== 팔 생성 ===============
void CreateArm() {

    glColor3f(1, 0.8, 0.8);  // 색상

    glTranslatef(RB_BODY_SIZE / 2.0f + RB_ARTICUL_GAP, RB_BODY_SIZE * 0.9, 0.0f);  // 이동

    glRotatef(15, 0, 0, 1);  // 축을 중심으로 팔의 움직임

    glRotatef(left_arm_X, 1, 0, 0);  // 팔흔들기

    glRotatef(-30, 0, 1, 0);        // 회전

    glScalef(RB_ARM_LENGTH / 6.0 / CUBE_SIZE, RB_ARM_LENGTH / CUBE_SIZE, RB_ARM_LENGTH / 6.0 / CUBE_SIZE);  // 크기

    glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);        // 이동

    glutSolidCube(CUBE_SIZE);       // CUBE_SIZE 크기의 사각형 생성

    glPopMatrix();      // PUSH, POP 매트리스 생성
    glPushMatrix();     // PUSH, POP 매트리스 생성


    glTranslatef(-(RB_BODY_SIZE / 2.0f + RB_ARTICUL_GAP), RB_BODY_SIZE * 0.9, 0.0f);  // 이동

    glRotatef(-15, 0, 0, 1);  // 축을 줌심으로 팔의 움직임

    glRotatef(right_arm_X, 1, 0, 0); //팔흔들기.

    glRotatef(30, 0, 1, 0);     // 회전

    glScalef(RB_ARM_LENGTH / 6.0 / CUBE_SIZE, RB_ARM_LENGTH / CUBE_SIZE, RB_ARM_LENGTH / 6.0 / CUBE_SIZE);      // 크기

    glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);        // 이동

    glutSolidCube(CUBE_SIZE);       // CUBE_SIZE 크기의 사각형 생성
}

// =============== 다리 생성 ===============

void CreateLeg() {

    glTranslatef(RB_BODY_SIZE / 4.0f, -RB_ARTICUL_GAP, 0.0f);  // 허벅지

    glRotatef(2, 0, 0, 1);  // 몸통과 다리 각도

    glColor3f(1, 0.8, 0.8);  // 색상 설정

    glRotatef(left_thigh_X, 1, 0, 0);  // 다리흔들기

    glScalef(RB_LEG_LENGTH / 2.5 / CUBE_SIZE, RB_LEG_LENGTH / CUBE_SIZE, RB_LEG_LENGTH / 2.5 / CUBE_SIZE);  // 크기 설정

    glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);  // 이동

    glutSolidCube(CUBE_SIZE);     // CUBE_SIZE 크기의 사각형 생성   

    glScalef(CUBE_SIZE * 2.5 / RB_LEG_LENGTH, CUBE_SIZE / RB_LEG_LENGTH, CUBE_SIZE * 2.5 / RB_LEG_LENGTH);  // 스케일 원상 복구


    glTranslatef(0.0f, -RB_LEG_LENGTH / 2.0f - RB_ARTICUL_GAP, 0.0f);  // 종아리

    glColor3f(0.8, 1, 1);  // 색상 설정

    glRotatef(left_calf_X, 1, 0, 0);  // 무릎각도. 무릎흔들기

    glScalef(RB_LEG_LENGTH / 2.5 / CUBE_SIZE, RB_LEG_LENGTH / CUBE_SIZE, RB_LEG_LENGTH / 2.5 / CUBE_SIZE);   // 크기 설정

    glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);   // 이동

    glutSolidCube(CUBE_SIZE);  // CUBE_SIZE 크기의 사각형 생성

    glPopMatrix();  // PUSH, POP 매트리스 생성

    glPushMatrix();  // PUSH, POP 매트리스 생성


    glTranslatef(-RB_BODY_SIZE / 4.0f, -RB_ARTICUL_GAP, 0.0f);  // 허벅지

    glRotatef(-2, 0, 0, 1);  // 몸통과 다리 각도

    glColor3f(1, 0.8, 0.8);  // 색상 설정

    glRotatef(right_thigh_X, 1, 0, 0);  // 다리흔들기

    glScalef(RB_LEG_LENGTH / 2.5 / CUBE_SIZE, RB_LEG_LENGTH / CUBE_SIZE, RB_LEG_LENGTH / 2.5 / CUBE_SIZE);  // 크기 설정

    glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);  // 이동

    glutSolidCube(CUBE_SIZE);   // CUBE_SIZE 크기의 사각형 생성

    glScalef(CUBE_SIZE * 2.5 / RB_LEG_LENGTH, CUBE_SIZE / RB_LEG_LENGTH, CUBE_SIZE * 2.5 / RB_LEG_LENGTH);  // 스케일원상복구


    glTranslatef(0.0f, -RB_LEG_LENGTH / 2.0f - RB_ARTICUL_GAP, 0.0f); // 종아리

    glColor3f(0.8, 1, 1);  // 색상 설정

    glRotatef(right_calf_X, 1, 0, 0);  // 무릎각도. 무릎흔들기.

    glScalef(RB_LEG_LENGTH / 2.5 / CUBE_SIZE, RB_LEG_LENGTH / CUBE_SIZE, RB_LEG_LENGTH / 2.5 / CUBE_SIZE);  // 크기 설정

    glTranslatef(0.0f, -CUBE_SIZE / 2.0f, 0.0f);  // 이동

    glutSolidCube(CUBE_SIZE);  // CUBE_SIZE 크기의 사각형 생성
}

// =============== 몸통 생성 ===============

void CreateBody() {

    glColor3f(0.2, 0, 1);       // 색상 설정

    glTranslatef(rb_center_x, rb_center_y, 0.0f);   // 로봇중심 이동

    glRotatef(0, 0, 1, 0); //로봇을돌리는앵글.

    glPushMatrix();  // PUSH 매트리스 생성


    glScalef(RB_BODY_SIZE / CUBE_SIZE, RB_BODY_SIZE / CUBE_SIZE, RB_BODY_SIZE / CUBE_SIZE);     // 크기 설정

    glTranslatef(0.0f, CUBE_SIZE / 2, 0.0f);        // 이동

    glRotatef(body_Y, 0, 1, 0); //몸통흔들기.

    glutSolidCube(CUBE_SIZE);       // CUBE_SIZE 크기의 사각형 생성
}

// =============== 머리 생성 ===============

void CreateHead() {

    glColor3f(1, 0.8, 0.8);     // 색상 설정

    glTranslatef(0.0f, RB_BODY_SIZE + RB_ARTICUL_GAP + (RB_HEAD_SIZE / 2.0f), 0.0f); // 얼굴중심.

    glRotatef(head_Y, 0, 1, 0);//얼굴움직임.

    glScalef(RB_HEAD_SIZE / CUBE_SIZE, RB_HEAD_SIZE / CUBE_SIZE, RB_HEAD_SIZE / CUBE_SIZE);     // 크기 설정

    glutSolidCube(CUBE_SIZE);       // CUBE_SIZE 크기의 사각형 생성
}


// =============== 케이지 생성 ===============

void CreateCage() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 버퍼 색상을 초기화시키고 깊이 버퍼를 적용

    glMatrixMode(GL_MODELVIEW);  //단위 행렬 모델뷰 행렬로 설정

    glPushMatrix();  // PUSH, POP 매트리스 생성


     // 케이지 밑바닥

    glNormal3f(0.0f, 1.0f, 0.0f);  // 법선벡터

    glRotatef(90.0, 0.0, 1.0, 0.0);  // 회전

    glBegin(GL_QUADS);  // 사각형 생성

    glColor3f(255, 255, 255);  // 흰색 색상

    glTexCoord2f(0.0f, 0.0f);       glVertex3f(-1005.0f, -400.0f, 1005.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 0.0f);       glVertex3f(-1005.0f, -400.0f, -1005.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 1.0f);       glVertex3f(1005.0f, -400.0f, -1005.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(0.0f, 1.0f);       glVertex3f(1005.0f, -400.0f, 1005.0f);  // 사각형의 꼭지점 좌표
    glEnd();


    // 케이지 바닥

    glNormal3f(0.0f, 1.0f, 0.0f);  // 법선벡터

    glEnable(GL_TEXTURE_2D);  // 텍스처 매핑 적용

    glBindTexture(GL_TEXTURE_2D, g_textureID[0]);   // 0번이미지 적용

    glRotatef(90.0, 0.0, 1.0, 0.0);  // 회전

    glBegin(GL_QUADS);  // 사각형 생성

    glColor3f(255, 255, 255);  // 흰색 색상

    glTexCoord2f(0.0f, 0.0f);       glVertex3f(-1055.0f, -200.0f, 1055.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 0.0f);       glVertex3f(-1055.0f, -200.0f, -1055.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 1.0f);       glVertex3f(1055.0f, -200.0f, -1055.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(0.0f, 1.0f);       glVertex3f(1055.0f, -200.0f, 1055.0f);  // 사각형의 꼭지점 좌표
    glEnd();


    // 오른쪽 벽

    glNormal3f(0.0f, 1.0f, 0.0f);  // 법선벡터

    glEnable(GL_TEXTURE_2D);  // 텍스처 매핑 적용

    glBindTexture(GL_TEXTURE_2D, g_textureID[1]);  // 1번 이미지 적용

    glRotatef(90.0, 0.0, 1.0, 0.0);  // 회전

    glBegin(GL_QUADS);  // 사각형 생성

    glColor3f(255, 255, 255);  // 흰색 색상

    glTexCoord2f(0.0f, 1.0f);       glVertex3f(-1150.0f, 200.0f, 1150.0f);  //사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 1.0f);       glVertex3f(-1150.0f, 200.0f, -1150.0f);  //사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 0.0f);       glVertex3f(-1000.0f, -400.0f, -1000.0f);  //사각형의 꼭지점 좌표
    glTexCoord2f(0.0f, 0.0f);       glVertex3f(-1000.0f, -400.0f, 1000.0f);  //사각형의 꼭지점 좌표
    glEnd();


    //정면 벽

    glNormal3f(0.0f, 1.0f, 0.0f);  // 법선벡터

    glEnable(GL_TEXTURE_2D);  // 텍스처 매핑 적용

    glBindTexture(GL_TEXTURE_2D, g_textureID[1]);  // 1번이미지 적용

    glRotatef(90.0, 0.0, 1.0, 0.0);  // 회전

    glBegin(GL_QUADS);  // 사각형 생성

    glColor3f(255, 255, 255);  // 흰색 색상

    glTexCoord2f(0.0f, 0.0f);       glVertex3f(1000.0f, -400.0f, 1000.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 0.0f);       glVertex3f(1000.0f, -400.0f, -1000.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 1.0f);       glVertex3f(1150.0f, 200.0f, -1150.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(0.0f, 1.0f);       glVertex3f(1150.0f, 200.0f, 1150.0f);   // 사각형의 꼭지점 좌표
    glEnd();


    //후면 벽

    glNormal3f(0.0f, 1.0f, 0.0f);  // 법선벡터

    glEnable(GL_TEXTURE_2D);  // 텍스처 매핑 적용

    glBindTexture(GL_TEXTURE_2D, g_textureID[1]);   // 1번이미지 적용

    glRotatef(90.0, 0.0, 1.0, 0.0);  // 회전

    glBegin(GL_QUADS);  // 사각형 생성

    glColor3f(255, 255, 255);  // 흰색 색상

    glTexCoord2f(1.0f, 0.0f);       glVertex3f(-1000.0f, -400.0f, -1000.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 1.0f);       glVertex3f(-1150.0f, 200.0f, -1150.0f);   // 사각형의 꼭지점 좌표
    glTexCoord2f(0.0f, 1.0f);       glVertex3f(1150.0f, 200.0f, -1150.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(0.0f, 0.0f);       glVertex3f(1000.0f, -400.0f, -1000.0f);  // 사각형의 꼭지점 좌표
    glEnd();


    //좌측 벽

    glNormal3f(0.0f, 1.0f, 0.0f);  // 법선벡터

    glEnable(GL_TEXTURE_2D);  // 텍스처 매핑 적용

    glBindTexture(GL_TEXTURE_2D, g_textureID[1]);  // 1번이미지 적용

    glRotatef(90.0, 0.0, 1.0, 0.0);  // 회전

    glBegin(GL_QUADS);  // 사각형 생성

    glColor3f(255, 255, 255);  // 흰색 색상

    glTexCoord2f(1.0f, 0.0f);       glVertex3f(-1000.0f, -400.0f, -1000.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(1.0f, 1.0f);       glVertex3f(-1150.0f, 200.0f, -1150.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(0.0f, 1.0f);       glVertex3f(1150.0f, 200.0f, -1150.0f);  // 사각형의 꼭지점 좌표
    glTexCoord2f(0.0f, 0.0f);       glVertex3f(1000.0f, -400.0f, -1000.0f);  // 사각형의 꼭지점 좌표
    glEnd();


    glutPostRedisplay();  // 화면에 다시 출력
}


//=============== 화면 처리 ===============

void C_Display() {  // 디스플레이 콜백

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 색상버퍼를 초기화하고 깊이 버퍼 사용

    glMatrixMode(GL_MODELVIEW);  // 단위 행렬 모델뷰 행렬로 설정

    glLoadIdentity();  // 행렬 초기화

    glRotatef(z_fDistance, 0.0f, 0.0f, 0.0f);  // 극좌표계
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);  // 극 좌표계
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);   //극 좌표계

    CreateCage();  // 케이지 그리기

    glRotatef(center_Y, 0, 1, 0.0);  // 앵글을 축으로 좌표값을 변환해 로봇이 회전

    glPushMatrix();  // PUSH 매트리스 생성


    CreateBody(); // 몸통 생성

    glPopMatrix();  // PUSH, POP 매트리스 생성

    glPushMatrix();  // PUSH, POP 매트리스 생성


    CreateHead(); // 머리  생성

    glPopMatrix();  // PUSH, POP 매트리스 생성

    glPushMatrix();  // PUSH, POP 매트리스 생성


    CreateLeg(); // 다리 생성

    glPopMatrix();  // PUSH, POP 매트리스 생성

    glPushMatrix();  // PUSH, POP 매트리스 생성


    CreateArm();  // 팔 생성

    glPopMatrix();  // PUSH, POP 매트리스 생성

    glPopMatrix();  // PUSH, POP 매트리스 생성


    glPopMatrix();      // PUSH, POP 매트리스 생성

    glPopMatrix();      // PUSH, POP 매트리스 생성


    glutSwapBuffers(); // 스왑버퍼로 화면처리를 부드럽게
}


//=============== 시간 처리 ===============

void start(void) {   // timefunc 콜백함수

    time += 0.04;  // 시간증가변수

    center_Y -= 0.3;  // 축 값을 계속 바꿔 회전

    head_Y = sin(time) * 5;  // 머리 움직임 5도

    body_Y = sin(time) * 10;  // 몸통 움직임 10도

    left_arm_X = sin(time) * 20;  // 왼팔 움직임 20도

    right_arm_X = -left_arm_X;  // 오른팔 왼팔 반대 움직임 20도

    right_thigh_X = sin(time) * 20;  // 오른쪽 허벅지 움직임 20도

    left_thigh_X = -right_thigh_X;  // 왼쪽 허벅지 오른쪽 허벅지 반대 움직임 20도

    right_calf_X = (right_thigh_X / 2.2f);  // 오른쪽 종아리 각도는 오른쪽 허벅지 각도를 1.2로 나눈 절대각도

    left_calf_X = (left_thigh_X / 2.2f);  // 왼쪽 종아리 각도는 왼쪽 허벅지 각도를 1.2로 나눈 절대각도

    glutPostRedisplay();    // 다시 화면에 디스플레이
}


//=============== 마우스 처리 ===============

void MyMouseClick(int button, int state, int x, int y)  // 마우스 이벤트 처리 콜백 함수
{
    switch (button) {

    case GLUT_LEFT_BUTTON:  // 마우스 왼쪽 버튼을
        if (state == GLUT_DOWN)  // 클릭시
            glutIdleFunc(start);  // 시작
        break;

    case GLUT_RIGHT_BUTTON:  // 마우스 오른쪽 버튼을
        if (state == GLUT_DOWN) // 클릭시
            glutIdleFunc(NULL);  // 정지
        break;
    }

}


//=============== 키보드 처리 ===============

void MySpecial(int key, int x, int y) {
    if (key == GLUT_KEY_PAGE_UP) {  // page up 버튼을 누르면
        if (z_fDistance > -70.0) z_fDistance -= 1.0f;  // 줌아웃
    }
    else if (key == GLUT_KEY_PAGE_DOWN) {  // pagedown 버튼을 누르면
        if (z_fDistance < 0.0)z_fDistance += 1.0f;   // 줌인
    }
    else if (key == GLUT_KEY_UP) {  // 위쪽 화살표를 누르면
        if (g_fSpinY < -3) g_fSpinY += 3.0f;  // 극좌표계 y축 상승
    }
    else if (key == GLUT_KEY_DOWN) {  // 아래 화살표를 누르면
        if (g_fSpinY > -95)g_fSpinY -= 3.0f;  // 극좌표계 y축 하강
    }
    else if (key == GLUT_KEY_LEFT) {   // 왼쪽 화살표를 누르면
        g_fSpinX -= 3.0f;  // 극좌표계 -x축 이동
    }
    else if (key == GLUT_KEY_RIGHT) {   // 오른쪽 화살표를 누르면
        g_fSpinX += 3.0f;  // 극좌표계 +x축 이동
    }
    glutPostRedisplay();  // 화면 디스플레이
}


//=============== 메인 ===============

void main(int argc, char** argv) {

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // 더블 버퍼 컬러 모드


    glutInitWindowSize(1024, 768);  //윈도우 크기

    glutInitWindowPosition(0, 0);  //윈도우 위치

    glutCreateWindow("UFC");  // 윈도우 생성(타이틀)

    InitGL();   // 깊이버퍼, 원근 보간법

    glutDisplayFunc(C_Display);  // display

    glutSpecialFunc(MySpecial);  // 키보드 호출

    glutMouseFunc(MyMouseClick);  // 마우스 콜백

    glInit();  // 조명 함수

    glutMainLoop();  // 메인루프

}