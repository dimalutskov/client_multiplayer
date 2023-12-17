#pragma once

#include "AppConstants.h"
#include "../wnd_engine/render/opengl/shaders/ShaderHandler.h"

using namespace wnd;

static char *FRAGMENT_SHADER_SPACE = const_cast<char *>(
        "precision mediump float;\n"
        "\n"
        "varying vec2 VertexCoord;\n"
        "varying vec2 TexCoord;\n"
        "\n"
        "uniform int hasTexture;\n"
        "uniform sampler2D ourTexture;\n"
        "uniform vec4 shaderColor;\n"
        "\n"
        "uniform vec2 u_resolution;\n"
        "\n"
        "\n"
        "void main()\n"
        "{\n"
        "    \tvec2 st = gl_FragCoord.xy / u_resolution.xy;\n"
        "\t\tvec2 dist = st - vec2(0.5);\n"
        "\t\tfloat antialliasing = 0.01;\n"
        "\t\tfloat radius = 1.;\n"
        "\t\tfloat circle = 1.0 - smoothstep(radius- (radius * antialliasing),\n"
        "\t\tradius + (radius * antialliasing),\n"
        "\t\tdot(dist, dist) * 4.0);\n"
        "\t\tvec3 color = vec3(circle);\n"
        "\t\tgl_FragColor = vec4(color, 1.0);\n"
        "        //gl_FragColor = shaderColor;\n"
        "}");

//// Shader uniforms ////
class AppShaderHandler : public ShaderHandler {
public:

    virtual void init() override {
        ShaderHandler::init();

        initShader(AppConstants::SHADER_TYPE_SPACE);
    }

    virtual int getShaderType(const RenderObject &obj) override {
        switch (obj.getShapeType()) {
            case AppConstants::SHADER_TYPE_SPACE: return AppConstants::SHADER_TYPE_SPACE;
        }
        return ShaderHandler::getShaderType(obj);
    }

    virtual void applyCustomShaderFields(Shader *shader, const RenderObject &obj) {
        switch (obj.getShapeType()) {
            case AppConstants::SHADER_TYPE_SPACE:
                return;
        }
    }

protected:

    virtual const char *getFragmentShaderSource(int shaderType) override {
        switch (shaderType) {
            case AppConstants::SHADER_TYPE_SPACE: return FRAGMENT_SHADER_SPACE;
        }
        return ShaderHandler::getFragmentShaderSource(shaderType);
    }

};