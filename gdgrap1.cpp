#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;

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
    gladLoadGL();

    string path = "3D/bunny.obj";
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> material;
    string warning, error;

    tinyobj::attrib_t attributes;
    
    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    // get the EBO indices array 
    vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLfloat vertices[] {
        //x    y   z
        0.f, 0.5f, 0.f, // 0
        -0.5f, -0.5f, 0.f, // 1
        0.5f, -0.5f, 0.f // 2
    };

    GLuint indices[]{
        0,1,2
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // currently editing VAO = null
    glBindVertexArray(VAO);
    // currently editing VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        &attributes.vertices[0], // attributes.vertices.data()
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,  // x y z
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float), // array of 3 floats
        (void*)0
    );

    // currently editing VBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // currently editing VBO = EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        &mesh_indices[0], // mesh_indices.data()
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    // currently editing VBO = VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // currently editing VBO = null

    // currently editing VAO = VAO
    glBindVertexArray(0);
    // currently editing VAO = null

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}