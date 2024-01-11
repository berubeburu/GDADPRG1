#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "JI Lalusin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.45f);
        glVertex2f(-0.4f, 0.15f);
        glVertex2f(-0.25f, -0.3f);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.25f, -0.3f);
        glVertex2f(0.0f, 0.45f);
        glVertex2f(0.25f, -0.3f);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2f(0.25f, -0.3f);
        glVertex2f(0.4f, 0.15f);
        glVertex2f(0.0f, 0.45f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}