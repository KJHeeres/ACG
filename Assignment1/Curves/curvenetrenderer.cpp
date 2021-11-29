#include "curvenetrenderer.h"

CurveNetRenderer::~CurveNetRenderer() {
    gl->glDeleteVertexArrays(1, &vao);
    gl->glDeleteBuffers(1, &vbo);

    delete shaderProg;
}

void CurveNetRenderer::init(QOpenGLFunctions_4_1_Core* f, Settings* s) {
    gl = f;
    settings = s;

    initShaders();
    initBuffers();
}

void CurveNetRenderer::initShaders() {
    //we use the qt wrapper functions for shader objects
    shaderProg = new QOpenGLShaderProgram();

    //you can grab shaders from the resource file
    //you can add additional shaders by right-clicking resources.qrc->add existing files
    shaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    shaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");
    //shaderProg->addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/fragshader.glsl");
    //shaderProg->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/fragshader.glsl");

    shaderProg->link();
}

void CurveNetRenderer::initBuffers() {
    // Pure OpenGL functions used here

    //create vao
    gl->glGenVertexArrays(1, &vao);
    //bind vao
    gl->glBindVertexArray(vao);
    //generate single buffer object
    gl->glGenBuffers(1, &vbo);
    //bind it
    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //enable vertex attribute array with index 0 in the currently bound vao
    gl->glEnableVertexAttribArray(0);
    //tell the currently bound vao what the layout is of the vbo
    gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //unbind
    gl->glBindVertexArray(0);
}

void CurveNetRenderer::updateBuffers(SubdivisionCurve& sc) {
    QVector<QVector2D> netCoords = sc.getNetCoords();

    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Draw the curve line if the option is set to CPU
    if(sc.getSubdivMethod() == "CPU" and settings->showCurvePts){
        QVector<QVector2D> curveCoords = sc.getCurveCoords();
        gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*curveCoords.size() + sizeof(QVector2D)*netCoords.size(), 0, GL_DYNAMIC_DRAW);
        gl->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(QVector2D)*netCoords.size(), netCoords.data());
        gl->glBufferSubData(GL_ARRAY_BUFFER, sizeof(QVector2D)*netCoords.size(), sizeof(QVector2D)*curveCoords.size(), curveCoords.data());
    }else if(sc.getSubdivMethod() == "GPU quadratic" and settings->showCurvePts){
        gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*netCoords.size(), netCoords.data(), GL_DYNAMIC_DRAW);
    }else if(sc.getSubdivMethod() == "GPU cubic" and settings->showCurvePts){
        gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*netCoords.size(), netCoords.data(), GL_DYNAMIC_DRAW);
    }else{
        gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*netCoords.size(), netCoords.data(), GL_DYNAMIC_DRAW);
    }
}


void CurveNetRenderer::draw(SubdivisionCurve& sc) {
    QVector<QVector2D> netCoords = sc.getNetCoords();
    shaderProg->bind();

    gl->glBindVertexArray(vao);

    // Draw control net
    gl->glDrawArrays(GL_LINE_STRIP, 0, netCoords.size());
    gl->glPointSize(8.0);
    gl->glDrawArrays(GL_POINTS, 0, netCoords.size());

    // Draw curve
    if(sc.getSubdivMethod() == "CPU" and settings->showCurvePts){
        QVector<QVector2D> curveCoords = sc.getCurveCoords();
        gl->glDrawArrays(GL_LINE_STRIP, netCoords.size(), curveCoords.size());
    }


    // Highlight selected control point
    if (settings->selectedPt > -1) {
        gl->glPointSize(12.0);
        gl->glDrawArrays(GL_POINTS, settings->selectedPt, 1);
    }

    gl->glBindVertexArray(0);


    shaderProg->release();
}
