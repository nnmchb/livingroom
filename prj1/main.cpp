#include "glut.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
using namespace std;
int windowWidth = 600;
int windowHeight = 600;
const int rat = 1 * windowWidth / windowHeight;
//int option = 1;
//bool flagScale = false;
float _angle = 30.0f;
//float _cameraAngle = 0.0f;
GLfloat cx = 0, cy = 1, cz = 5, ex = 0, ey = 0, ez = 0;
bool light1 = false;
bool light2 = false;
bool light3 = false;

bool ambientM = true;
bool specularM = true;
bool diffuseM = true;

bool ambient = true;
bool specular = true;
bool diffuse = true;

void face(GLfloat A[], GLfloat B[], GLfloat C[], GLfloat D[]) {
    glBegin(GL_POLYGON);
    glVertex3fv(A);
    glVertex3fv(B);
    glVertex3fv(C);
    glVertex3fv(D);
    glEnd();
}
void resize(int windowWidth, int windowHeight) {
    glViewport(100, 20, windowHeight * rat, windowHeight);
}
void doMaterial(GLfloat x, GLfloat y, GLfloat z) {
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { x / 3, y, z, 1.0 };
    GLfloat mat_diffuse[] = { x, y, z, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 60 };
    GLfloat globalAmbient[] = { 0.1,0.1,0.1,1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    if (ambientM) {
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    }
    else {
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    }
    if (diffuseM) {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    }
    else {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, no_mat);
    }
    if (specularM) {
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    }
    else {
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    }
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void light() {
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat l_amb[] = { .01, .01, .01, 1 };
    GLfloat l_dif[] = { .8, .8, .8, 1 };
    GLfloat l_spec[] = { .5, .5, .5, 1 };

    GLfloat pos[] = { 2.5, 1, -.5, 1 };
    GLfloat pos1[] = { -2.5, 1, -.5, 1 };
    GLfloat pos2[] = { 1.7, 0.2, -2.6, 1 };
    if (!ambient) {
        l_amb[0] = 0;
        l_amb[1] = 0;
        l_amb[2] = 0;
    }
    if (!diffuse) {
        l_dif[0] = 0;
        l_dif[1] = 0;
        l_dif[2] = 0;
    }
    if (!specular) {
        l_spec[0] = 0;
        l_spec[1] = 0;
        l_spec[2] = 0;
    }
    if (!light1) {
        glDisable(GL_LIGHT0);
    }
    if (light1) {
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, l_dif);
        glLightfv(GL_LIGHT0, GL_SPECULAR, l_spec);
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
    }
    if (!light2) {
        glDisable(GL_LIGHT1);
    }
    if (light2) {
        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, l_amb);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, l_dif);
        glLightfv(GL_LIGHT1, GL_SPECULAR, l_spec);
        glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    }
    if (light3) {
        glEnable(GL_LIGHT2);
        glLightfv(GL_LIGHT2, GL_AMBIENT, l_amb);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, l_dif);
        glLightfv(GL_LIGHT2, GL_SPECULAR, l_spec);
        glLightfv(GL_LIGHT2, GL_POSITION, pos2);
        GLfloat spot_direction[] = { 0.0, -2.0, 0.0 };//hướng sáng
        GLfloat spot_cutoff[] = { 45.0,0,0 };// góc mở sáng
        GLfloat spot_exponent[]{ 30,0,0 };//độ tỏa sáng
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0);
    }
    if (!light3) {
        glDisable(GL_LIGHT2);
    }
}
GLfloat V[8][3] =
{
    {-1, 0.5, .5},
    {1, 0.5, .5},
    {1, -0.5, .5},
    {-1, -0.5, .5},

    {-1, 0.5, -.5},
    {1, 0.5, -.5},
    {1, -0.5, -.5},
    {-1, -0.5, -.5}

};
GLfloat c[8][3] =
{
    {0, 1.5, 1.5},
    {2, 1.5, 1.5},
    {1, .5, 1.5},
    {0, 0.5, 1.5},

    {0, 1.5, .5},
    {2, 1.5, .5},
    {2, 0.5, .5},
    {0, 0.5, .5}

};

