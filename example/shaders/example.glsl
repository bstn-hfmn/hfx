#ifdef VERTEX
    layout(location = 0) in vec3 iPosition;
    layout(location = 1) in vec3 iNormal;
    layout(location = 2) in vec4 iTangent;
    layout(location = 3) in vec4 iColor;
    layout(location = 4) in vec2 iUV;

    out vec2 TexCoord;
    out vec4 VertexColor;

    void main() {
        TexCoord = iUV;
        VertexColor = iColor;

        gl_Position = HFX_MATRIX_MVP * vec4(iPosition.xyz, 1.0f);
    }
#endif

#ifdef FRAGMENT
    in vec2 TexCoord;
    in vec4 VertexColor;

    out vec4 Color;

    void main() {
        Color =  vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
#endif