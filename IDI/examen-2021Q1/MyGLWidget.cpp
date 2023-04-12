// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

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

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  //ExamGLWidget::paintGL();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Pintem el terra
    glBindVertexArray (VAO_Terra);
    modelTransformTerra ();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Pintem el Patricio
    if(estatCubPat){
        glBindVertexArray (VAO_Patr);
        modelTransformPatricio ();
        glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
    }else{
        glBindVertexArray(VAO_Cub);

        modelTransformCub (4.0, 0.0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        modelTransformCub (5.0, (2*M_PI)/3);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        modelTransformCub (6.0, ((2*M_PI)/3)*2);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    

    glUniform3fv(colfocusLoc, 1, &colFocus[0]);
  //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



  glBindVertexArray(0);
}

void MyGLWidget::initializeGL(){
    ExamGLWidget::initializeGL();
    
    glUniform3fv(colfocusLoc, 1, &colFocus[0]);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  //ExamGLWidget::modelTransformCub (1.0, 0.0);
  TG = glm::mat4(1.0f);
  TG = glm::translate(TG, glm::vec3(5*cos(angle), 0.0, 5*sin(angle)));
  TG = glm::scale(TG, glm::vec3(escala,escala,escala));
  TG = glm::rotate(TG, angle, glm::vec3(0,-1,0));
  //TG = glm::translate(TG, -glm::vec3(0,0,0));//falta concretar

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  ExamGLWidget::modelTransformPatricio ();
  TG = glm::mat4(1.0f);
  TG = glm::translate(TG, glm::vec3(5*cos(AnglePatricio), 0.0, 5*sin(AnglePatricio)));
  TG = glm::scale(TG, glm::vec3(escala*2,escala*2,escala*2));
  TG = glm::rotate(TG, AnglePatricio, glm::vec3(0,-1,0));
  TG = glm::rotate(TG, float(M_PI/2), glm::vec3(0,-1,0));
  TG = glm::translate(TG, -centreBasePat);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    glm::mat4 View(1.0f);
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, float(M_PI/2), glm::vec3(1, 0, 0));
    View = glm::rotate(View, float(M_PI/2), glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj(1.0f);
    if (ra > 1)Proj = glm::ortho(-radiEsc*ra,radiEsc*ra,-radiEsc ,radiEsc , zn, zf);
    else Proj = glm::ortho(-radiEsc,radiEsc,-radiEsc/ra ,radiEsc/ra , zn, zf);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      estatCubPat = !estatCubPat;
    break;
	}
  case Qt::Key_1: {
      AnglePatricio = 0;
    break;
	}
  case Qt::Key_2: {
      AnglePatricio = (2*M_PI)/3;
    break;
	}
  case Qt::Key_3: {
      AnglePatricio = ((2*M_PI)/3)*2;
    break;
	}
  case Qt::Key_F: {
      if (colFocus.z == 0) colFocus = glm::vec3(1,1,1);
      else colFocus = glm::vec3(1,1,0);
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      viewTransform();
      projectTransform();
    break;
	}
  case Qt::Key_Right: {
      // ...
    break;
	}
  case Qt::Key_Left: {
      // ...
    break;
	}
  case Qt::Key_R: {
      // ...
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

