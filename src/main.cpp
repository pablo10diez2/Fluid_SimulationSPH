#include "config.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <glm/glm.hpp>
#include <vector>
#include "gravity.h"

unsigned int make_module(const std::string& filepath, unsigned int module_type);
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
void buildCircle(float radius, int count, float xUser, float yUser, float speed);
void rebuildCenters(int count);
void reBuildCircle(float radius, int count, float xUser, float yUser);

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

std::vector<float> vertices;
std::vector<float> speeds;
std::vector<float> radiusArray;
std::vector<float> centers;

int numCircles;
bool nuevo;

int main(){
    GLFWwindow* window;
    
    if(!glfwInit()){
        std::cout<<"GLFW could not start";
    }
    
    window = glfwCreateWindow(640, 640, "MyWindow", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    unsigned int shader = make_shader("../src/shaders/vertex.txt", "../src/shaders/fragment.txt");
    
    int numTriangles = 38;
    numCircles = 0;

    buildCircle(0.15f, numTriangles, 0.3f, 0.3f, -0.0001f);
    buildCircle(0.3f, numTriangles, -0.2f, 0.6f, -0.005f);

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Frame rate
    float previousTime = 0.0f;
    float currentTime = 0.0f;
    float timeDiff;

    //gravity
    float previousTimeG = 0.0f;
    float currentTimeG = 0.0f;
    float timeDiffG;

    unsigned int counter = 0;

    int numTrianglesReal = numTriangles-2;

    while(!glfwWindowShouldClose(window)){
        if(nuevo){
            buildCircle(0.15f, numTriangles, 0.0f, 0.8f, -0.0001f);
            nuevo = false;
        }

        glfwPollEvents();

        //Frame rate
        currentTime = glfwGetTime();
        timeDiff = currentTime-previousTime;
        counter++;

        if(timeDiff >= 1.0f/10.0f){
            float fps = (1.0f/timeDiff)*counter;
            int fpsInt = fps;

            std::string fpsString = std::to_string(fpsInt);
            std::string newTitle = "Engine   "+ fpsString + "FPS";
            glfwSetWindowTitle(window, newTitle.c_str());
            previousTime = currentTime;
            counter = 0;
        }

        currentTimeG = glfwGetTime();
        timeDiffG = currentTimeG - previousTimeG;

        ballCollisions(centers, numCircles, speeds);
        isEdge(centers, numCircles, speeds);
        gravity(centers, numCircles, speeds, timeDiffG);

        rebuildCenters(numTrianglesReal);
        
        for(int i = 0; i< speeds.size(); i++){
            std::cout<<"speed"<<i<<":"<<speeds[i]<<std::endl;
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size()/6);
        glfwSwapBuffers(window);
        
        GLenum err = glGetError();

        if(err != GL_NO_ERROR){
            std::cout<<err<<std::endl;
            break;
            std::cout<<vertices.size();
        }
    }
    
    previousTimeG = currentTimeG;

    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void buildCircle(float radius, int count, float xUser, float yUser, float speed){
    radiusArray.push_back(radius);
    centers.push_back(xUser);
    centers.push_back(yUser);

    float angle = 360.0f/count;
    int triangleCount = count-2;

    std::vector<glm::vec3> temp;
    speeds.push_back(speed);
    for(int i=0; i<count; i++){
        float currentAngle = angle*i;
        float x = (radius * cos(glm::radians(currentAngle)))+xUser;
        float y = (radius * sin(glm::radians(currentAngle)))+yUser;
        float z = 0.0f;

        temp.push_back(glm::vec3(x,y,z));
    }
    for(int i=0; i < triangleCount; i++){
        glm::vec3 v0 = temp[0];
        glm::vec3 v1 = temp[i+1];
        glm::vec3 v2 = temp[i+2];
        vertices.insert(vertices.end(), {v0.x, v0.y, v0.z, 1.0f, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {v1.x, v1.y, v1.z, 1.0f, 1.0f, 1.0f});
        vertices.insert(vertices.end(), {v2.x, v2.y, v2.z, 1.0f, 1.0f, 1.0f});
    }
    numCircles++;
}
void reBuildCircle(float radius, int count, float xUser, float yUser){
    float angle = 360.0f/count;
    int triangleCount = count-2;

    std::vector<glm::vec3> temp;
    for(int i=0; i<count; i++){
        float currentAngle = angle*i;
        float x = (radius * cos(glm::radians(currentAngle)))+xUser;
        float y = (radius * sin(glm::radians(currentAngle)))+yUser;
        float z = 0.0f;

        temp.push_back(glm::vec3(x,y,z));
    }
    for(int i=0; i < triangleCount; i++){
        glm::vec3 v0 = temp[0];
        glm::vec3 v1 = temp[i+1];
        glm::vec3 v2 = temp[i+2];
        vertices.insert(vertices.end(), {v0.x, v0.y, v0.z, 1.0f, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {v1.x, v1.y, v1.z, 1.0f, 1.0f, 1.0f});
        vertices.insert(vertices.end(), {v2.x, v2.y, v2.z, 1.0f, 1.0f, 1.0f});
    }
}
void rebuildCenters(int count){
    vertices.clear();
    for(int i=0; i<numCircles; i++){
        reBuildCircle(radiusArray[i], count, centers[2*i], centers[2*i+1]);
    }
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath){
    std::vector<unsigned int> modules;

        modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
        modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));
        
        unsigned int shader = glCreateProgram();
        
        for (unsigned int shaderModule : modules){
            glAttachShader(shader, shaderModule);
        }

        glLinkProgram(shader);
        
        int success;
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success){
            char error[1024];
            glGetProgramInfoLog(shader, 1024, NULL, error);
            std::cout<<error<<std::endl;
        }

        for (unsigned int shaderModule : modules){
            glDeleteShader(shaderModule);
        }
        return shader;
}

unsigned int make_module(const std::string& filepath, unsigned int module_type){
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);

    while(std::getline(file, line)){
        bufferedLines << line << "\n";
    }

    std::string shaderSource = bufferedLines.str();
    const char *shaderSrc = shaderSource.c_str();
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);

    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);

    if (!success) {
        char error[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, error);
        std::cout << "Shader compilation error (" << filepath << "):\n" << error << std::endl;
    }

    return shaderModule;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    if(button==GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        nuevo = true;
    }
}
