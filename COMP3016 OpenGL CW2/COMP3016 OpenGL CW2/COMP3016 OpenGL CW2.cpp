// COMP3016 OpenGL CW2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//INCLUDES
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm/ext/vector_float3.hpp"
#include <glm/glm/ext/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "main.h"
#include "LoadShaders.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//NAMESPACES
using namespace std;
using namespace glm;

//GLOBAL VARIABLES
mat4 transform;
mat4 model;
mat4 view;
mat4 projection;

vec3 cameraPosition = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

float deltaTime;
float lastFrame;

//Camera sideways rotation
float cameraYaw = -90.0f;
//Camera vertical rotation
float cameraPitch = 0.0f;
//Determines if first entry of mouse into window
bool mouseFirstEntry = true;
//Positions of camera from given last frame
float cameraLastXPos = 800.0f / 2.0f;
float cameraLastYPos = 600.0f / 2.0f;

//bool signatureLoaded;

//VAO vertex attribute positions in correspondence to vertex attribute type
enum VAO_IDs { Triangles, Indices, Colours, Textures, NumVAOs = 2 };
//VAOs
GLuint VAOs[NumVAOs];

//Buffer types
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
//Buffer objects
GLuint Buffers[NumBuffers];
GLuint program;

//Get time to rotate cubes
float GetTime() {
    return glfwGetTime() * 100;
}

