/**
 * @file   phong.fs
 * @brief  A fragment shader that outputs a phong shaded surface.
*/
#version 330 core

in vec3 normal;
in vec3 frag_pos;
in vec3 light_pos;
in vec3 view_pos;

out vec4 frag_color;

uniform vec3 light_color;
uniform vec3 object_color;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_color;

    // diffuse 
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    // specular
    float specular_strength = 0.5;
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflectDir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflectDir), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;  

    vec3 result = (ambient + diffuse + specular) * object_color;
    frag_color = vec4(result, 1.0);
}