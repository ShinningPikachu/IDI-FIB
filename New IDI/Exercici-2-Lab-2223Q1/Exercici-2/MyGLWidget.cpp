#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <QtMath>
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

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::RickTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(-2.5, 0, 0));
  TG = glm::rotate(TG, direccio, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
  TG = glm::translate(TG, -centreBaseModels[RICK]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posMorty);
  TG = glm::rotate(TG, direccioMorty, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
  TG = glm::translate(TG, -centreBaseModels[MORTY]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyTransformNew(){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posMortyNew);
  TG = glm::rotate(TG, direccioMortyNew, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
  TG = glm::translate(TG, -centreBaseModels[MORTY]);
  glm::vec3 c (1.0, 0.0, 0.0);
  glUniform3fv(MortyColorLoc, 1, &c[0]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::PortalTransform ()
{
  glm::mat4 TG(1.0f);

  TG = glm::translate(TG, glm::vec3(-2.5 + 3*cos(direccioPortal-float(M_PI/2)),0, 0 - 3*sin(direccioPortal-float(M_PI/2))));
  TG = glm::rotate(TG, direccioPortal, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[PORTAL]/4, escalaModels[PORTAL], escalaModels[PORTAL]));
  TG = glm::translate(TG, -centreBaseModels[PORTAL]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::identTransform ()
{
  glm::mat4 TG(1.0f);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  //View = glm::lookAt (obs, vrp, up);
  if(camera){
    View = glm::lookAt(glm::vec3(posMorty[0], 0.8, posMorty[2]),glm::vec3(posMorty[0] + 18 * cos(direccioMorty-float(M_PI/2)), 0.8, posMorty[2] - 18*sin(direccioMorty-float(M_PI/2))),glm::vec3(0,1,0));
  }else{
    View = glm::translate(View, glm::vec3(0.,0.,-2*radiEscena));
    View = glm::rotate(View, -girTheta, glm::vec3(1., 0., 0.));
    View = glm::rotate(View, -girPsi, glm::vec3(0., 1., 0.));
    View = glm::translate(View, -centreEscena);
  }


  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if (camera){
    Proj = glm::perspective(fovNew, raNew, znearNew, zfarNew);
  }else{
    //std::cout << ample << " " << alt << std::endl;
    //std::cout << float(ample/alt) << std::endl;
    float pFov = fov;
    if(ra < 1) pFov = 2.0 * atan(tan(float(fov/2.0))/ra);
    Proj = glm::perspective (pFov, ra, znear, zfar);
  }
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}
void MyGLWidget::iniCamera()
{
  fov = 2.0 * asin(radiEscena / (2*radiEscena));
  znear = radiEscena;
  zfar  = 3*radiEscena;
  ra = 1.0;

  girPsi = 0.0;
  x_ant = girPsi;
  girTheta = -float(M_PI/4.0);
  y_ant = -girTheta;

  projectTransform();
  viewTransform();
}
void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();

  if(not camera){
    if(e->x() > x_ant) girPsi -= 0.03;
    else if(e->x() < x_ant) girPsi += 0.03;

    if(e->y() > y_ant) girTheta -= 0.03;
    else if(e->y() < y_ant) girTheta += 0.03;

    x_ant = e->x();
    y_ant = e->y();
  }
  viewTransform();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: {
    	posMorty = posMorty + glm::vec3(cos(direccioMorty-float(M_PI/2)),0.0,-sin(direccioMorty-float(M_PI/2)))*glm::vec3(0.1);
      viewTransform();
      break;
    }
    case Qt::Key_Down: {
	    posMorty = posMorty - glm::vec3(cos(direccioMorty-float(M_PI/2)),0.0,-sin(direccioMorty-float(M_PI/2)))*glm::vec3(0.1);
      viewTransform();
      break;
    }
    case Qt::Key_Q: {
      direccio += float(M_PI/4);
      break;
    }
    case Qt::Key_E: {
      direccio -= float(M_PI/4);
      break;
    }
    case Qt::Key_P: {
      if(direccioPortal == direccio and tancat) tancat = false;
      else if(direccioPortal == direccio and not tancat) tancat = true;
      else tancat = true;
      direccioPortal = direccio;
      break;
    }
    case Qt::Key_Left: {
      direccioMorty += float(M_PI/4);
      viewTransform();
      break;
    }
    case Qt::Key_Right: {
      direccioMorty -= float(M_PI/4);
      viewTransform();
      break;
    }
    case Qt::Key_C: {
      camera = !camera;
      viewTransform();
      projectTransform();
      break;
    }
    case Qt::Key_R: {
      camera = false;
      girPsi = 0.0;
      x_ant = girPsi;
      girTheta = -float(M_PI/4.0);
      y_ant = -girTheta;
      viewTransform();
      projectTransform();
      existe = false;
      tancat = true;
      posMorty = glm::vec3(1,0,0);
      direccioMorty = 0.0;

      direccio = float(M_PI);


      direccioPortal = float(M_PI);
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Rick

  glBindVertexArray (VAO_models[RICK]);
  RickTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);

  glm::vec3 c (1.0, 1.0, 1.0);
  glUniform3fv(MortyColorLoc, 1, &c[0]);
  if(MortyPortalTest(direccio, posMorty) == true and detect == false and tancat == true){
    tancat = false;
    existe = true;
    posMortyNew = posMorty;
    direccioMortyNew = direccioMorty;
  }
  if (existe){
    // MortyNew
    glBindVertexArray (VAO_models[MORTY]);
    MortyTransformNew();
    glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
  }
  glUniform3fv(MortyColorLoc, 1, &c[0]);
  detect = MortyPortalTest(direccio, posMorty);
  // Morty

  glBindVertexArray (VAO_models[MORTY]);
  MortyTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);



  // Portal
  if(tancat){
    glBindVertexArray (VAO_models[PORTAL]);
    PortalTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[PORTAL].faces().size()*3);
  }
  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}
void MyGLWidget::iniEscena(){
  LL2GLWidget::iniEscena();
}
void MyGLWidget::carregaShaders(){
  LL2GLWidget::carregaShaders();
  MortyColorLoc = glGetUniformLocation(program->programId(), "MortyColor");
}
