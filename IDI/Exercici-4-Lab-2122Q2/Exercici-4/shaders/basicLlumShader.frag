#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform vec3 PosFocusCamara;
uniform vec3 PosFocusEscena;
uniform vec3 PosFocusPatricio;

uniform vec3 ColorFocusCamara;
uniform vec3 ColorFocusEscena;
uniform vec3 ColorFocusPatricio;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
    vec3 n = normalize(fnormal);
    
    
    vec3 PosFocusCamaraSCO = (vec4(PosFocusCamara, 0.0)).xyz;
    vec3 PosFocusEscenaSCO = (View*(vec4(PosFocusEscena, 1.0))).xyz;
    vec3 PosFocusPatricioSCO = (View*(vec4(PosFocusPatricio, 1.0))).xyz;
    
    vec3 LCamara =normalize(PosFocusCamaraSCO-fvertex);
    vec3 LEscena =normalize(PosFocusEscenaSCO-fvertex);
    vec3 LPatricio =normalize(PosFocusPatricioSCO-fvertex);
    
    vec3 difus = Difus(n, LCamara, ColorFocusCamara) + Difus(n, LEscena, ColorFocusEscena) + Difus(n, LPatricio, ColorFocusPatricio);
    
    vec3 especular = Especular(n, LCamara, fvertex, ColorFocusCamara) + Especular(n, LEscena, fvertex, ColorFocusEscena) + Especular(n, LPatricio, fvertex, ColorFocusPatricio);
    
    vec3 fcolor = Ambient() + difus + especular;
    
    FragColor = vec4(fcolor, 1);
}
