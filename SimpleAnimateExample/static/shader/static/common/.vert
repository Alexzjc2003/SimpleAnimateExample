#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aTan;
layout(location = 4) in vec3 aBit;

out VS_OUT {
  vec3 pos_frag;
  vec3 norm_frag;
  vec2 uv;
} vs_out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
  vs_out.pos_frag = (uModel * vec4(aPos, 1.0)).xyz;
  vs_out.norm_frag = transpose(inverse(mat3(uModel))) * aNorm;
  vs_out.uv = aUV;

  gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}