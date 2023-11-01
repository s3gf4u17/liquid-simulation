#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <vector>
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
#include <math.h>

#define IMGWIDTH 800
#define IMGHEIGHT 600

class Particle {
public:
    float x,y;
    Particle() {
        x = rand()%200/100.0-1.0;
        y = rand()%200/100.0-1.0;
    }
};

float maxf(float a,float b) {
    if (a>b) return a;
    return b;
}

float minf(float a,float b) {
    if (a>b) return b;
    return a;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTex;\n"
    "out vec2 texCoord;"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   texCoord=vec2(aTex.x,aTex.y);"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 texCoord;"
    "uniform sampler2D texture1;"
    "void main()\n"
    "{\n"
    "   FragColor = texture(texture1,texCoord);\n"
    "}\n\0";

const char *vertexShaderSource2 = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";
const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0);\n"
    "}\n\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader2, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader2);
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader2);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    glDeleteShader(vertexShader2);
    glDeleteShader(fragmentShader2);

    float vertices[] = {
        1,1,0,1,1,  // top right
        1,-1,0,1,0,  // bottom right
        -1,-1,0,0,0,  // bottom left
        1,1,0,1,1,  // top right
        -1,-1,0,0,0,  // bottom left
        -1,1,0,0,1   // top left 
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    std::vector<Particle> particles;
    for (int i=0;i<300;i++) particles.push_back(Particle());

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char data[SCR_HEIGHT*IMGWIDTH*3] = {0};// = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    float smoothradius = 0.2;
    for (int i = 0 ; i < SCR_HEIGHT*IMGWIDTH;i++) {
        float x = (i%IMGWIDTH)/(float)IMGWIDTH*2.0-1.0;
        float y = (i/IMGWIDTH)/(float)IMGHEIGHT*2.0-1.0;
        float value = 0.0;
        for (Particle particle : particles) {
            if (abs(particle.x-x)>smoothradius) continue;
            if (abs(particle.y-y)>smoothradius) continue;
            float dist = sqrt(pow(particle.x-x,2)+pow(particle.y-y,2));
            if (dist>smoothradius) continue;
            float force = maxf(0.0,smoothradius*smoothradius-dist*dist);
            value+=pow(force,3);
        }
        value*=5500;
        if (value<1) {
            data[i*3]=((1-value)*(0.1266)+(value)*(1.0))*255;
            data[i*3+1]=((1-value)*(0.5330)+(value)*(0.9952))*255;
            data[i*3+2]=((1-value)*(0.6886)+(value)*(0.9952))*255;
        } else {
            value-=1.0;
            data[i*3]=(maxf(0,1-value)*(1.0)+(minf(value,1.0))*(0.8301))*255;
            data[i*3+1]=(maxf(0,1-value)*(0.9952)+(minf(value,1.0))*(0.2914))*255;
            data[i*3+2]=(maxf(0,1-value)*(0.9952)+(minf(value,1.0))*(0.2075))*255;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMGWIDTH, IMGHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, texture);

    float *datastream = (float*)malloc(3*sizeof(float)*particles.size());
    for (int i = 0 ; i < particles.size();i++) {
        datastream[3*i] = particles[i].x;
        datastream[3*i+1] = particles[i].y;
        datastream[3*i+2] = 0.0;
    }
    unsigned int VBO2,VAO2;
    glGenVertexArrays(1,&VAO2);
    glGenBuffers(1,&VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER,VBO2);
    glBufferData(GL_ARRAY_BUFFER,3*sizeof(float)*(particles.size()),datastream,GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glEnable(GL_PROGRAM_POINT_SIZE); // GL_VERTEX_PROGRAM_POINT_SIZE
    glfwSwapInterval(1);
    glPointSize(5);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_POINTS,0,particles.size());
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}