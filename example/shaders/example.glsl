#ifdef VERTEX
    V2F vert(ATTRIBUTES attrs) {
        V2F v2f;
        v2f.Normal = normalize(mat3(transpose(hfx_WorldToObject)) * attrs.Normal);
        v2f.WorldPosition = HFX_ObjectToWorldPos(attrs.Vertex);
        v2f.ClipPosition = HFX_ObjectToClipPos(attrs.Vertex);
        v2f.TexCoord0 = attrs.TexCoord0;

        return v2f;
    }
#endif

#ifdef FRAGMENT
    uniform sampler2D Texture;

    vec4 fragment(V2F v2f)
    {
        vec3 light = vec3(-5.0f, 3.0f, -5.0f);
        vec3 toLight = normalize(light - v2f.WorldPosition);

        float ambient = 0.1f;
        float diffuse = max(dot(v2f.Normal, toLight), 0.0f);

        return texture(Texture, v2f.TexCoord0) * (ambient + diffuse);
    }
#endif