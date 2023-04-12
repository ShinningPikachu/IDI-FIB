    // MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;



void MyGLWidget::carregaShaders(){
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(), "proj");
    projectTransform();
    viewLoc = glGetUniformLocation(program->programId(), "view");
    viewTransform();
}
void MyGLWidget::Calcula(){
    minim.x = maxim.x = m.vertices()[0];
    minim.y = maxim.y = m.vertices()[1];
    minim.z = maxim.z = m.vertices()[2];
    for (unsigned int i = 0; i < m.vertices().size(); i+= 3){
        if(m.vertices()[i] > maxim.x){
            maxim.x = m.vertices()[i];
        }else if (m.vertices()[i] < minim.x){
            minim.x = m.vertices()[i];
        }
        if(m.vertices()[i+1] > maxim.y){
            maxim.y = m.vertices()[i+1];
        }else if (m.vertices()[i+1] < minim.y){
            minim.y = m.vertices()[i];
        }
        if(m.vertices()[i+2] > maxim.z){
            maxim.z = m.vertices()[i+2];
        }else if (m.vertices()[i+2] < minim.z){
            minim.z = m.vertices()[i+2];
        }
    }
    std::cout << minim.x << " " << minim.y << " " << minim.z << endl;
    std::cout << maxim.x << " " << maxim.y << " " << maxim.z << endl;
}
void MyGLWidget::iniCamara(){
    
}
void MyGLWidget::iniCaja(){
    
}
void MyGLWidget::viewTransform(){
    glm::mat4 view = glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0) );
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}


void MyGLWidget::projectTransform(){
    glm::mat4 Proj = glm::perspective(float(M_PI)/2.0f, 1.0f, 0.4f, 3.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}


void MyGLWidget::initializeGL (){
    //BL2GLWidget::initializeGL();
    
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.5, 0.7, 1.0, 1.0);
    carregaShaders();
    
    creaBuffers();
    
    creaBuffers_Terra();
    escala = 1.0f;
}

void MyGLWidget::creaBuffers_Terra(){
    glm::vec3 posicio[9] = {
	glm::vec3(-2.0, -1.0, 2.0),
	glm::vec3( -2, -1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3( 2.0, -1.0, 2.0),
	glm::vec3( -2.0, -1.0, 2.0),
    };
    glm::vec3 color[9] = {
	glm::vec3(1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,0,1),
	glm::vec3(0,0,1),
	glm::vec3(0,1,0),
	glm::vec3(1,0,0),
    };
    glGenVertexArrays(1, &VAO_T);
    glBindVertexArray(VAO_T);
    GLuint VBO_T[2];
    glGenBuffers(2, VBO_T);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_T[0]);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(posicio),posicio,GL_STATIC_DRAW);
    
    //activem vertexloc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_T[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
    
    //activem colorloc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
    
    glBindVertexArray(0);
}

void MyGLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //trans models
    modelTransform();
    
    glBindVertexArray(VAO_H);
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
    
    glBindVertexArray(VAO_T);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
}
void MyGLWidget::resizeGL(){
    //float ra = float(width())/float(height());
}
void MyGLWidget::creaBuffers(){
    m.load("../models/HomerProves.obj");
    Calcula();
    glGenVertexArrays(1, &VAO_H);
    glBindVertexArray(VAO_H);
    GLuint VBO_H[2];
    glGenBuffers(2, VBO_H);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_H[0]);
    
    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces ().size () * 3 * 3,m.VBO_vertices (), GL_STATIC_DRAW);
    
    //activem vertexloc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0,0);
    glEnableVertexAttribArray(vertexLoc);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_H[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * m.faces ().size () * 3 * 3,m.VBO_matdiff (), GL_STATIC_DRAW);
    
    //activem colorloc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
    
    glBindVertexArray(0);
}
int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}


