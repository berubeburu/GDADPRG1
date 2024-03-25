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
float axis_x_mod = 1;
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
    if (key == GLFW_KEY_D) {  // move to right
        x_mod += 0.1f;
    }
    // when user presses A
    if (key == GLFW_KEY_A) {  // move to left
        x_mod -= 0.1f;
    }
    // when user presses S
    if (key == GLFW_KEY_S) {  // move down
        y_mod -= 0.1f;
    }
    // when user presses W
    if (key == GLFW_KEY_W) {  // move up
        y_mod += 0.1f;
    }
    // when user presses up
    if (key == GLFW_KEY_UP) {   // rotate up
        theta_mod -= 10;
        axis_x_mod = 1;
        axis_y_mod = 0;
    }
    // when user presses down
    if (key == GLFW_KEY_DOWN) { // rotate bunny down
        theta_mod += 10;
        axis_x_mod = 1;
        axis_y_mod = 0;
    }
    // when user presses left
    if (key == GLFW_KEY_LEFT) { // rotate bunny left
        theta_mod -= 10;
        axis_y_mod = 1;
        axis_x_mod = 0;
    }
    // when user presses right
    if (key == GLFW_KEY_RIGHT) {    // rotate bunny right
        theta_mod += 10;
        axis_y_mod = 1;
        axis_x_mod = 0;
    }
    // when user presses Q
    if (key == GLFW_KEY_Q) {    // decrease
        scale_x_mod -= 0.1f;
        scale_y_mod -= 0.1f;
    }
    // when user presses E
    if (key == GLFW_KEY_E) {    // increase
        scale_x_mod += 0.1f;
        scale_y_mod += 0.1f;
    }
    // when user presses Z
    if (key == GLFW_KEY_Z) {    // zoom in
        z_mod -= -0.1f;
    }
    // when user presses X
    if (key == GLFW_KEY_X) {    // zoom out
        z_mod += -0.1f;
    }
}

using namespace std;

int main(void)
{
    float x = 0, y = 3, z = 0, scale_x = 3, scale_y = 3, scale_z = 3, theta = 1, axis_x = 1, axis_y = 0, axis_z = 0;
    float window_width = 600.f;
    float window_height = 600.f;
    glm::mat4 identity_matrix = glm::mat4(1.0f);
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "JI Lalusin", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, window_width, window_height);

    int img_width, img_height, colorChannels;
    stbi_set_flip_vertically_on_load(true);
    // load the texture and fill out the variables
    unsigned char* tex_bytes = stbi_load("3D/brickwall.jpg", &img_width, &img_height, &colorChannels, 0);
    // OpenGL reference to the texture
    GLuint texture;
    // generate a reference
    glGenTextures(1, &texture);
    // set the current texture we're working with on to Texture 0
    glActiveTexture(GL_TEXTURE0);
    // bind out next tasks to tex0 to our current refernce similar to what wer're doing to VBOs
    glBindTexture(GL_TEXTURE_2D, texture);
    // assign the loaded texture to the OpenGL reference
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    // generate the mipmaps to the current texture
    glGenerateMipmap(GL_TEXTURE_2D);
    // free up the loaded bytes
    stbi_image_free(tex_bytes);
    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    int img_width2, img_height2, colorChannels2;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* normal_bytes = stbi_load("3D/brickwall_normal.jpg", &img_width2, &img_height2, &colorChannels2, 0);
    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, img_width2, img_height2, 0, GL_RGB, GL_UNSIGNED_BYTE, normal_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(normal_bytes);
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

    fstream skyboxVertSrc("Shaders/skybox.vert");
    stringstream skyboxVertBuff;
    skyboxVertBuff << skyboxVertSrc.rdbuf();
    string skyboxVertS = skyboxVertBuff.str();
    const char* sky_v = skyboxVertS.c_str();

    fstream skyboxFragSrc("Shaders/skybox.frag");
    stringstream skyboxFragBuff;
    skyboxFragBuff << skyboxFragSrc.rdbuf();
    string skyboxFragS = skyboxFragBuff.str();
    const char* sky_f = skyboxFragS.c_str();

    GLuint vertexShaderSkybox = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderSkybox, 1, &sky_v, NULL);
    glCompileShader(vertexShaderSkybox);

    GLuint fragShaderSkybox = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShaderSkybox, 1, &sky_f, NULL);
    glCompileShader(fragShaderSkybox);

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

    GLuint skyboxShaderProg = glCreateProgram();
    glAttachShader(skyboxShaderProg, vertexShaderSkybox);
    glAttachShader(skyboxShaderProg, fragShaderSkybox);

    glLinkProgram(skyboxShaderProg);
    glDeleteShader(vertexShaderSkybox);
    glDeleteShader(fragShaderSkybox);

    /*
      7--------6
     /|       /|
    4--------5 |
    | |      | |
    | 3------|-2
    |/       |/
    0--------1
    */
//Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    string path = "3D/plane.obj";
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> material;
    string warning, error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

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

    vector<GLuint> mesh_indices;
    vector<glm::vec3> tangents;
    vector<glm::vec3> bitangents;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
        );
        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
        );
        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
        );

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[vData1.texcoord_index * 2],
            attributes.texcoords[(vData1.texcoord_index * 2) + 1]
        );
        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[vData2.texcoord_index * 2],
            attributes.texcoords[(vData2.texcoord_index * 2) + 1]
        );
        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[vData3.texcoord_index * 2],
            attributes.texcoords[(vData3.texcoord_index * 2) + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    // initialize the array of vertex data
    vector<GLfloat> fullVertexData;
    // loop through all the vertex indices
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        // push the X position of the vertex
        // multiply by 3 to get base offset
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]);
        // push the Y position of the vertex
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]);
        // push the Z position of the vertex
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]);

        // push the X position of normal
        // multiply by 3 to get base offset
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);
        // push the Y position of normal
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
        // push the Z position of normal
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]);

        // push the U of the tex coords
        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
        // push the V of the tex coords
        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);

        fullVertexData.push_back(tangents[i].x); // 8 T.x
        fullVertexData.push_back(tangents[i].y); // 9 T.y
        fullVertexData.push_back(tangents[i].z); // 10 T.z
        fullVertexData.push_back(bitangents[i].x); // 11 B.x
        fullVertexData.push_back(bitangents[i].y); // 12 B.y
        fullVertexData.push_back(bitangents[i].z); // 13 B.z
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

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // currently editing VAO = null
    glBindVertexArray(VAO);
    // currently editing VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // new array of vertex data in the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);
    // how to get position data from our array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    // since our UV starts at index 3, or the 4th index of our vertex data
    GLintptr normPtr = 3 * sizeof(float);
    // how to get normals data from our array
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)normPtr);
    // since our UV starts at index 3, or the 4th index of our vertex data
    GLintptr uvPtr = 6 * sizeof(float);
    // how to get UV data from our array
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)uvPtr);

    GLintptr tangentPtr = 8 * sizeof(float);
    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)tangentPtr);
    glVertexAttribPointer(4, 3,  GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)bitangentPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(3);

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    string facesSkybox[]{
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png",
    };

    unsigned int skyboxTex;

    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }
    stbi_set_flip_vertically_on_load(true);

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
    glm::vec3 Center = glm::vec3(0, 3.0f, 0);
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

    glm::vec3 lightPos = glm::vec3(4, 5, 0);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*x = x_mod;
        y = y_mod;
        z = z_mod;
        theta = theta_mod;
        axis_x = axis_x_mod;
        axis_y = axis_y_mod;
        scale_x = scale_x_mod;
        scale_y = scale_y_mod;*/

        theta += 0.04f;

        // Start with the translation matrix
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x, y, z));
        // multiply the resulting matrix with the scale matrix
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_x, scale_y, scale_z));
        // finally multiply it with the rotation matrix
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShaderProg);
        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShaderProg, "view");
        glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));
        unsigned int skyboxProjLoc = glGetUniformLocation(skyboxShaderProg, "projection");
        glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        // tell open GL to use this shader for the VAO/s below
        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        // get the location of tex 0 in the fragment shader
        GLuint texBLoc = glGetUniformLocation(shaderProg, "tex0");
        // tell openGL to use the texture
        glBindTexture(GL_TEXTURE_2D, texture);
        // use the texture at 0
        glUniform1i(texBLoc, 0);

        glActiveTexture(GL_TEXTURE1);
        GLuint tex1Loc = glGetUniformLocation(shaderProg, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(tex1Loc, 1);

        GLuint lightAddress = glGetUniformLocation(shaderProg, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
        GLuint lightColorAddress = glGetUniformLocation(shaderProg, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint ambientStrAddress = glGetUniformLocation(shaderProg, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);
        GLuint ambientColorAddress = glGetUniformLocation(shaderProg, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint cameraPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
        GLuint specStrAddress = glGetUniformLocation(shaderProg, "specStr");
        glUniform1f(specStrAddress, specStr);
        GLuint specPhongAddress = glGetUniformLocation(shaderProg, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        // draw using uv
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}