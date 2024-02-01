#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// modifier for the model's x position
float x_mod = 0;
// modifier for the model's y position
float y_mod = 0;
// modifier for the model's z position
float z_mod = -2.f;

void Key_CallBack(GLFWwindow* window, // pointer to the window
    int key, // keycode of the press
    int scancode, // physical position of the press
    int action, // either press / release
    int mods) // which modifier keys is held down
{
    // when user presses D
    if (key == GLFW_KEY_D &&
        action == GLFW_PRESS) {
        // move bunny to right
        x_mod += 0.1f;
    }
    // when user presses A
    if (key == GLFW_KEY_A &&
        action == GLFW_PRESS) {
        // move bunny to left
        x_mod -= 0.1f;
    }
    // when user presses S
    if (key == GLFW_KEY_S &&
        action == GLFW_PRESS) {
        // move bunny down
        z_mod += -0.3f;
    }
    // when user presses W
    if (key == GLFW_KEY_W &&
        action == GLFW_PRESS) {
        // move bunny up
        z_mod -= -0.3f;
    }
}

using namespace std;

int main(void)
{
    glm::mat4 identity_matrix = glm::mat4(1.0f);

    float x = 0, y = 0, z = 0, scale_x = 1, scale_y = 1, scale_z = 1, theta = 90, axis_x = 0, axis_y = 1, axis_z = 0;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float window_width = 600.f;
    float window_height = 600.f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "JI Lalusin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //glViewport(0, 0, 300, 600);

    // set the callback function to the window
    glfwSetKeyCallback(window, Key_CallBack);

    // load the shader file into a string stream        vertex
    fstream vertSrc("Shaders/sample.vert");
    stringstream vertBuff;
    // add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();
    // convert the stream to a character array
    string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    // load the shader file into a string stream        fragment
    fstream fragSrc("Shaders/sample.frag");
    stringstream fragBuff;
    // add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();
    // convert the stream to a character array
    string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    // create a vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // assign the source to the vertex shader
    glShaderSource(vertexShader, 1, &v, NULL);
    // compile the vertex shader
    glCompileShader(vertexShader);

    // create a fragment shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    // assign the source to the fragment shader
    glShaderSource(fragShader, 1, &f, NULL);
    // compile the fragment shader
    glCompileShader(fragShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        vector<GLchar> errorLog(maxLength);
        // cout << errorLog[maxLength];
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(vertexShader); // Don't leak the shader.
    }

    // create the shader program
    GLuint shaderProg = glCreateProgram();
    // attach the compiled vertex shader
    glAttachShader(shaderProg, vertexShader);
    // attach the compiled fragment shader
    glAttachShader(shaderProg, fragShader);

    // finalize the compilation process
    glLinkProgram(shaderProg);

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

    /*glm::mat4 projectionMatrix = glm::ortho(
        -2.f, // left
        2.f, // right
        -2.f, // bottom
        2.f, // top
        -1.f, // znear
        1.f // zfar
    );*/

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f), // FOV
        window_height / window_width, // aspect ratio
        0.1f, // znear > 0
        100.f // zfar
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        z = z_mod;

        // Start with the translation matrix
        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix,
            glm::vec3(x, y, z)
        );

        // multiply the resulting matrix with the scale matrix
        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        // finally multiply it with the rotation matrix
        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
        );

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // get the location of the transform variable in the shader
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        // assign the matrix
        glUniformMatrix4fv(transformLoc, // address of the transform variable
            1, // how many matrices to assign
            GL_FALSE, // transpose
            glm::value_ptr(transformation_matrix)); // pointer to the matrix

        // tell open GL to use this shader for the VAO/s below
        glUseProgram(shaderProg);
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