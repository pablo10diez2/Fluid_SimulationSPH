#include "config.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <glm/glm.hpp>
#include <vector>
#include "gravity.h"
#include <unordered_map>
#include <map>
#include "fluid.h"

unsigned int make_module(const std::string& filepath, unsigned int module_type);
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
void buildCircle(int count, float xUser, float yUser, float speed1, float speed2);
void rebuildCenters(int count);
void reBuildCircle(int count, float xUser, float yUser, int index);
void selectColor(float* red, float* blue, float pressure);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

std::vector<float> vertices; 
std::vector<float> speeds;
std::vector<float> centers;
std::vector<float> densities;
std::vector<float> pressures;
std::vector<float> pressureForces;
std::vector<float> viscosities;

std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash> grid;
std::unordered_map<std::pair<int, int>, float, pairHash> distances;

unsigned int numCircles;
unsigned int numTriangles;
bool newCircle;
float radius = 0.02f;
float mass  = 1.0f;
float h = 0.2f;

int main(){
    GLFWwindow* window;
    
    if(!glfwInit()){
        std::cout<<"GLFW could not start";
    }

    unsigned short windowSizeX = 700;
    unsigned short windowSizeY = 700;

    window = glfwCreateWindow(windowSizeX, windowSizeY, "MyWindow", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    unsigned int shader = make_shader("../src/shaders/vertex.txt", "../src/shaders/fragment.txt");
    
    numTriangles = 20;
    numCircles = 0;
    
    float xCircle = -0.95f;
    float yCircle = 0.5f;
    float xSpeed = 0.15f;
    float ySpeed = 0.0f;

    for(int i=0; i<20; i++){
        buildCircle(numTriangles, xCircle, yCircle, xSpeed, ySpeed);
        xCircle += 0.08f;
        yCircle = 0.5f;
        for(int j=0; j<13; j++){
            buildCircle(numTriangles, xCircle, yCircle, xSpeed, ySpeed);
            yCircle -=0.08f;
        }
    }
    
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

    //glfwSwapInterval(0);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        //Frame rate
        currentTime = glfwGetTime();
        timeDiff = currentTime-previousTime;
        counter++;

        if(timeDiff >= 1.0f/10.0f){
            float fps = (1.0f/timeDiff)*counter;
            int fpsInt = fps;

            std::string fpsString = std::to_string(fpsInt);
            std::string newTitle = "Engine   "+ fpsString + "FPS" + " | Balls: "+std::to_string(numCircles);
            glfwSetWindowTitle(window, newTitle.c_str());
            previousTime = currentTime;
            counter = 0;

        }

        if(newCircle){
            float x = 0.8f;
            for(int i=0; i<5; i++){
                buildCircle(numTriangles, x, 0.8, -0.01, -0.04);
                x -= 0.1;
            }
            newCircle = false;
        }

        currentTimeG = glfwGetTime();
        timeDiffG = currentTimeG - previousTimeG;

        findNeighbors(centers, grid, numCircles);
        searchDistances(distances, numCircles, centers, grid);

        calculateDensities(numCircles, centers, grid, densities, distances);
        calculatePressures(numCircles, pressures, densities);
        calculatePressureForce(numCircles, centers, grid, pressureForces, pressures, densities, distances);
        calculateViscosity(numCircles, viscosities, centers, grid, speeds, densities, distances);
        
        applyForces(numCircles, timeDiffG, centers, speeds, pressureForces, viscosities, densities);
        isEdge(centers, numCircles, speeds);

        rebuildCenters(numTrianglesReal);
    
        for(int i=0; i<numCircles; i++){
            std::cout<<"Density: "<<densities[i]<<std::endl;
            std::cout<<"Pressure: "<<pressures[i]<<std::endl;
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
        previousTimeG = currentTimeG;

    }   

    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void buildCircle(int count, float xUser, float yUser, float speed1, float speed2){
    centers.push_back(xUser);
    centers.push_back(yUser);

    float angle = 360.0f/count;
    int triangleCount = count-2;

    std::vector<glm::vec3> temp;
    speeds.push_back(speed1);
    speeds.push_back(speed2);
    for(int i=0; i<count; i++){
        float currentAngle = angle*i;
        float x = (radius * cos(glm::radians(currentAngle)))+xUser;
        float y = (radius * sin(glm::radians(currentAngle)))+yUser;
        float z = 0.0f;

        temp.push_back(glm::vec3(x,y,z));
    }

    float red = 0.0f;
    float blue = 1.0f;

    //selectColor(&red, &blue, speed1, speed2);

    for(int i=0; i < triangleCount; i++){
        glm::vec3 v0 = temp[0];
        glm::vec3 v1 = temp[i+1];
        glm::vec3 v2 = temp[i+2];
        vertices.insert(vertices.end(), {v0.x, v0.y, v0.z, red, 0.0f, blue});
        vertices.insert(vertices.end(), {v1.x, v1.y, v1.z, red, 0.0f, blue});
        vertices.insert(vertices.end(), {v2.x, v2.y, v2.z, red, 0.0f, blue});
    }
    numCircles++;
}
void reBuildCircle(int count, float xUser, float yUser, int index){
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

    float red = 0.0f;
    float blue = 1.0f;

    //selectColor(&red, &blue, pressures[index]);

    for(int i=0; i < triangleCount; i++){
        glm::vec3 v0 = temp[0];
        glm::vec3 v1 = temp[i+1];
        glm::vec3 v2 = temp[i+2];
        vertices.insert(vertices.end(), {v0.x, v0.y, v0.z, red, 0.0f, blue});
        vertices.insert(vertices.end(), {v1.x, v1.y, v1.z, red, 0.0f, blue});
        vertices.insert(vertices.end(), {v2.x, v2.y, v2.z, red, 0.0f, blue});
    }
}

void rebuildCenters(int count){
    vertices.clear();
    for(int i=0; i<numCircles; i++){
        reBuildCircle(count, centers[2*i], centers[2*i+1], i);
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
        newCircle = !newCircle;
    }
}

void selectColor(float* red, float* blue, float pressure){
    if(pressure>1.0f){
        *red = 1.0f;
        *blue = 0.0f;
    }else if(pressure<-1.0f){
        *red = 0.0f;
        *blue = 0.0f;
    }else{
        float eq = (pressure-(-1.0f))/(1.0f-(-1.0f));
        *red = eq/2;
        *blue = 2*(1-eq);
    }
}
