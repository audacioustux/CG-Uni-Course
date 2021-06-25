#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <array>
#include <iostream>
#include <math.h>
#include <random>
#include <tuple>
#include <vector>

using namespace std;

#define PI 3.14159265358
#define PI2 6.28318530718

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 650
#define STAR_COUNT 100

#define GL_255U 1.0f / 255

union Color {
  unsigned int hex;
#if IS_BIG_ENDIAN
  struct {
    unsigned char r, g, b;
  };
#else
  struct {
    unsigned char b, g, r;
  };
#endif
};
struct GLColorRGB {
  GLfloat r, g, b;
};

GLColorRGB Hex2glRGB(unsigned int hex) {
  union Color _hex;
  _hex.hex = hex;
  GLColorRGB rgb;
  rgb.r = _hex.r / 255.f;
  rgb.g = _hex.g / 255.f;
  rgb.b = _hex.b / 255.f;
  return rgb;
}

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

void glDrawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

void glDrawQuadUnit() {
  glBegin(GL_QUADS);
  float coords[] = {0, 0, 1, 0, 1, 1, 0, 1};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
}
void glDrawQuad(GLfloat width, GLfloat height) {
  glPushMatrix();
  glScalef(width, height, 0);
  glDrawQuadUnit();
  glPopMatrix();
};
void glDrawQuad(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
  glPushMatrix();
  glTranslatef(x, y, 0.f);
  glScalef(width, height, 0);
  glDrawQuad(width, height);
  glPopMatrix();
}
void glDrawQuad(GLfloat size) { glDrawQuad(size, size); };
void glDrawQuad(GLfloat x, GLfloat y, GLfloat size) {
  glDrawQuad(x, y, size, size);
};
void glDrawPolygon(vector<array<GLfloat, 2>> poly_cords) {
  glBegin(GL_POLYGON);
  for (auto cord : poly_cords) {
    glVertex2fv((GLfloat *)&cord);
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
  glDrawFilledCircle(0, 0, .3);
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

  glColor4f(GL_255U * 51, GL_255U * 34, GL_255U * 114, .4f);
  glDrawFilledCircle(0, 0, .4);

  glDrawMoon();

  glPopMatrix();
}

void glDrawGrid(GLfloat width, GLfloat height) {
  glColor4f(1, 1, 1, 0.05);
  glDrawLine(0, height, width, 0);
  glDrawLine(0, 0, width, height);

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

void glDrawRiver(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
  glColor3f(0, GL_255U * 94, GL_255U * 137);
  glDrawQuad(x, y, width, height);
};

void glDrawWindMillBlades() {
  for (int i = 0; i < 4; ++i) {
    glPushMatrix();
    glRotatef(90 * i, 0, 0, 1);
    glTranslatef(-.1, .1, 0);
    glDrawQuad(1.f / 5, 1);
    glPopMatrix();
  }
}
void glDrawWindMill() {
  static short blade_angle = 0;

  glColor3f(1, 1, 1);
  glPushMatrix();
  glTranslatef(800, 200, 0.f);
  glDrawQuad(-12, 0, 5, 15);

  glColor3f(.9, .9, .9);
  glPushMatrix();
  glScalef(100, 100, 0);
  glRotatef(blade_angle, 0, 0, 1);
  glDrawWindMillBlades();
  glPopMatrix();

  glColor3f(1, 1, 1);
  glDrawFilledCircle(0, 0, 20);
  glPopMatrix();

  blade_angle = (blade_angle % 360) + 1;
}
void glDrawMountains() {
  vector<tuple<tuple<unsigned int, GLfloat>, vector<array<GLfloat, 2>>>>
      mountains = {
          {{0x0BB79A, 1}, {{0, 505}, {0, 526}, {100, 494}, {56, 450}}},
          {{0x0b947f, 1},
           {
               {50, 500},
               {40, 485},
               {0, 526},
               {100, 494},
               {56, 450},
           }},
          {{0x0BB79A, 1},
           {{0, 572}, {440, 572}, {222, 370}, {125, 484}, {0, 526}}},
          {{0x0b947f, 1},
           {
               {440, 572},
               {222, 370},
               {256, 500},
               {214, 474},
               {200, 508},
               {168, 492},
               {118, 572},
           }},
      };

  for (auto mountain : mountains) {
    auto [_color, cords] = mountain;
    auto [color, alpha] = _color;
    auto [r, g, b] = Hex2glRGB(color);
    glColor4f(r, g, b, alpha);
    glDrawPolygon(get<1>(mountain));
  }
}
void glDrawScene(GLFWwindow *window) {
  glDrawGrid(WINDOW_WIDTH, WINDOW_HEIGHT);

  glDrawSky();
  glDrawMountains();
  glDrawWindMill();
  glDrawRiver(0, WINDOW_HEIGHT - 50, WINDOW_WIDTH, 50);
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
