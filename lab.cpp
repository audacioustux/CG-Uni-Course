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

#define WINDOW_WIDTH 950
#define WINDOW_HEIGHT 470

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
  float coords[] = {0, 0, 0, 1, 1, 1, 1, 0};
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
void glDrawGrid(GLfloat width, GLfloat height) {
  glLineWidth(1);
  glColor4f(0, 0, 0, 0.05);
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
void glDrawWindowFrame() {
  glLineWidth(5);
  GLColorRGB rgbddf1fa = Hex2glRGB(0x000000);
  glColor3fv((GLfloat *)&rgbddf1fa);
  glBegin(GL_LINE_LOOP);
  float coords[] = {146, 260, 146, 338, 206, 338, 206, 260};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
}
void glDrawWindow() {
  GLColorRGB rgbddf1fa = Hex2glRGB(0xddf1fa);
  glColor3fv((GLfloat *)&rgbddf1fa);
  glBegin(GL_QUADS);
  float coords[] = {146, 260, 146, 338, 206, 338, 206, 260};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
}
void glDrawDoor() {
  glLineWidth(1);
  glColor3f(0, 0, 0);
  glBegin(GL_LINE_LOOP);
  float coords[] = {326, 240, 326, 425, 422, 425, 422, 240};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
  float b_coords[] = {326, 425, 326, 432, 442, 432, 442, 425};
  glBegin(GL_LINE_LOOP);
  glVertex2fv(b_coords);
  glVertex2fv(b_coords + 2);
  glVertex2fv(b_coords + 4);
  glVertex2fv(b_coords + 6);
  glEnd();
}
void glDrawGarage() {
  glLineWidth(1);
  GLColorRGB rgbc8c8c8 = Hex2glRGB(0xc8c8c8);
  glColor3fv((GLfloat *)&rgbc8c8c8);
  glBegin(GL_QUADS);
  float coords[] = {474, 256, 474, 426, 826, 426, 826, 256};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
  glColor3f(0, 0, 0);
  glLineWidth(2);
  glBegin(GL_LINE_LOOP);
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
  glLineWidth(1);
  for (float y = coords[1]; y <= coords[3]; y += 20) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(coords[0], y);
    glVertex2f(coords[4], y);
    glEnd();
  };
}
void glDrawRooftop() {
  GLColorRGB rgbb6a898 = Hex2glRGB(0xb6a898);
  glColor3fv((GLfloat *)&rgbb6a898);
  glBegin(GL_TRIANGLES);
  float coords[] = {45, 215, 915, 215, 480, 55};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glEnd();
  glColor3f(0, 0, 0);
  glLineWidth(2);
  glBegin(GL_LINE_LOOP);
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glEnd();
}
void glDrawCeiling() {
  GLColorRGB rgbb6a898 = Hex2glRGB(0x2e313a);
  glColor3fv((GLfloat *)&rgbb6a898);
  glLineWidth(5);
  glBegin(GL_LINES);
  glVertex2f(35, 218);
  glVertex2f(925, 218);
  glEnd();
  glLineWidth(5);
  glBegin(GL_LINES);
  glVertex2f(45, 222);
  glVertex2f(915, 222);
  glEnd();
}
void glDrawScene(GLFWwindow *window) {
  glDrawGrid(WINDOW_WIDTH, WINDOW_HEIGHT);

  // bottom slab
  glColor3f(0, 0, 0);
  glLineWidth(5);
  glBegin(GL_LINES);
  glVertex2f(20, 430);
  glVertex2f(930, 430);
  glEnd();

  GLColorRGB rgb848388 = Hex2glRGB(0x848388);
  GLColorRGB rgbb5a999 = Hex2glRGB(0xb5a999);

  // slab on top the base slab
  glColor3f(0, 0, 0);
  glLineWidth(7);
  glBegin(GL_LINES);
  glVertex2f(44, 425);
  glVertex2f(90, 425);
  glEnd();
  glColor3fv((GLfloat *)&rgb848388);
  glLineWidth(3);
  glBegin(GL_LINES);
  glVertex2f(44, 425);
  glVertex2f(90, 425);
  glEnd();

  // house front
  glColor3fv((GLfloat *)&rgbb5a999);
  glBegin(GL_QUADS);
  float coords[] = {90, 225, 90, 427, 872, 427, 872, 225};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
  glColor3f(0, 0, 0);
  glLineWidth(2);
  glBegin(GL_LINE_LOOP);
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();

  // divider
  glColor3f(0, 0, 0);
  glLineWidth(2);
  glBegin(GL_LINES);
  glVertex2f(442, 225);
  glVertex2f(442, 427);
  glEnd();

  // window
  glDrawWindow();
  glDrawWindowFrame();

  // door
  glDrawDoor();

  // garage
  glDrawGarage();

  // rooftop
  glDrawRooftop();

  // ceiling
  glDrawCeiling();
}

void render(GLFWwindow *window) {
  if (glfwWindowShouldClose(window))
    return;

  glClearColor(1, 1, 1, 1.0f);
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
