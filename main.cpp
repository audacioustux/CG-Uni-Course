#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <random>

using namespace std;

#define PI 3.14159265358
#define PI2 6.28318530718

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 650
#define STAR_COUNT 100

#define GL_255U 1.0f / 255

void glDrawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
  int i;
  int triangleAmount = 255;

  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y);
  for (i = 0; i <= triangleAmount; i++) {
    glVertex2f(x + (radius * cos(i * PI2 / triangleAmount)),
               y + (radius * sin(i * PI2 / triangleAmount)));
  }
  glEnd();
}

void glDrawHollowCircle(GLfloat x, GLfloat y, GLfloat radius) {
  int i;
  int lineAmount = 255;

  glBegin(GL_LINE_LOOP);
  for (i = 0; i <= lineAmount; i++) {
    glVertex2f(x + (radius * cos(i * PI2 / lineAmount)),
               y + (radius * sin(i * PI2 / lineAmount)));
  }
  glEnd();
}

void glDrawMoon() {
  glColor4f(1, 1, 1, 0.05);
  glDrawFilledCircle(0, 0, .15);
  glColor4f(1, 1, 1, 0.1);
  glDrawFilledCircle(0, 0, .12);
  glColor3f(1, 1, 1);
  glDrawFilledCircle(0, 0, .1);
}
void glDrawStar() {
  glColor4f(GL_255U * 185, GL_255U * 183, GL_255U * 193, 0.1);
  glDrawFilledCircle(0, 0, .2);
  glColor3f(185, GL_255U * 183, GL_255U * 193);
  glDrawFilledCircle(0, 0, .1);
}
struct STAR_META {
  int x;
  int y;
  int r;
};
void glDrawStars(GLfloat widht, GLfloat height) {
  static bool ran_once = false;
  static STAR_META stars_meta[STAR_COUNT];

  if (ran_once) {
    for (int n = 0; n < STAR_COUNT; ++n) {
      STAR_META star = stars_meta[n];

      glPushMatrix();
      glTranslatef(star.x, star.y, 0.f);
      glScalef(star.r, star.r, 1.0f);
      glDrawStar();
      glPopMatrix();
    }
    return;
  }
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> u_rand_x(0, widht);
  uniform_int_distribution<> u_rand_y(0, height);
  uniform_int_distribution<> u_rand_r(15, 30);

  for (int n = 0; n < STAR_COUNT; ++n) {
    int x = u_rand_x(gen), y = u_rand_y(gen), r = u_rand_r(gen);
    STAR_META star = {x, y, r};
    stars_meta[n] = star;
  }
  ran_once = true;
}
void glDrawSky() {
  glDrawStars(WINDOW_WIDTH, 400);

  glPushMatrix();
  glTranslatef(225, 225, 0.f);
  glScalef(300, 300, 1.0f);

  // blur out stars near moon
  glColor4f(GL_255U * 51, GL_255U * 34, GL_255U * 114, .4f);
  glDrawFilledCircle(0, 0, .4);

  glDrawMoon();

  glPopMatrix();
}

void glDrawGrid(GLfloat width, GLfloat height) {
  glColor4f(1, 1, 1, 0.05);

  glBegin(GL_LINES);
  glVertex2f(0, 0);
  glVertex2f(width, height);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(0, height);
  glVertex2f(width, 0);
  glEnd();

  for (float x = 0; x <= width; x += 50) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, 0);
    glVertex2f(x, height);
    glEnd();
  };
  for (float y = 0; y <= height; y += 50) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(0, y);
    glVertex2f(width, y);
    glEnd();
  };
}

void glDrawRiver(GLfloat x, GLfloat y, GLfloat height, GLfloat width) {
  glPushMatrix();
  glTranslatef(x, y, 0.f);
  glScalef(width, height, 1.0f);

  glColor3f(0, GL_255U * 94, GL_255U * 137);
  glBegin(GL_QUADS);
  float coords[] = {0, 0, 1, 0, 1, 1, 0, 1};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();

  glPopMatrix();
};

void glDrawScene(GLFWwindow *window) {
  glDrawGrid(WINDOW_WIDTH, WINDOW_HEIGHT);

  glDrawSky();
  glDrawRiver(0, WINDOW_HEIGHT - 50, 50, WINDOW_WIDTH);
}

void render(GLFWwindow *window) {
  if (glfwWindowShouldClose(window))
    return;

  glClearColor(GL_255U * 51, GL_255U * 34, GL_255U * 114, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glDrawScene(window);

  glfwSwapBuffers(window);

  glfwPollEvents();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  render(window);
}

void set_framebuffer_size(GLFWwindow *window, int width, int height) {
  const float targetAspect = float(WINDOW_WIDTH) / WINDOW_HEIGHT;
  float aspect = (float)width / height;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, -1);

  glMatrixMode(GL_MODELVIEW);
}

int main(void) {

  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CG-ASS", NULL, NULL);
  glfwSetWindowTitle(window, u8"Mountain View");
  glfwSetWindowPos(window, WINDOW_WIDTH, 0);
  glfwSetWindowAspectRatio(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetWindowAttrib(window, GLFW_FLOATING, GLFW_TRUE);
  glfwSetWindowAttrib(window, GLFW_FOCUS_ON_SHOW, GLFW_FALSE);

  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, set_framebuffer_size);

  glfwMakeContextCurrent(window);

  set_framebuffer_size(window, WINDOW_WIDTH, WINDOW_HEIGHT);

  render(window);

  glfwTerminate();
  return 0;
}