static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3) {
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2 - x1;
    Uy = y2 - y1;
    Uz = z2 - z1;

    Vx = x3 - x1;
    Vy = y3 - y1;
    Vz = z3 - z1;

    Nx = Uy * Vz - Uz * Vy;
    Ny = Uz * Vx - Ux * Vz;
    Nz = Ux * Vy - Uy * Vx;

    glNormal3f(Nx, Ny, Nz);
}
void cube2(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat V4[], GLfloat V5[], GLfloat V6[], GLfloat V7[]) {
    ///front
    doMaterial(0.545, 0.271, 0.075);
    getNormal3p(V0[0], V0[1], V0[2], V3[0], V3[1], V3[2], V2[0], V2[1], V2[2]);
    face(V0, V1, V2, V3);
    ///back
    //glColor3f(1.0f, 0.5f, 1.0f);
    getNormal3p(V4[0], V4[1], V4[2], V5[0], V5[1], V3[2], V6[0], V6[1], V6[2]);
    face(V4, V5, V6, V7);
    ///left
    doMaterial(0.502, 0.000, 0.000);
    getNormal3p(V0[0], V0[1], V0[2], V4[0], V4[1], V4[2], V7[0], V7[1], V7[2]);
    face(V0, V4, V7, V3);
    ///right
    //glColor3f(1.0f, 1.0f, 0.0f);
    getNormal3p(V1[0], V1[1], V1[2], V2[0], V2[1], V2[2], V6[0], V6[1], V6[2]);
    face(V1, V5, V6, V2);
    ///bottom
    doMaterial(0.502, 0.000, 0.000);
    getNormal3p(V3[0], V3[1], V3[2], V7[0], V7[1], V7[2], V6[0], V6[1], V6[2]);
    face(V3, V2, V6, V7);
    /// top
    //glColor3f(1.0f, 0.0f, 1.0f);
    getNormal3p(V0[0], V0[1], V0[2], V1[0], V1[1], V1[2], V5[0], V5[1], V5[2]);
    face(V0, V1, V5, V4);
}
void cube3(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat V4[], GLfloat V5[], GLfloat V6[], GLfloat V7[], GLfloat x, GLfloat y, GLfloat z) {
    ///top
    doMaterial(x, y, z);
    getNormal3p(V0[0], V0[1], V0[2], V3[0], V3[1], V3[2], V2[0], V2[1], V2[2]);
    face(V0, V1, V2, V3);
    /// front
    //glColor3f(1.0f, 0.5f, 1.0f);
    getNormal3p(V4[0], V4[1], V4[2], V5[0], V5[1], V3[2], V6[0], V6[1], V6[2]);
    face(V4, V5, V6, V7);
    ///right
    //glColor3f(1.0f, 1.0f, 1.0f);
    getNormal3p(V0[0], V0[1], V0[2], V4[0], V4[1], V4[2], V7[0], V7[1], V7[2]);
    face(V0, V4, V7, V3);
    ///back
    //glColor3f(1.0f, 1.0f, 0.0f);
    getNormal3p(V1[0], V1[1], V1[2], V2[0], V2[1], V2[2], V6[0], V6[1], V6[2]);
    face(V1, V5, V6, V2);
    ///left
    //glColor3f(0.0f, 1.0f, 1.0f);
    getNormal3p(V3[0], V3[1], V3[2], V7[0], V7[1], V7[2], V5[0], V5[1], V5[2]);
    face(V3, V2, V6, V7);
    /// bottom
    //glColor3f(1.0f, 1.0f, 1.0f);
    getNormal3p(V0[0], V0[1], V0[2], V1[0], V1[1], V1[2], V5[0], V5[1], V5[2]);
    face(V0, V1, V5, V4);
}
void window() {
    glPushMatrix();
    glScalef(.01, 1.3, 1.2);
    //glTranslatef(-290,0,-2);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    //window border
    glPushMatrix();
    glScalef(.01, 1.3, .01);
    glTranslatef(1, 0, 0);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0, 0, 0);
    glPopMatrix();
    //LEFT VERTICAL
    glPushMatrix();
    glScalef(.05, 1.3, .05);
    glTranslatef(1, 0, 12);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], .545, 0, 0);
    glPopMatrix();
    //RIGHT VERTICAL
    glPushMatrix();
    glScalef(.05, 1.3, .05);
    glTranslatef(1, 0, -12);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], .545, 0, 0);
    glPopMatrix();
    //MIDDLE LINE
    glPushMatrix();
    glScalef(.01, .01, 1.2);
    glTranslatef(10, 1, 0);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0, 0, 0);
    glPopMatrix();
    //TOP VERTICA;
    glPushMatrix();
    glScalef(.05, .1, 1.2);
    glTranslatef(-.6, 6, -.02);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], .545, 0, 0);
    glPopMatrix();
    //BOTTOM VERTICA;
    glPushMatrix();
    glScalef(.06, .1, 1.2);
    glTranslatef(-.2, -6.5, 0);
    //glRotatef(90,0,1,0);
    //glColor3f(1,0,0);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], .545, 0, 0);
    glPopMatrix();
}
void mirror(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glScalef(.01, 1.3, 1.2);   
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], x, y, z);
    glPopMatrix();
    //LEFT VERTICAL
    glPushMatrix();
    glScalef(.05, 1.3, .05);
    glTranslatef(1, 0, 12);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.545, 0.271, 0.075);
    glPopMatrix();
    //RIGHT VERTICAL
    glPushMatrix();
    glScalef(.05, 1.3, .05);
    glTranslatef(1, 0, -12);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.545, 0.271, 0.075);
    glPopMatrix();
    //TOP VERTICAL;
    glPushMatrix();
    glScalef(.05, .1, 1.2);
    glTranslatef(1, 6, -.02);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.545, 0.271, 0.075);
    glPopMatrix();
    //BOTTOM VERTICAL;
    glPushMatrix();
    glScalef(.06, .1, 1.2);
    glTranslatef(1, -6.5, 0);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.545, 0.271, 0.075);
    glPopMatrix();
}
void fan() {
    glPushMatrix();
    glRotatef(_angle, 0.0f, -1.0f, 0.0f);
    glPushMatrix();
    glScalef(.3, .05, .15);
    glTranslatef(-1.4, 0, 0);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.412, 0.412, 0.412);
    glPopMatrix();
    //blade front
    glPushMatrix();
    glScalef(.09, .05, .7);
    glTranslatef(0, 0, .5);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.412, 0.412, 0.412);
    glPopMatrix();
    //blade right
    glPushMatrix();
    glScalef(.3, .05, .15);
    glTranslatef(1.4, 0, 0);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.412, 0.412, 0.412);
    glPopMatrix();
    //blade back
    glPushMatrix();
    glScalef(.09, .05, .7);
    glTranslatef(0, 0, -.5);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.412, 0.412, 0.412);
    glPopMatrix();
    //center
    glPushMatrix();
    glScalef(.1, .1, .15);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    glPopMatrix();
}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cx, cy, cz, ex, ey, ez, 0, 1, 0);
    light();
    //glViewport(0,0,800,800);
    //floor
    glPushMatrix();
    glTranslatef(0, -1.5, 0);
    glScalef(3, .1, 7);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.902, 0.902, 0.980);
    glPopMatrix();
    //right wall
    glPushMatrix();
    glTranslatef(-3, 0, 0);
    glRotatef(90, 0, 0, 1);
    glScalef(1.5, .1, 7);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.502, 0.502, 0.502);
    glPopMatrix();
    //left wall
    glPushMatrix();
    glTranslatef(3, 0, 0);
    glRotatef(90, 0, 0, 1);
    glScalef(1.5, .1, 7);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.502, 0.502, 0.502);
    glPopMatrix();
    //back wall
    glPushMatrix();
    glTranslatef(0, 0, -3.6);
    glScalef(3.1, 3, .1);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.502, 0.502, 0.502);
    glPopMatrix();
    //ceiling
    glPushMatrix();
    glTranslatef(0, 1.5, 0);
    glScalef(3, .1, 7);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1.000, 0.937, 0.835);
    glPopMatrix();

    //television
    glPushMatrix();
    glScalef(.8, .8, .6);
    glTranslatef(0, 0.2, -6.4);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0, 0, 0);
    glPopMatrix();
    // Vẽ mặt bàn
    glPushMatrix(); 
    glTranslatef(0, -1, -1.8); 
    glScalef(1, 0.1, 1); 
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], .545, 0, 0);
    glPopMatrix(); 
    // Vẽ chân bàn
    glPushMatrix(); // Lưu ma trận hiện tại
    glTranslatef(-0.45, -1.45, -1.4); // Di chuyển và thay đổi vị trí của chân bàn 1
    glScalef(0.1, 0.9, 0.1); // Kích thước chân bàn
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1); // Vẽ chân bàn 1
    glPopMatrix(); // Khôi phục ma trận hiện tại

    glPushMatrix(); // Lưu ma trận hiện tại
    glTranslatef(0.45, -1.45, -1.4); // Di chuyển và thay đổi vị trí của chân bàn 2
    glScalef(0.1, 0.9, 0.1); // Kích thước chân bàn
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1); // Vẽ chân bàn 2
    glPopMatrix(); 

    glPushMatrix(); // Lưu ma trận hiện tại
    glTranslatef(-0.45, -1.45, -2); // Di chuyển và thay đổi vị trí của chân bàn 3
    glScalef(0.1, 0.9, 0.1); // Kích thước chân bàn
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1); // Vẽ chân bàn 3
    glPopMatrix(); // Khôi phục ma trận hiện tại

    glPushMatrix(); // Lưu ma trận hiện tại
    glTranslatef(0.45, -1.45, -2); // Di chuyển và thay đổi vị trí của chân bàn 4
    glScalef(0.1, 0.9, 0.1); // Kích thước chân bàn
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1); // Vẽ chân bàn 4
    glPopMatrix(); // Khôi phục ma trận hiện tại
    // Vẽ ghế fa
    // Mặt ghế
    glPushMatrix();
    glTranslatef(0, -1.05, 0.2);
    glScalef(1.5, 0.35, 1.2);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.6f, 0.8f, 1.0f);
    glPopMatrix();
    //tay ghế 1
    glPushMatrix();
    glTranslatef(1.2, -0.8, 0.16);
    glScalef(0.1, 0.5, 0.8);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1.0f, 1.0f, 1.0f);
    glPopMatrix();
    //tay ghế 2
    glPushMatrix();
    glTranslatef(-1.2, -0.8, 0.16);
    glScalef(0.1, 0.5, 0.8);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1.0f, 1.0f, 1.0f);
    glPopMatrix();
    //Lưng ghế
    glPushMatrix();
    glTranslatef(0, -0.65, 0.63);
    glScalef(1.2, 0.5, 0.2);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1.0f, 0.5f, 0.5f);
    glPopMatrix();
    //tủ đèn
    glPushMatrix();
    glTranslatef(1.5, -1.2, -3);
    //body
    glPushMatrix();
    glScalef(.3, .6, .8);
    cube2(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    glPopMatrix();
    //border
    glPushMatrix();
    glScalef(.3, .04, .01);
    glTranslatef(0, 0, 40);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], .545, 0, 0);
    glPopMatrix();
    //tay cầm trên
    glPushMatrix();
    glScalef(.1, .04, .01);
    glTranslatef(0, 3, 50);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    //tay cầm dưới
    glPushMatrix();
    glScalef(.1, .04, .01);
    glTranslatef(0, -3, 50);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    glPopMatrix();
    //tủ TV
    glPushMatrix();
    glTranslatef(0, -1, -3);
    //body
    glPushMatrix();
    glScalef(.98, 1, .8);
    cube2(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    glPopMatrix();
    //border top
    glPushMatrix();
    glScalef(.8, .04, .01);
    glTranslatef(0, 4, 40);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], .545, 0, 0);
    glPopMatrix();
    //border bottom
    glPushMatrix();
    glScalef(.8, .04, .01);
    glTranslatef(0, -4, 40);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], .545, 0, 0);
    glPopMatrix();
    //Handle bottom
    glPushMatrix();
    glScalef(.1, .04, .01);
    glTranslatef(0, -8, 50);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    //handle middle
    glPushMatrix();
    glScalef(.1, .04, .01);
    glTranslatef(0, 0, 50);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    //handle top
    glPushMatrix();
    glScalef(.1, .04, .01);
    glTranslatef(0, 8, 50);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    glPopMatrix();
    //tủ giày
    glPushMatrix();
    glTranslatef(-2.7, -1.1, -0.38);
    //body
    glPushMatrix();
    glScalef(.4, .7, .6);
    glRotatef(90, 0, 1, 0);
    cube2(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    glPopMatrix();
    //border
    glPushMatrix();
    glScalef(.02, .7, .05);
    glTranslatef(10, 0, 0);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], .545, 0, 0);
    glPopMatrix();
    //left handle
    glPushMatrix();
    glScalef(.03, .2, .05);
    glTranslatef(8, 0, 6);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    //left handle
    glPushMatrix();
    glScalef(.03, .2, .05);
    glTranslatef(8, 0, -5);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    //window
    glPushMatrix();
    glTranslatef(1.5, 0, -3.4);
    glRotatef(270, 0, 1, 0);
    window();
    glPopMatrix();
    // WONDOW-bLACK
    glPushMatrix();
    glTranslatef(-2.9, 0, -2);
    window();
    glPopMatrix();

    //Điều hòa
    glPushMatrix();
    glScalef(.8, .4, .5);
    glTranslatef(0, 2.8, -6.4);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
  
    //tranh
    glPushMatrix();
    glTranslatef(-2, 0.5, -3.5);
    glScalef(0.6, .6, .6);
    glRotatef(270, 0, 1, 0);
    mirror(0.690, 0.878, 0.902);
    glPopMatrix();
    //clock
    glPushMatrix();
    glTranslatef(1.5, 1, -3.4);
    glScalef(0.3, .3, .3);
    glRotatef(270, 0, 1, 0);
    //body
    glPushMatrix();
    mirror(1, 1, 1);
    glPopMatrix();
    //minute
    glPushMatrix();
    glTranslatef(0, .05, -.2);
    glRotatef(-45, 1, 0, 0);
    glScalef(.02, .5, .05);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0, 0, 0);
    glPopMatrix();
    //second
    glPushMatrix();
    glTranslatef(0.08, .2, 0);
    glScalef(0.03, .6, .04);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0, 0, 0);
    glPopMatrix();
    //hour
    glPushMatrix();
    glTranslatef(0.05, -.3, 0);
    glScalef(.005, .8, .05);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0, 0, 0);
    glPopMatrix();
    glPopMatrix();
    //lamp-head
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    glTranslatef(1.52, -.02, 2.9);
    glScalef(.3, .3, .3);
    for (int i = 0; i < 360; i += 1) {
        glPushMatrix();
        glRotatef(i, 0, 1, 0);
        glRotatef(30, 0, 0, 1);
        glScalef(.1f, 1, .1f);
        cube3(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], 0.545, 0, 0);
        glPopMatrix();
    }
    glPopMatrix();
    //lamp handle
    glPushMatrix();
    glTranslatef(1.5, -.6, -2.9);
    glScalef(.02, .4, .1);
    cube2(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    glPopMatrix();
    //lamp base
    glPushMatrix();
    glTranslatef(1.5, -0.85, -2.8);
    glScalef(.1, .1, .1);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0, 0, 0);
    glPopMatrix();
    //fan
    glPushMatrix();
    glTranslatef(0, 1, 0);
    fan();
    //handle
    glPushMatrix();
    glTranslatef(0, .3, 0);
    glScalef(.02, .5, .1);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 0.412, 0.412, 0.412);
    glPopMatrix();
    glPopMatrix();
    //light1-right
    glPushMatrix();
    glTranslatef(3, 1, -.5);
    glScalef(.1, .1, 1);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-3, 1, -.5);
    glScalef(.1, .1, 1);
    cube3(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7], 1, 1, 1);
    glPopMatrix();
    //light3-spot
    glTranslatef(1.5, -1, -1);
    glFlush();
    glutSwapBuffers();
}
void myKeyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'x':
        cx = cx - 0.5;
        break;
    case 'X':
        cx = cx + 0.5;
        break;
    case 'y':
        cy = cy - 0.5;
        break;
    case 'Y':
        cy = cy + 0.5;
        break;
    case '-':
        cz = cz + .5;
        break;
    case '+':
        cz = cz - .5;
        break;
    case 'L':
        ex = ex - 0.5;
        break;
    case 'R':
        ex = ex + 0.5;
        break;
    case 'D':
        ey = ey - 0.5;
        break;
    case 'U':
        ey = ey + 0.5;
        break;
    case '1':
        light1 = !light1;
        break;
    case '2':
        light2 = !light2;
        break;
    case '3':
        light3 = !light3;
        break;
    case 'd':
        diffuse = !diffuse;
        break;
    case 's':
        specular = !specular;
        break;
    case 'a':
        ambient = !ambient;
        break;
    case 'f':
        diffuseM = !diffuseM;
        break;
    case 'g':
        specularM = !specularM;
        break;
    case 'h':
        ambientM = !ambientM;
        break;
    }
    printf("Ambient:%d Specular:%d Diffusion:%d\n", ambient, specular, diffuse);
    //printf("%d %d %d\n" , ambientM , specularM , diffuseM);
    glutPostRedisplay();
}
void MyInit() {
    glClearColor(0, 0, 0, 1);  //background color
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 2, 10);
    glMatrixMode(GL_MODELVIEW);
}
void update(int value) {
    _angle += 12.0f;
    if (_angle > 360) {
        _angle -= 360;
    }
    glutPostRedisplay();  ////Tell GLUT that the scene has changed
    glutTimerFunc(25, update, 0);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Bedroom");
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    MyInit();
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboardFunc);
    //glutReshapeFunc(resize);
    glEnable(GL_LIGHTING);
    glutTimerFunc(25, update, 0);  //Add a timer
    printf("Follow below instructions:\n\n x/X to move camera on X axis \n y/Y to move camera on Y axis \n + to zoom in \n - to zoom out \n R to look right side \n L to look left side \n U to look up \n D to look down\n 1 for on/off Right light,2 for on/off left light,3 for on/off spot light \n a for on/off ambient light \n s for on/off specular light \n d for on/off diffusion light \n");
    glutMainLoop();
    return 0;
}
