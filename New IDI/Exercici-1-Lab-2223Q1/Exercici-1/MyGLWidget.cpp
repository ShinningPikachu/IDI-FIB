#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersCapICos();
  creaBuffersBrac();
  creaBuffersCama();
  creaBuffersCorona();
}

void MyGLWidget::pintaNino()
{
  modelTransformCapICos();
  pintaCapICos();

  modelTransformCamaEsquerra();
  pintaCama(); 
  
  modelTransformCamaDreta();
  pintaCama(); 
  
  modelTransformBracDret();
  pintaBrac();
  
  modelTransformBracEsquerre();
  pintaBrac();
  
  modelTransformCorona();
  pintaCorona();
  
}

void MyGLWidget::pintaCapICos()
{
  glBindVertexArray(VAOCapICos);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void MyGLWidget::pintaCama()
{
  glBindVertexArray(VAOCama);
  glDrawArrays(GL_TRIANGLES, 0, 12);
}

void MyGLWidget::pintaBrac()
{
  glBindVertexArray(VAOBrac);
  glDrawArrays(GL_TRIANGLES, 0, 12);
}

void MyGLWidget::pintaCorona()
{
  glBindVertexArray(VAOCorona);
  glDrawArrays(GL_TRIANGLES, 0, 15);
}


void MyGLWidget::modelTransformCapICos()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG,glm::vec3(0.4,0.4,1.0));  
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCamaDreta()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,glm::vec3(-0.1,-0.4,0.0));
  TG = glm::scale(TG,glm::vec3(0.2,0.2,1.0));  
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]); 
}

void MyGLWidget::modelTransformCamaEsquerra()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,glm::vec3(0.1,-0.4,0.0));
  TG = glm::scale(TG,glm::vec3(0.2,0.2,1.0));  
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]); 
}

void MyGLWidget::modelTransformBracDret()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(-0.2, -0.15, 0.0));
  TG = glm::scale (TG, glm::vec3(0.15, 0.15, 0.0));
  TG = glm::translate(TG, glm::vec3(0.25, 1.0, 0.0));
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]); 
}

void MyGLWidget::modelTransformBracEsquerre()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(0.125, -0.15, 0.0));
  TG = glm::scale (TG, glm::vec3(0.15, 0.15, 0.0));
  
  TG = glm::translate(TG, glm::vec3(0.25, 1.0, 0.0));
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]); 
}

