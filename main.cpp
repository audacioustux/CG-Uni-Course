#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <array>
#include <chrono>
#include <ctime>
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
  return {_hex.r * GL_255U, _hex.g * GL_255U, _hex.b * GL_255U};
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
  glDrawQuad(width, height);
  glPopMatrix();
}
void glDrawQuad(GLfloat size) { glDrawQuad(size, size); };
void glDrawQuad(GLfloat x, GLfloat y, GLfloat size) {
  glDrawQuad(x, y, size, size);
};
struct Point {
  GLfloat x;
  GLfloat y;
};
void glDrawPolygon(vector<Point> poly_cords) {
  glBegin(GL_POLYGON);
  for (auto cord : poly_cords) {
    glVertex2fv((GLfloat *)&cord);
  }
  glEnd();
}
void glDrawCubicBezierCurve(Point controls[4]) {
  // https://en.wikipedia.org/wiki/B%C3%A9zier_curve
  Point p0 = controls[0], p1 = controls[1], p2 = controls[2], p3 = controls[3];
  for (float t = 0.f; t <= 1.f; t += .01f) {
    Point l0 = {(1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y};
    Point l1 = {(1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y};
    Point l2 = {(1 - t) * p2.x + t * p3.x, (1 - t) * p2.y + t * p3.y};

    Point q0 = {(1 - t) * l0.x + t * l1.x, (1 - t) * l0.y + t * l1.y};
    Point q1 = {(1 - t) * l1.x + t * l2.x, (1 - t) * l1.y + t * l2.y};

    Point c0 = {(1 - t) * q0.x + t * q1.x, (1 - t) * q0.y + t * q1.y};

    glVertex2fv((GLfloat *)&c0);
  }
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
  glColor4f(1, 1, 1, 0.1);
  glDrawFilledCircle(0, 0, .3);
  glColor3f(.8, .8, .8);
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
  glLineWidth(1);
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
void glDrawShip() {
  static short x = WINDOW_WIDTH - 300;
  auto ship_color = Hex2glRGB(0x3CAFBF);
  glColor3fv((GLfloat *)&ship_color);

  glPushMatrix();
  glTranslatef(x, 620, 0);
  glScalef(20, 15, 0);
  glBegin(GL_TRIANGLES);
  glVertex2f(0, 1);
  glVertex2f(.40, 0);
  glVertex2f(.40, 1);
  glEnd();
  glBegin(GL_QUADS);
  float pal[] = {.45, 0, .45, 1, .50, 1, .50, .2};
  glVertex2fv(pal);
  glVertex2fv(pal + 2);
  glVertex2fv(pal + 4);
  glVertex2fv(pal + 6);
  glEnd();
  glBegin(GL_TRIANGLES);
  glVertex2f(.55, .2);
  glVertex2f(.55, 1);
  glVertex2f(1, 1);
  glEnd();
  glBegin(GL_QUADS);
  float base[] = {-.2, 1.1, 0, 1.4, 1.1, 1.4, 1.2, 1.1};
  glVertex2fv(base);
  glVertex2fv(base + 2);
  glVertex2fv(base + 4);
  glVertex2fv(base + 6);
  glEnd();

  glPopMatrix();
  x -= 1;
  if (x < 0)
    x = WINDOW_WIDTH - 150;
}

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
  glTranslatef(800, 330, 0.f);
  glDrawQuad(-10, 0, 20, 250);
  glDrawQuad(-35, 245, 70, 10);
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
void glDrawMountains(
    vector<tuple<tuple<unsigned int, GLfloat>, vector<Point>>> mountains) {
  // <vector of polygons <tuple of <tuple of rgb, alpha>, <vector of {x,y}>>>

  for (auto mountain : mountains) {
    auto [_color, cords] = mountain;
    auto [color, alpha] = _color;
    auto [r, g, b] = Hex2glRGB(color);
    glColor4f(r, g, b, alpha);
    glDrawPolygon(get<1>(mountain));
  }
}
void glDrawBird() {
  static auto x = 0;
  Point flap1[] = {{-50, 200}, {-25, 225}, {-0, 200}};
  Point flap2[] = {{-50, 215}, {-25, 200}, {-0, 215}};

  glColor3f(.8, .8, .5);
  glLineWidth(10);
  glPushMatrix();
  glTranslatef(x, 0, 0);
  glBegin(GL_LINE_STRIP);
  if (x % 50 < 10) {
    glVertex2fv((GLfloat *)&flap1[0]);
    glVertex2fv((GLfloat *)&flap1[1]);
    glVertex2fv((GLfloat *)&flap1[2]);
  } else {
    glVertex2fv((GLfloat *)&flap2[0]);
    glVertex2fv((GLfloat *)&flap2[1]);
    glVertex2fv((GLfloat *)&flap2[2]);
  }
  glEnd();
  glPopMatrix();
  x += 2;
  if (x > WINDOW_WIDTH + 100)
    x = 0;
}
void glDrawLand() {

  Point controls[] = {{0, 572}, {400, 540}, {400, 570}, {WINDOW_WIDTH, 572}};
  // glColor3f(1, 0, 0);
  // glBegin(GL_LINES);
  // glVertex2fv((GLfloat *)&controls[0]);
  // glVertex2fv((GLfloat *)&controls[1]);
  // glEnd();
  // glBegin(GL_LINES);
  // glVertex2fv((GLfloat *)&controls[1]);
  // glVertex2fv((GLfloat *)&controls[2]);
  // glEnd();
  // glBegin(GL_LINES);
  // glVertex2fv((GLfloat *)&controls[2]);
  // glVertex2fv((GLfloat *)&controls[3]);
  // glEnd();
  auto land_color = Hex2glRGB(0x9DC746);
  glColor3fv((GLfloat *)&land_color);
  glBegin(GL_POLYGON);
  glDrawCubicBezierCurve(controls);
  glVertex2f(WINDOW_WIDTH, 600);
  glVertex2f(0, 600);
  glEnd();
}
void glDrawClock() {
  auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
  auto now_tm = localtime(&now);

  Point center = {WINDOW_WIDTH - 200, 100};
  GLfloat r = 50;

  glColor4f(1, 1, 1, .4);
  glLineWidth(2);

  glPushMatrix();
  glTranslatef(center.x, center.y, 0.f);
  glScalef(50, 50, 0);
  glPushMatrix();
  glRotatef(now_tm->tm_sec * (360.f / 60), 0, 0, 1);
  glDrawLine(0, 0, 0, -1. + .1);
  glPopMatrix();
  glPushMatrix();
  glRotatef(now_tm->tm_min * (360.f / 60), 0, 0, 1);
  glDrawLine(0, 0, 0, -1. + .3);
  glPopMatrix();
  glPushMatrix();
  glRotatef((now_tm->tm_hour + 1) * (360.f / 12), 0, 0, 1);
  glDrawLine(0, 0, 0, -1. + .5);
  glPopMatrix();

  glDrawHollowCircle(0, 0, 1);

  glPopMatrix();
};
void glDrawTree1(GLColorRGB color) {
  glColor3fv((GLfloat *)&color);
  glBegin(GL_TRIANGLES);
  glVertex2f(0, 1);
  glVertex2f(.5, 0);
  glVertex2f(1, 1);
  glEnd();
}
void glDrawTree2(GLColorRGB color) {
  // with snow
  glColor3f(1, 1, 1);
  glBegin(GL_TRIANGLES);
  glVertex2f(0, 1);
  glVertex2f(.45, 0);
  glVertex2f(.45, 1);
  glEnd();
  glColor3fv((GLfloat *)&color);
  glBegin(GL_TRIANGLES);
  glVertex2f(.5, 0);
  glVertex2f(.5, 1);
  glVertex2f(1, 1);
  glEnd();
}
void glDrawTrees() {
  // on land
  glPushMatrix();
  glTranslatef(200, 565, 0);
  glScalef(7, 15, 0);
  glDrawTree1(Hex2glRGB(0x8CAF45));
  glPopMatrix();
  glPushMatrix();
  glTranslatef(210, 570, 0);
  glScalef(10, 15, 0);
  glDrawTree1(Hex2glRGB(0xAFD24A));
  glPopMatrix();
  // on back mountains
  glPushMatrix();
  glTranslatef(150, 450, 0);
  glScalef(10, 20, 0);
  glDrawTree2(Hex2glRGB(0x269971));
  glPopMatrix();
  glPushMatrix();
  glTranslatef(120, 470, 0);
  glScalef(8, 15, 0);
  glDrawTree2(Hex2glRGB(0x269971));
  glPopMatrix();
  glPushMatrix();
  glTranslatef(140, 475, 0);
  glScalef(7, 15, 0);
  glDrawTree1(Hex2glRGB(0x269971));
  glPopMatrix();
  // on front mountains
  glPushMatrix();
  glTranslatef(1170, 420, 0);
  glScalef(10, 20, 0);
  glDrawTree2(Hex2glRGB(0x269971));
  glPopMatrix();
  glPushMatrix();
  glTranslatef(980, 550, 0);
  glScalef(8, 15, 0);
  glDrawTree2(Hex2glRGB(0x269971));
  glPopMatrix();
  glPushMatrix();
  glTranslatef(980, 575, 0);
  glScalef(7, 15, 0);
  glDrawTree1(Hex2glRGB(0x269971));
  glPopMatrix();
  glPushMatrix();
  glTranslatef(140, 475, 0);
  glScalef(7, 15, 0);
  glDrawTree1(Hex2glRGB(0x269971));
  glPopMatrix();
}
void glDrawScene() {
  glDrawGrid(WINDOW_WIDTH, WINDOW_HEIGHT);

  glDrawSky();
  glDrawMountains({
      {{0x0BB79A, 1}, {{0, 505}, {0, 526}, {100, 494}, {56, 450}}},
      {{0x0b947f, 1},
       {
           {50, 500},
           {40, 485},
           {0, 526},
           {100, 494},
           {56, 450},
       }},
      {{0x0BB79A, 1}, {{0, 572}, {440, 572}, {222, 370}, {125, 484}, {0, 526}}},
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
  });
  glDrawRiver(0, WINDOW_HEIGHT - 50, WINDOW_WIDTH, 50);
  glDrawShip();
  glDrawLand();
  glDrawWindMill();
  glDrawMountains({
      {{0x0BB79A, 1},
       {{WINDOW_WIDTH, 340}, {1124, 460}, {WINDOW_WIDTH, WINDOW_HEIGHT}}},
      {{0x0BB79A, 1},
       {{906, WINDOW_HEIGHT},
        {WINDOW_WIDTH, WINDOW_HEIGHT},
        {WINDOW_WIDTH, 472},
        {1033, 418}}},
      {{0x0b947f, 1},
       {
           {1083, 555},
           {1036, 526},
           {975, WINDOW_HEIGHT},
           {WINDOW_WIDTH, WINDOW_HEIGHT},
           {WINDOW_WIDTH, 472},
           {1033, 418},
       }},
  });
  glDrawTrees();
  glDrawClock();
  glDrawBird();
}

void render(GLFWwindow *window) {
  if (glfwWindowShouldClose(window))
    return;

  glClearColor(GL_255U * 51, GL_255U * 34, GL_255U * 114, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glDrawScene();

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
