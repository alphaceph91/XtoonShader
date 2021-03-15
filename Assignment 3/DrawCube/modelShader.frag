#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

uniform sampler2D texture_toon;
uniform vec3 cameraPos;
uniform vec3 lightPos;

//uniform int shaderType;
//uniform float weightTexture;

void main()
{    
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 shadingColor;

    //Ambient
    float ambientFactor = 0.1;
    vec3 ambient = ambientFactor * lightColor;

    vec3 nnormal = normalize(normal);

    //Diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffFactor * lightColor;

    //Specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = normalize(reflect(nnormal, -lightDir));
    float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * specFactor * lightColor;

	shadingColor = (ambient + diffuse + specular); 

    //Xtoon
    float f1 = max(dot(nnormal, lightDir), 0.005); //diffuse light influence calculated in diffuse component
    float f2 = max(dot(nnormal, viewDir), 0.005); // view angle influence calculated in the specular component

    vec4 xToonColor = texture(texture_toon, vec2(f1, f2));

    vec3 result = shadingColor.rgb * vec3(1.0, 1.0, 1.0);

    FragColor =  xToonColor;
}