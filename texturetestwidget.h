#ifndef TEXTURETESTWIDGET_H
#define TEXTURETESTWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <Qtimer>
#include <QImage>



class TextureTestWidget:public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:

    TextureTestWidget(QWidget *parent = 0);
    ~TextureTestWidget();

//    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:

    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width,int height) Q_DECL_OVERRIDE;


    QMatrix4x4              m_matrixrix;
    QSize                   m_winSize;

    QImage                  *m_image;
    QOpenGLTexture          *m_texture;


    QOpenGLBuffer           m_indexBuffer;
    QOpenGLBuffer           m_vertextBuffer;
    QOpenGLBuffer           m_textureBuffer;


    QOpenGLShaderProgram    m_shaderProgram;

    QTimer *m_timer;



};

#endif // TEXTURETESTWIDGET_H
