#version 330 core

in VS_OUT {
  vec3 pos_frag;
  vec3 norm_frag;
  vec2 uv;
} fs_in;

out vec4 FragColor;

uniform sampler2D uDiffuse;

void main() {
  FragColor = texture2D(uDiffuse, fs_in.uv);
}