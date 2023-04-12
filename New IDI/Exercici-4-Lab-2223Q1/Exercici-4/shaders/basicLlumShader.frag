#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform vec3 colFocusCanto;
uniform vec3 posFocusCanto1;
uniform vec3 posFocusCanto2;
uniform vec3 posFocusCanto3;
uniform vec3 posFocusCanto4;

uniform vec3 colFocusLlanterna;
uniform vec3 posFocusLlanterna;

uniform mat4 TGpos;

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

    vec3 NM = normalize(fnormal);

    vec3 posCant1 = normalize((View*vec4(posFocusCanto1, 1.0)).xyz - fvertex.xyz);
    vec3 posCant2 = normalize((View*vec4(posFocusCanto2, 1.0)).xyz - fvertex.xyz);
    vec3 posCant3 = normalize((View*vec4(posFocusCanto3, 1.0)).xyz - fvertex.xyz);
    vec3 posCant4 = normalize((View*vec4(posFocusCanto4, 1.0)).xyz - fvertex.xyz);

    vec3 posLlan = normalize((View*TGpos*vec4(posFocusLlanterna, 1.0)).xyz - fvertex.xyz);

    vec3 fcolor = Ambient() + Difus(NM, posCant1, colFocusCanto) + Especular(NM, posCant1, fvertex, colFocusCanto) +
                              Difus(NM, posCant2, colFocusCanto) + Especular(NM, posCant2, fvertex, colFocusCanto) +
                              Difus(NM, posCant3, colFocusCanto) + Especular(NM, posCant3, fvertex, colFocusCanto) +
                              Difus(NM, posCant4, colFocusCanto) + Especular(NM, posCant4, fvertex, colFocusCanto) +
                              Difus(NM, posLlan, colFocusLlanterna) + Especular(NM, posLlan, fvertex, colFocusLlanterna);


    FragColor = vec4(fcolor, 1);
}
