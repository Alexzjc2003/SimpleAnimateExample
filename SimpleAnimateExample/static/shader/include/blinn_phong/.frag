#version 330 core
#extension GL_ARB_shading_language_include : require

// max 8 point lights
#define MAX_POINT_LIGHTS 8
uniform int PointLightCount;
#include "/PointLight"

// max 4 directional lights
#define MAX_DIRECTIONAL_LIGHTS 4
uniform int DirectionalLightCount;
#include "/DirectionalLight"

// max 2 spot lights
#define MAX_SPOT_LIGHTS 2
uniform int SpotLightCount;
#include "/SpotLight"

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
// uniform sampler2D normalMap;
uniform float shininess;

in BlinnPhong {
  vec3 pos_frag;
  vec3 norm_frag;
  vec2 uv;
} fs_in;

vec3 CalcAmbient(vec3 color) {
  return color;
}

vec3 CalcDiffuse(vec3 color, vec3 normal, vec3 lightDir) {
  return color * max(dot(normal, lightDir), 0.0);
}

vec3 CalcSpecular(vec3 color, vec3 normal, vec3 lightDir, vec3 viewDir, float shininess) {
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
  return color * spec;
}

float CalcAttenuation(float dist, vec3 atten) {
  return 1.0 / (atten.x + atten.y * dist + atten.z * dist * dist);
}

vec3 CalcPointLight(PointLight_t light, vec2 uv, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fs_in.pos_frag);
  vec3 color = 
    light.ads[0] * CalcAmbient(texture2D(diffuseMap, uv).rgb) +
    light.ads[1] * CalcDiffuse(texture2D(diffuseMap, uv).rgb, normal, lightDir) +
    light.ads[2] * CalcSpecular(texture2D(specularMap, uv).rgb, normal, lightDir, viewDir, shininess);
  return color * CalcAttenuation(distance(fs_in.pos_frag, light.position), light.atten);
}

vec3 CalcDirectionalLight(DirectionalLight_t light, vec2 uv, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);
  vec3 color = 
    light.ads[0] * CalcAmbient(texture2D(diffuseMap, uv).rgb) +
    light.ads[1] * CalcDiffuse(texture2D(diffuseMap, uv).rgb, normal, lightDir) +
    light.ads[2] * CalcSpecular(texture2D(specularMap, uv).rgb, normal, lightDir, viewDir, shininess);
  return color;
}

vec3 CalcSpotLight(SpotLight_t light, vec2 uv, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fs_in.pos_frag);
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutoff.y - light.cutoff.x;
  float intensity = clamp((theta - light.cutoff.x) / epsilon, 0.0, 1.0);
  if(intensity > 0.0) {
    vec3 color = 
      light.ads[0] * CalcAmbient(texture2D(diffuseMap, uv).rgb) +
      light.ads[1] * CalcDiffuse(texture2D(diffuseMap, uv).rgb, normal, lightDir) +
      light.ads[2] * CalcSpecular(texture2D(specularMap, uv).rgb, normal, lightDir, viewDir, shininess);
    return color * intensity * CalcAttenuation(distance(fs_in.pos_frag, light.position), light.atten);
  } else {
    return light.ads[0] * CalcAmbient(texture2D(diffuseMap, uv).rgb);
  }
}

vec3 CalcLight(vec2 uv, vec3 normal, vec3 viewDir) {
  vec3 result = vec3(0.0);

  // for(int i = 0; i < min(PointLightCount, MAX_POINT_LIGHTS); i++) {
  //   result += CalcPointLight(pointLights[i].light, uv, normal, viewDir);
  // }

  // for(int i = 0; i < min(DirectionalLightCount, MAX_DIRECTIONAL_LIGHTS); i++) {
  //   result += CalcDirectionalLight(directionalLights[i].light, uv, normal, viewDir);
  // }

  // for(int i = 0; i < min(SpotLightCount, MAX_SPOT_LIGHTS); i++) {
  //   result += CalcSpotLight(spotLights[i].light, uv, normal, viewDir);
  // }

  switch(PointLightCount) {
    case 8:
      result += CalcPointLight(pointLights[7].light, uv, normal, viewDir);
    case 7:
      result += CalcPointLight(pointLights[6].light, uv, normal, viewDir);
    case 6:
      result += CalcPointLight(pointLights[5].light, uv, normal, viewDir);
    case 5:
      result += CalcPointLight(pointLights[4].light, uv, normal, viewDir);
    case 4:
      result += CalcPointLight(pointLights[3].light, uv, normal, viewDir);
    case 3:
      result += CalcPointLight(pointLights[2].light, uv, normal, viewDir);
    case 2:
      result += CalcPointLight(pointLights[1].light, uv, normal, viewDir);
    case 1:
      result += CalcPointLight(pointLights[0].light, uv, normal, viewDir);
    case 0:
      break;
  }

  switch(DirectionalLightCount) {
    case 4:
      result += CalcDirectionalLight(directionalLights[3].light, uv, normal, viewDir);
    case 3:
      result += CalcDirectionalLight(directionalLights[2].light, uv, normal, viewDir);
    case 2:
      result += CalcDirectionalLight(directionalLights[1].light, uv, normal, viewDir);
    case 1:
      result += CalcDirectionalLight(directionalLights[0].light, uv, normal, viewDir);
    case 0:
      break;
  }

  switch(SpotLightCount) {
    case 2:
      result += CalcSpotLight(spotLights[1].light, uv, normal, viewDir);
    case 1:
      result += CalcSpotLight(spotLights[0].light, uv, normal, viewDir);
    case 0:
      break;
  }

  return result;
}

out vec4 FragColor;

uniform vec3 uCamPos;

void main() {
  FragColor = vec4(CalcLight(fs_in.uv, normalize(fs_in.norm_frag), normalize(uCamPos - fs_in.pos_frag)), 1.0);
}