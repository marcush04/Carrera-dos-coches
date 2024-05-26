#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec4  fvertex;
in vec3  fnormal;

uniform vec4 poscoche1;
uniform vec4 poscoche2;
uniform bool boolean;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);
const vec3 posfocus = vec3(0,0,0);
const vec3 colorFocus = vec3(0.8,0.8,0.8);
const vec3 colorFocusCoche = vec3(0.6,0.6,0);
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
    vec3 normalSCOFS = normalize(fnormal);
    vec3 LSCO = normalize(posfocus.xyz-fvertex.xyz);
    vec3 LSCOcoche1 = normalize(poscoche1.xyz-fvertex.xyz);
    vec3 LSCOcoche2 = normalize(poscoche2.xyz-fvertex.xyz);
    vec3 fcolor;
    if(boolean) fcolor = Ambient() + Difus(normalSCOFS, LSCO, colorFocus) + Especular(normalSCOFS, LSCO, fvertex.xyz, colorFocus) + Difus(normalSCOFS, LSCOcoche1, colorFocusCoche) + Especular(normalSCOFS, LSCOcoche1, fvertex.xyz, colorFocusCoche) + Difus(normalSCOFS, LSCOcoche2, colorFocusCoche) + Especular(normalSCOFS, LSCOcoche2, fvertex.xyz, colorFocusCoche);
    else {
      fcolor = Ambient() + Difus(normalSCOFS, LSCOcoche1, colorFocusCoche) + Especular(normalSCOFS, LSCOcoche1, fvertex.xyz, colorFocusCoche) + Difus(normalSCOFS, LSCOcoche2, colorFocusCoche) + Especular(normalSCOFS, LSCOcoche2, fvertex.xyz, colorFocusCoche);
    }
    FragColor = vec4(fcolor, 1);
}
