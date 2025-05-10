#include "Vector.h"
#include "config.h"

using namespace std;

int main(){
    GLFWwindow *window;
    
    if(!glfwInit()){
        std::cout<<"GLFW could not start";
    }
    
    window = glfwCreateWindow(640, 480, "MyWindow", NULL, NULL);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
