// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;
void MyGLWidget::initializeGL()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffersPatricio();
  creaBuffersTerraIParet();
  
  glm::vec3 PosFocusCamara = glm::vec3(0.0,0.0,0.0);
  glUniform3fv(PosFocusCamaraLoc, 1, &PosFocusCamara[0]);
  
  glm::vec3 PosFocusEscena = glm::vec3(10.0, 3.0, 5.0);
  glUniform3fv(PosFocusEscenaLoc, 1, &PosFocusEscena[0]);
    
  glm::vec3 PosFocusPatricio = glm::vec3(5, 3, 2);
  glUniform3fv(PosFocusPatricioLoc, 1, &PosFocusPatricio[0]);
  
  glUniform3fv(ColorFocusCamaraLoc, 1, &ColorFocusCamara[0]);
  glUniform3fv(ColorFocusEscenaLoc, 1, &ColorFocusEscena[0]);
  glUniform3fv(ColorFocusPatricioLoc, 1, &ColorFocusPatricio[0]);
    
  
  iniEscena();
  iniCamera();
}
void MyGLWidget::modelTransformPatricio ()
{
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,2));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreBasePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
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
void MyGLWidget::paintGL(){
    LL4GLWidget::paintGL();
}
MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_C: {
      LlumFocus = !LlumFocus;
      if(LlumFocus) ColorFocusCamara = glm::vec3(0.0,0.0,0.0);
      else ColorFocusCamara = glm::vec3(0.9, 0.0, 0.9);
      glUniform3fv(ColorFocusCamaraLoc, 1, &ColorFocusCamara[0]);

    break;
	}
  case Qt::Key_E: {
      LlumEscena = !LlumEscena;
      if(LlumEscena) ColorFocusEscena = glm::vec3(0.0,0.0,0.0);
      else ColorFocusEscena = glm::vec3(0.9, 0.9, 0.9);
      glUniform3fv(ColorFocusEscenaLoc, 1, &ColorFocusEscena[0]);

    break;
	}
  case Qt::Key_P: {
      LlumPatricio = !LlumPatricio;
      if(LlumPatricio) ColorFocusPatricio = glm::vec3(0.0,0.0,0.0);
      else ColorFocusPatricio = glm::vec3(0.9, 0.9, 0.2);
      glUniform3fv(ColorFocusPatricioLoc, 1, &ColorFocusPatricio[0]);
    break;
	}
  case Qt::Key_Right: {
      gir += 25;
      TG = glm::translate(glm::mat4(1.f), +centreBasePatr);
      TG = glm::rotate(TG, glm::radians(-gir), glm::vec3(0.0,1.0,0.0));
      TG = glm::translate(TG, -centreBasePatr);
      
      glm::vec3 PosFocusPatricio = TG*glm::vec4(5, 3, 2, 1);
      glUniform3fv(PosFocusPatricioLoc, 1, &PosFocusPatricio[0]);
      
    break;
	}
  case Qt::Key_Left: {
            gir -= 25;
      TG = glm::translate(glm::mat4(1.f), +centreBasePatr);
      TG = glm::rotate(TG, glm::radians(-gir), glm::vec3(0.0,1.0,0.0));
      TG = glm::translate(TG, -centreBasePatr);
      
      glm::vec3 PosFocusPatricio = TG*glm::vec4(5, 3, 2, 1);
      glUniform3fv(PosFocusPatricioLoc, 1, &PosFocusPatricio[0]);
    break;
	}
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}
void MyGLWidget::carregaShaders(){
    LL4GLWidget::carregaShaders();
    
    PosFocusCamaraLoc = glGetUniformLocation(program->programId(),"PosFocusCamara");
    ColorFocusCamaraLoc = glGetUniformLocation(program->programId(),"ColorFocusCamara");
    
    PosFocusEscenaLoc = glGetUniformLocation(program->programId(),"PosFocusEscena");
    ColorFocusEscenaLoc = glGetUniformLocation(program->programId(),"ColorFocusEscena");
    
    PosFocusPatricioLoc = glGetUniformLocation(program->programId(),"PosFocusPatricio");
    ColorFocusPatricioLoc = glGetUniformLocation(program->programId(),"ColorFocusPatricio");
    
}
