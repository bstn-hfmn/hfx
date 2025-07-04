layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec4 color;
layout(location = 4) in vec2 uv;

#ifdef VERTEX
    void main() {
        gl_Position = vec4(position.xyz, 1.0f);
    }
#endif

#ifdef FRAGMENT
    out vec4 Color;

    void main() {
        Color =  vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
#endif