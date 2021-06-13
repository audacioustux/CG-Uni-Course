#include <GLFW/glfw3.h>
#include <math.h>

#define PI 3.14159265358
#define PI2 6.28318530718

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
  int i;
  int triangleAmount = 20;

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
  int lineAmount = 100;

  glBegin(GL_LINE_LOOP);
  for (i = 0; i <= lineAmount; i++) {
    glVertex2f(x + (radius * cos(i * PI2 / lineAmount)),
               y + (radius * sin(i * PI2 / lineAmount)));
  }
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

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(window);

  glfwPollEvents();

  render(window);
}

int main(void) {
  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CG-ASS", NULL, NULL);

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
