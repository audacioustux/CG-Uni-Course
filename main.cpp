#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>

#define PI 3.14159265358
#define PI2 6.28318530718

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

#define GL_255U 1.0f / 255

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
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

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius) {
  int i;
  int lineAmount = 255;

  glBegin(GL_LINE_LOOP);
  for (i = 0; i <= lineAmount; i++) {
    glVertex2f(x + (radius * cos(i * PI2 / lineAmount)),
               y + (radius * sin(i * PI2 / lineAmount)));
  }
  glEnd();
}

void moon() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1, 1, 1, 0.05);
  drawFilledCircle(-1.2, .6, .15);
  glColor4f(1, 1, 1, 0.1);
  drawFilledCircle(-1.2, .6, .12);
  glColor3f(1, 1, 1);
  drawFilledCircle(-1.2, .6, .1);
}
void draw_scene() {
  moon();

  glBegin(GL_LINE_LOOP);
  float coords[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
  glVertex2fv(coords);
  glVertex2fv(coords + 2);
  glVertex2fv(coords + 4);
  glVertex2fv(coords + 6);
  glEnd();
}

void set_framebuffer_size(GLFWwindow *window, int width, int height) {
  float ratio = (float)width / height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, width, height);
}

void render(GLFWwindow *window) {
  if (glfwWindowShouldClose(window))
    return;

  glClearColor(GL_255U * 51, GL_255U * 34, GL_255U * 114, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  draw_scene();

  glfwSwapBuffers(window);

  glfwPollEvents();

  render(window);
}

int main(void) {

  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CG-ASS", NULL, NULL);
  glfwSetWindowTitle(window, u8"Mountain View");
  glfwSetWindowPos(window, 0, 0);
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
