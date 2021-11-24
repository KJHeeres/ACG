#include "curverenderer.h"

CurveRenderer::~CurveRenderer() {

}

void CurveRenderer::init(QOpenGLFunctions_4_1_Core* f, Settings* s) {
    gl = f;
    settings = s;

    initShaders();
    initBuffers();
}

void CurveRenderer::initShaders() {
    //we use the qt wrapper functions for shader objects
    shaderProg = new QOpenGLShaderProgram();

    //add your geometry shader here
    //shaderProg->addShaderFromSourceCode(QOpenGLShader::Geometry, ":/???");

    shaderProg->link();
}

void CurveRenderer::initBuffers() {


}

void CurveRenderer::updateBuffers(SubdivisionCurve& sc) {

}


void CurveRenderer::draw(SubdivisionCurve& sc) {


    shaderProg->bind();

    gl->glBindVertexArray(vao);

    //draw something

    gl->glBindVertexArray(0);


    shaderProg->release();
}
