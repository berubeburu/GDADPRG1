#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// modifier for the model's x position
float x_mod = 0;
// modifier for the model's y position
float y_mod = 0;
// modifier for the model's z position
float z_mod = -2.f;
// modifier for the model's theta
float theta_mod = 0;
// modifier for the model's x axis
float axis_x_mod = 0;
// modifier for the model's y axis
float axis_y_mod = 0;
// modifier for the model's x scale
float scale_x_mod = 1;
// modifier for the model's y scale
float scale_y_mod = 1;

void Key_CallBack(GLFWwindow* window, // pointer to the window
    int key, // keycode of the press
    int scancode, // physical position of the press
    int action, // either press / release
    int mods) // which modifier keys is held down
{
    // when user presses D
    if (key == GLFW_KEY_D) {
        // move bunny to right
        x_mod += 0.1f;
    }
    // when user presses A
    if (key == GLFW_KEY_A) {
        // move bunny to left
        x_mod -= 0.1f;
    }
    // when user presses S
    if (key == GLFW_KEY_S) {
        // move bunny down
        y_mod -= 0.1f;
    }
    // when user presses W
    if (key == GLFW_KEY_W) {
        // move bunny up
        y_mod += 0.1f;
    }
    // when user presses up
    if (key == GLFW_KEY_UP) {
        // rotate bunny up
        theta_mod -= 10;
        axis_x_mod = 1;
        axis_y_mod = 0;
    }
    // when user presses down
    if (key == GLFW_KEY_DOWN) {
        // rotate bunny down
        theta_mod += 10;
        axis_x_mod = 1;
        axis_y_mod = 0;
    }
    // when user presses left
    if (key == GLFW_KEY_LEFT) {
        // rotate bunny left
        theta_mod -= 10;
        axis_y_mod = 1;
        axis_x_mod = 0;
    }
    // when user presses right
    if (key == GLFW_KEY_RIGHT) {
        // rotate bunny right
        theta_mod += 10;
        axis_y_mod = 1;
        axis_x_mod = 0;
    }
    // when user presses Q
    if (key == GLFW_KEY_Q) {
        // decrease bunny
        scale_x_mod -= 0.1f;
        scale_y_mod -= 0.1f;
    }
    // when user presses E
    if (key == GLFW_KEY_E) {
        // increase bunny
        scale_x_mod += 0.1f;
        scale_y_mod += 0.1f;
    }
    // when user presses Z
    if (key == GLFW_KEY_Z) {
        // zoom in
        z_mod -= -0.1f;
    }
    // when user presses X
    if (key == GLFW_KEY_X) {
        // zoom out
        z_mod += -0.1f;
    }
}

using namespace std;

int main(void)
{
    glm::mat4 identity_matrix = glm::mat4(1.0f);

    float x = 0, y = 0, z = 0, scale_x = 1, scale_y = 1, scale_z = 1, theta = 0, axis_x = 0, axis_y = 0, axis_z = 1;

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

    glViewport(0, 0, window_width, window_height);

    int img_width, // width of texture
        img_height, // height of texture
        colorChannels; // number of color channels

    // fix the flipped texture
    stbi_set_flip_vertically_on_load(true);

    // load the texture and fill out the variables
    unsigned char* tex_bytes =
        stbi_load("3D/ayaya.png", // texture path
            &img_width, // fills out the width
            &img_height, // fills out the height
            &colorChannels,// fills out the color channel
            0);

    // OpenGL reference to the texture
    GLuint texture;
    // generate a reference
    glGenTextures(1, &texture);
    // set the current texture we're working with on to Texture 0
    glActiveTexture(GL_TEXTURE0);
    // bind out next tasks to tex0 to our current refernce similar to what wer're doing to VBOs
    glBindTexture(GL_TEXTURE_2D, texture);

    // assign the loaded texture to the OpenGL reference
    glTexImage2D(GL_TEXTURE_2D,
        0, // texture 0
        GL_RGBA, // target color format of the texture
        img_width, // texture width
        img_height, // texture height
        0,
        GL_RGBA, // color format of the texture
        GL_UNSIGNED_BYTE,
        tex_bytes); // loaded texture in bytes

    // generate the mipmaps to the current texture
    glGenerateMipmap(GL_TEXTURE_2D);
    // free up the loaded bytes
    stbi_image_free(tex_bytes);

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

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

    string path = "3D/myCube.obj";
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

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };

    // get the EBO indices array 
    vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLfloat vertices[]{
        //x    y   z
        0.f, 0.5f, 0.f, // 0
        -0.5f, -0.5f, 0.f, // 1
        0.5f, -0.5f, 0.f // 2
    };

    GLuint indices[]{
        0,1,2
    };

    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO_UV);

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

    // bind the UV buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])), // float * size of the UV array
        &UV[0], // the UV array earlier
        GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        2, // 2 for the UV or tex coords
        2, // UV
        GL_FLOAT, // type of array
        GL_FALSE,
        2 * sizeof(float), // every 2 index
        (void*)0
    );

    // enable 2 
    glEnableVertexAttribArray(2);

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

    // position of the camera in the world / eye
    glm::vec3 cameraPos = glm::vec3(x_mod, 0, 10.f);

    // construct the position matrix using the eye
    glm::mat4 cameraPositionMatrix =
        glm::translate(glm::mat4(1.0f), // initialize as identity matrix
            cameraPos * -1.0f); // multiply to -1 since we need -P

    // world's up direction; normally just 1 in Y
    glm::vec3 WorldUp = glm::vec3(0, 1.0f, 0);
    // camera's center
    glm::vec3 Center = glm::vec3(0, 3.f, 0);
    // get the forward
    glm::vec3 F = glm::vec3(Center - cameraPos);
    // normalize the forward
    F = glm::normalize(F);
    // get the right
    glm::vec3 R = glm::normalize(glm::cross(F, WorldUp));
    // get the up
    glm::vec3 U = glm::normalize(glm::cross(R, F));

    // construct the orientation matrix
    glm::mat4 cameraOrientation = glm::mat4(1.f);

    // manually assign the matrix[column][row]
    cameraOrientation[0][0] = R.x;
    cameraOrientation[1][0] = R.y;
    cameraOrientation[2][0] = R.z;

    cameraOrientation[0][1] = U.x;
    cameraOrientation[1][1] = U.y;
    cameraOrientation[2][1] = U.z;

    cameraOrientation[0][2] = -F.x;
    cameraOrientation[1][2] = -F.y;
    cameraOrientation[2][2] = -F.z;

    // camera view matrix
    //glm::mat4 viewMatrix = cameraOrientation * cameraPositionMatrix;
    glm::mat4 viewMatrix = glm::lookAt(cameraPos, Center, WorldUp);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        x = x_mod;
        y = y_mod;
        z = z_mod;
        theta = theta_mod;
        axis_x = axis_x_mod;
        axis_y = axis_y_mod;
        scale_x = scale_x_mod;
        scale_y = scale_y_mod;

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

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, // address of the variable
            1, // how many values are we modifying
            GL_FALSE,
            glm::value_ptr(viewMatrix)); // view matrix

        // tell open GL to use this shader for the VAO/s below
        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        // get the location of tex 0 in the fragment shader
        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        // tell openGL to use the texture
        glBindTexture(GL_TEXTURE_2D, texture);
        // use the texture at 0
        glUniform1i(tex0Address, 0);

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