void MyGLWidget::modelTransformCorona()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(0.0, 0.5, 0.0));
  TG = glm::scale(TG,glm::vec3(0.2, 0.2, 1.0));
  
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]); 
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Pintem el nino
  pintaNino();
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffersCapICos ()
{
  glm::vec3 Vertices[36];  // vèrtexs amb X, Y i Z
  
  // Torso
  Vertices[0] = glm::vec3(-0.5, -0.5, 0);
  Vertices[1] = glm::vec3( 0.5, -0.5, 0);
  Vertices[2] = glm::vec3( 0.5, 0.5, 0);
  Vertices[3] = glm::vec3( 0.5, 0.5, 0);
  Vertices[4] = glm::vec3( -0.5, 0.5, 0);
  Vertices[5] = glm::vec3( -0.5, -0.5, 0);
  
  // Coll  
  Vertices[6] = glm::vec3(-0.1, 0.5, 0);
  Vertices[7] = glm::vec3( 0.1, 0.5, 0);
  Vertices[8] = glm::vec3( 0.1, 0.625, 0);
  Vertices[9] = glm::vec3( 0.1, 0.625, 0);
  Vertices[10] = glm::vec3( -0.1, 0.625, 0);
  Vertices[11] = glm::vec3( -0.1, 0.5, 0);

  // Cabell
  Vertices[12] = glm::vec3(-0.55, 0.75, 0);
  Vertices[13] = glm::vec3( 0.55, 0.75, 0);
  Vertices[14] = glm::vec3( 0.55, 1.25, 0);
  Vertices[15] = glm::vec3( 0.55, 1.25, 0);
  Vertices[16] = glm::vec3( -0.55, 1.25, 0);
  Vertices[17] = glm::vec3( -0.55, 0.75, 0);
  
  // Cap
  Vertices[18] = glm::vec3(-0.5, 0.625, 0);
  Vertices[19] = glm::vec3( 0.5, 0.625, 0);
  Vertices[20] = glm::vec3( 0.5, 1.2, 0);
  Vertices[21] = glm::vec3( 0.5, 1.2, 0);
  Vertices[22] = glm::vec3( -0.5, 1.2, 0);
  Vertices[23] = glm::vec3( -0.5, 0.625, 0);
  
  // Ulls
  Vertices[24] = glm::vec3(-0.3, 0.85, 0);
  Vertices[25] = glm::vec3( -0.2, 0.85, 0);
  Vertices[26] = glm::vec3( -0.2, 1.0, 0);
  Vertices[27] = glm::vec3( -0.2, 1.0, 0);
  Vertices[28] = glm::vec3( -0.3, 1.0, 0);
  Vertices[29] = glm::vec3( -0.3, 0.85, 0);

  Vertices[30] = glm::vec3(0.3, 0.85, 0);
  Vertices[31] = glm::vec3( 0.2, 0.85, 0);
  Vertices[32] = glm::vec3( 0.2, 1.0, 0);
  Vertices[33] = glm::vec3( 0.2, 1.0, 0);
  Vertices[34] = glm::vec3( 0.3, 1.0, 0);
  Vertices[35] = glm::vec3( 0.3, 0.85, 0);
    
  // Creació del Vertex Array Object (VAO) que usarem per pintar el braç
  glGenVertexArrays(1, &VAOCapICos);
  glBindVertexArray(VAOCapICos);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glm::vec3 Colors[36];  // colors amb R, G, B
  Colors[0] = colorCos;
  Colors[1] = colorCos;
  Colors[2] = colorCos;
  Colors[3] = colorCos;
  Colors[4] = colorCos;
  Colors[5] = colorCos;
  
  Colors[6] = colorCarn;
  Colors[7] = colorCarn;
  Colors[8] = colorCarn;
  Colors[9] = colorCarn;
  Colors[10] = colorCarn;
  Colors[11] = colorCarn;
  
  Colors[12] = colorCabell;
  Colors[13] = colorCabell;
  Colors[14] = colorCabell;
  Colors[15] = colorCabell;
  Colors[16] = colorCabell;
  Colors[17] = colorCabell;
  
  Colors[18] = colorCarn;
  Colors[19] = colorCarn;
  Colors[20] = colorCarn;
  Colors[21] = colorCarn;
  Colors[22] = colorCarn;
  Colors[23] = colorCarn;
  
  Colors[24] = colorUlls;
  Colors[25] = colorUlls;
  Colors[26] = colorUlls;
  Colors[27] = colorUlls;
  Colors[28] = colorUlls;
  Colors[29] = colorUlls;
  
  Colors[30] = colorUlls;
  Colors[31] = colorUlls;
  Colors[32] = colorUlls;
  Colors[33] = colorUlls;
  Colors[34] = colorUlls;
  Colors[35] = colorUlls;
  
  // Creació del buffer amb els colors dels vèrtexs
  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersCama ()
{
  glm::vec3 Vertices[12];  // vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.25, -1.0, 0);
  Vertices[1] = glm::vec3( 0.25, -1.0, 0);
  Vertices[2] = glm::vec3( 0.25, -0.5, 0);
  Vertices[3] = glm::vec3( 0.25, -0.5, 0);
  Vertices[4] = glm::vec3( -0.25, -0.5, 0);
  Vertices[5] = glm::vec3( -0.25, -1.0, 0);
  
  Vertices[6] = glm::vec3(-0.25, -0.5, 0);
  Vertices[7] = glm::vec3( 0.25, -0.5, 0);
  Vertices[8] = glm::vec3( 0.25, 1.0, 0);
  Vertices[9] = glm::vec3( 0.25, 1.0, 0);
  Vertices[10] = glm::vec3( -0.25, 1.0, 0);
  Vertices[11] = glm::vec3( -0.25, -0.5, 0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el braç
  glGenVertexArrays(1, &VAOCama);
  glBindVertexArray(VAOCama);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);


  glm::vec3 Colors[12];  // colors amb R, G, B
  Colors[0] = colorSabata;
  Colors[1] = colorSabata;
  Colors[2] = colorSabata;
  Colors[3] = colorSabata;
  Colors[4] = colorSabata;
  Colors[5] = colorSabata;
  
  Colors[6] = colorPantalo;
  Colors[7] = colorPantalo;
  Colors[8] = colorPantalo;
  Colors[9] = colorPantalo;
  Colors[10] = colorPantalo;
  Colors[11] = colorPantalo;
  
  // Creació del buffer amb els colors dels vèrtexs
  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersBrac ()
{
  glm::vec3 Vertices[12];  // vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.25, -1.0, 0);
  Vertices[1] = glm::vec3( 0.25, -1.0, 0);
  Vertices[2] = glm::vec3( 0.25, -0.5, 0);
  Vertices[3] = glm::vec3( 0.25, -0.5, 0);
  Vertices[4] = glm::vec3( -0.25, -0.5, 0);
  Vertices[5] = glm::vec3( -0.25, -1.0, 0);
  
  Vertices[6] = glm::vec3(-0.25, -0.5, 0);
  Vertices[7] = glm::vec3( 0.25, -0.5, 0);
  Vertices[8] = glm::vec3( 0.25, 1.0, 0);
  Vertices[9] = glm::vec3( 0.25, 1.0, 0);
  Vertices[10] = glm::vec3( -0.25, 1.0, 0);
  Vertices[11] = glm::vec3( -0.25, -0.5, 0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el braç
  glGenVertexArrays(1, &VAOBrac);
  glBindVertexArray(VAOBrac);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);


  glm::vec3 Colors[12];  // colors amb R, G, B
  Colors[0] = colorCarn;
  Colors[1] = colorCarn;
  Colors[2] = colorCarn;
  Colors[3] = colorCarn;
  Colors[4] = colorCarn;
  Colors[5] = colorCarn;
  
  Colors[6] = colorManiga;
  Colors[7] = colorManiga;
  Colors[8] = colorManiga;
  Colors[9] = colorManiga;
  Colors[10] = colorManiga;
  Colors[11] = colorManiga;
  
  // Creació del buffer amb els colors dels vèrtexs
  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersCorona ()
{
  glm::vec3 Vertices[15];  // vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.75, -1.0, 0);
  Vertices[1] = glm::vec3( 0.75, -1.0, 0);
  Vertices[2] = glm::vec3( 0.75, -0.5, 0);
  Vertices[3] = glm::vec3( 0.75, -0.5, 0);
  Vertices[4] = glm::vec3( -0.75, -0.5, 0);
  Vertices[5] = glm::vec3( -0.75, -1.0, 0);
  
  Vertices[6] = glm::vec3(-0.75, -0.5, 0);
  Vertices[7] = glm::vec3( -0.25, -0.5, 0);
  Vertices[8] = glm::vec3( -0.5, 1.0, 0);
  
  Vertices[9] = glm::vec3( -0.25, -0.5, 0);
  Vertices[10] = glm::vec3( 0.25, -0.5, 0);
  Vertices[11] = glm::vec3( 0.0, 1.0, 0);

  Vertices[12] = glm::vec3( 0.25, -0.5, 0);
  Vertices[13] = glm::vec3( 0.75, -0.5, 0);
  Vertices[14] = glm::vec3( 0.5, 1.0, 0);
  
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el braç
  glGenVertexArrays(1, &VAOCorona);
  glBindVertexArray(VAOCorona);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);


  glm::vec3 Colors[15];  // colors amb R, G, B
  Colors[0] = colorCorona;
  Colors[1] = colorCorona;
  Colors[2] = colorCorona;
  Colors[3] = colorCorona;
  Colors[4] = colorCorona;
  Colors[5] = colorCorona;
  
  Colors[6] = colorCorona;
  Colors[7] = colorCorona;
  Colors[8] = colorCorona;
  
  Colors[9] = colorCorona;
  Colors[10] = colorCorona;
  Colors[11] = colorCorona;  
  
  Colors[12] = colorCorona;
  Colors[13] = colorCorona;
  Colors[14] = colorCorona;
  
  // Creació del buffer amb els colors dels vèrtexs
  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  
    // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");

  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG");
}
