#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define GLEW_STATIC
#include <gl/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <webp/decode.h>
#include "vananagui/platform/BinaryResource.h"
#include "vananagui/renderer/Texture.h"
#include "vananagui/Vanme.h"
#include "common.h"



Texture::Texture(const char* resourcename, int frames) {

    BinaryResource* res = BinaryResource::Load(resourcename, "WEBP");
    uint8_t* pixels = WebPDecodeBGRA((uint8_t*)res->data, res->size, &this->width, &this->height);
    #ifdef DEBUG
    if (!pixels) {
        printf("Failed to decode WebP of size %ix%i\n", this->width, this->height);
        return;
    }
    #endif

    this->frames = frames;

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (char*)pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    SAFE_DELETE_PTR(res);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->id);
}



#ifdef FALSE
void Texture::draw(glm::vec2 pos, glm::vec2 origin, glm::vec2 scale, int frame, float rotation) {
    Van::me().shaderTexture->use();

    float u0 = 0.0, u1 = 1.0, v0, v1;
    float th = 1.0 / this->frames;
    v0 = th * frame; v1 = v0 + th;
    float w = this->width * scale.x;
    float h = (this->height * scale.y) / this->frames;



    // glTranslatef(pos.x + origin.x, pos.y + origin.y, 0.0);
    // glRotatef(rotation, 0,0,1);
    // glTranslatef(-origin.x, -origin.y, 0.0);



    float x0 = pos.x;
    float x1 = pos.x + w;
    float y0 = pos.y;
    float y1 = pos.y + h;
    Van::me().shaderTexture->uniformMat4f(
        Van::me().shaderTexture->locateUniform("uMatrix"),
        Van::me().getProjectionMatrix()
        // glm::rotate(Van::me().getProjectionMatrix(), rotation, glm::vec3(0,0,1))
        // MAT4F::Rotate(
        //     Van::me().getProjectionMatrix(),
        //     0.0f,
        //     0.0f,
        //     0.0f,
        //     1.0f
        // )
    );

    Van::me().shaderTexture->sampler(
        Van::me().shaderTexture->locateUniform("uSampler"),
        this->id
    );


    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3D), (void*)offsetof(Vector3D, position));
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3D), (void*)offsetof(Vector3D, textCoordinate));

    glBegin(GL_QUADS);
    glTexCoord2f(u0, v0); glVertex2f(x0, y0);
    glTexCoord2f(u0, v1); glVertex2f(x0, y1);
    glTexCoord2f(u1, v1); glVertex2f(x1, y1);
    glTexCoord2f(u1, v0); glVertex2f(x1, y0);
    glEnd();


}
#endif