int main()
{
    //Window Resolution
    int windowWidth = 1280;
    int windowHeight = 720;

    //Initialisation of GLFW
    glfwInit();
    //Initialisation of 'GLFWwindow' object
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "CW2 - Dylan Greenwell", NULL, NULL);

    //Checks if window has been successfully instantiated
    if (window == NULL)
    {
        cout << "GLFW Window did not instantiate\n";
        glfwTerminate();
        return -1;
    }

    //Sets cursor to automatically bind to window & hides cursor pointer
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Binds OpenGL to window
    glfwMakeContextCurrent(window);
    glewInit();

    //Load shaders
    ShaderInfo shaders[] =
    {
        { GL_VERTEX_SHADER, "shaders/vertexShader.vert" },
        { GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
        { GL_NONE, NULL }
    };

    program = LoadShaders(shaders);
    glUseProgram(program);

    glViewport(0, 0, 1280, 720);

    glEnable(GL_DEPTH_TEST);

    //Sets the framebuffer_size_callback() function as the callback for the window resizing event
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Sets the mouse_callback() function as the callback for the mouse movement event
    glfwSetCursorPosCallback(window, mouse_callback);

    //VERTICES
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    //CUBE POSITIONS
    vec3 cubePositions[] = {
        vec3(-3.2f,  4.2f,  7.7f),
        vec3(2.0f,  5.0f, -12.0f),
        vec3(-1.5f, -2.2f, -2.5f),
        vec3(-3.8f, -2.0f, -12.3f),
        vec3(2.4f, -0.4f, -3.5f),
        vec3(-1.7f,  3.0f, -7.5f),
        vec3(1.3f, -2.0f, -2.5f),
        vec3(1.5f,  2.0f, -2.5f),
        vec3(1.5f,  0.2f, -1.5f),
        vec3(3.2f,  -0.2f, -10.0f),
        vec3(6.7f,  -4.2f, 1.0f),
        vec3(-1.9f,  1.0f, 4.0f),
        vec3(4.2f,  4.0f, 5.3f),
        vec3(-1.3f,  -3.2f, -4.0f),
        vec3(6.0f,  1.7f, -6.1f)
    };

    //SIGNATURE CUBE POSITION
    vec3 signaturePosition = vec3(0.0f, 0.0f, 0.0f);

    //Sets index of VAO
    glGenVertexArrays(NumVAOs, VAOs);
    //Binds VAO to a buffer
    glBindVertexArray(VAOs[0]);
    //Sets indexes of all required buffer objects
    glGenBuffers(NumBuffers, Buffers);

    //Binds VAO to array buffer
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
    //Allocates buffer memory for the vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Allocates vertex attribute memory for vertex shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //Index of vertex attribute for vertex shader
    glEnableVertexAttribArray(0);
    //Allocates vertex attribute memory for vertex shader
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    //Index of vertex attribute for vertex shader
    glEnableVertexAttribArray(1);

    //Unbinding
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Textures
    glGenTextures(NumBuffers, Buffers);
    glBindTexture(GL_TEXTURE_2D, Buffers[Textures]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    int width;
    int height;
    int colourChannels;
    unsigned char* data = stbi_load("media/bob_signature.jpg", &width, &height, &colourChannels, 0); //actual texture with signature
    //unsigned char* signature = stbi_load("media/signature.jpg", &width, &height, &colourChannels, 0); //signature texture

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Textures failed to load..." << endl;
        return -1;
    }

    /*
    if (signature) {
        signatureLoaded = true;
    }
    else {
        cout << "Signature texture failed to load..." << endl;
        return -1;
    }
    */

    //INDEXES
    GLint modelIndex = glGetUniformLocation(program, "model");
    GLint viewIndex = glGetUniformLocation(program, "view");
    GLint projectionIndex = glGetUniformLocation(program, "projection");
    

    //Render loop
    while (glfwWindowShouldClose(window) == false)
    {
        //Calculate Delta Time
        float currentFrame = static_cast<float>(glfwGetTime() * 3);
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Input
        ProcessUserInput(window); //Takes user input

        //Rendering
        glClearColor(0.25f, 0.0f, 0.0f, 1.0f); //Colour to display on cleared window
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Clears the colour buffer and depth buffer

        //Transform/MVP
        
        view = mat4(1.0f);
        projection = mat4(1.0f);

        view = lookAt(cameraPosition, cameraPosition+cameraFront, cameraUp);

        projection = perspective(radians(45.0f), (float)(windowWidth / windowHeight), 0.1f, 100.f);

        glUniformMatrix4fv(viewIndex, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(projectionIndex, 1, GL_FALSE, value_ptr(projection));

        glBindTexture(GL_TEXTURE_2D, Buffers[Textures]);
        glBindVertexArray(VAOs[0]); //Bind buffer object to render

        //For each cube
        for (int i = 0; i < 15; i++) {
            model = mat4(1.0f);
            model = translate(model, cubePositions[i]);

            float angle = 17.0f * i;

            model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f));
            model = rotate(model, radians(GetTime()), cubePositions[i]);
            glUniformMatrix4fv(modelIndex, 1, GL_FALSE, value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        //Signature Cube
        model = mat4(1.0f);
        model = translate(model, signaturePosition);
        model = rotate(model, radians(180.0f), vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(modelIndex, 1, GL_FALSE, value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /*
        if (signatureLoaded) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, signature);
            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, Buffers[Textures]);
            glBindVertexArray(VAOs[0]); //Bind buffer object to render

            model = mat4(1.0f);
            model = translate(model, signaturePosition);

            glUniformMatrix4fv(modelIndex, 1, GL_FALSE, value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        */

        //Refreshing
        glfwSwapBuffers(window); //Swaps the colour buffer
        glfwPollEvents(); //Queries all GLFW events
    }

    //Safely terminates GLFW
    glfwTerminate();

    return 0;
}


//FUNCTIONS
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //Resizes window based on contemporary width & height values
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //Initially no last positions, so sets last positions to current positions
    if (mouseFirstEntry)
    {
        cameraLastXPos = (float)xpos;
        cameraLastYPos = (float)ypos;
        mouseFirstEntry = false;
    }

    //Sets values for change in position since last frame to current frame
    float xOffset = (float)xpos - cameraLastXPos;
    float yOffset = cameraLastYPos - (float)ypos;

    //Sets last positions to current positions for next frame
    cameraLastXPos = (float)xpos;
    cameraLastYPos = (float)ypos;

    //Moderates the change in position based on sensitivity value
    const float sensitivity = 0.025f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    //Adjusts yaw & pitch values against changes in positions
    cameraYaw += xOffset;
    cameraPitch += yOffset;

    //Prevents turning up & down beyond 90 degrees to look backwards
    if (cameraPitch > 89.0f)
    {
        cameraPitch = 89.0f;
    }
    else if (cameraPitch < -89.0f)
    {
        cameraPitch = -89.0f;
    }

    //Modification of direction vector based on mouse turning
    vec3 direction;
    direction.x = cos(radians(cameraYaw)) * cos(radians(cameraPitch));
    direction.y = sin(radians(cameraPitch));
    direction.z = sin(radians(cameraYaw)) * cos(radians(cameraPitch));
    cameraFront = normalize(direction);
}

void ProcessUserInput(GLFWwindow* WindowIn)
{
    //Closes window on 'exit' key press
    if (glfwGetKey(WindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        //cout << "Escape Key Pressed..." << endl;
        glfwSetWindowShouldClose(WindowIn, true);
    }

    //Camera Movement

    const float movementSpeed = 1.0f * deltaTime;

    if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS)
    {
        //cout << "W Key Pressed..." << endl;
        cameraPosition += movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS)
    {
        //cout << "A Key Pressed..." << endl;
        cameraPosition -= normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS)
    {
        //cout << "S Key Pressed..." << endl;
        cameraPosition -= movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS)
    {
        //cout << "D Key Pressed..." << endl;
        cameraPosition += normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
}