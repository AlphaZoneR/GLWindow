#include "Texture2D.hpp"

bool Texture2D::LoadTexture(std::string fileLocation){

    this->fragmentShader =
            "#version 330\
            \nuniform sampler2D Sampler2D;\
            \n in vec2 UVs;\
            \nout vec3 color;\
            \nvoid main(){\
                \ncolor = texture(Sampler2D, UVs).rgb;\
            \n}";

    this->vertexShader =
            "#version 330\
            \n in vec3 in_verts; \
            \n in vec2 uvs;\
            \n out vec2 UVs;\
            \n uniform float Scale;\
            \n void main(){ \
                \n gl_Position = vec4(in_verts.xy * Scale, 1.0, 1.0); \
                \n UVs = in_verts.xy * Scale;\
            \n } ";

    this->programID = Utils::LoadShader(this->vertexShader, this->fragmentShader);




    this->fif = FreeImage_GetFileType(fileLocation.c_str(), 0);

    if(fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(fileLocation.c_str());

    if(fif == FIF_UNKNOWN)
        return false;

    if(FreeImage_FIFSupportsReading(fif))
        this->image = FreeImage_Load(fif, fileLocation.c_str());

    if(!this->image)
        return 0;

    this->bits = FreeImage_GetBits(this->image);
    FreeImage_FlipVertical(this->image);

    this->height = FreeImage_GetHeight(this->image);
    this->width = FreeImage_GetWidth(this->image);

    if((this->bits == 0) || (this->width == 0) || (this->height == 0)){
        return false;
    }

    glGenTextures(1, &this->textureID);

    glBindTexture(GL_TEXTURE_2D, this->textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->height, this->width, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->bits);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    float g_vertex_buffer_data[] = {
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->VertexArrayID);
    glBindVertexArray(this->VertexArrayID);

    for(int i = 0; i < 18; i++){
        this->data.push_back(g_vertex_buffer_data[i]);
    }

    glGenBuffers(1, &this->vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->data.size() * sizeof(float), &this->data.front(), GL_STATIC_DRAW);


    FreeImage_Unload(this->image);

    return 1;


}

void Texture2D::Render(){
    Utils::updateUniformFloat(this->scale, this->programID, "Scale");
    glUseProgram(this->programID);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    glDrawArrays(GL_TRIANGLES, 0, this->data.size()/3);
    glDisableVertexAttribArray(0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_TEXTURE_BUFFER, this->textureID);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       2,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    glDrawArrays(GL_TRIANGLES, 0, sizeof(this->bits));
    glDisableVertexAttribArray(0);





}


void Texture2D::setScale(float sc){
    this->scale = sc;
}
