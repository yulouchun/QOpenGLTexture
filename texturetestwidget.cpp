#include "texturetestwidget.h"






/*可视窗口坐标
 *              ^
 *              |
 *   -1,1       |        1,1
 *              |
 *              |
 *   ----------0,0------------->
 *              |
 *              |
 *  -1,-1       |        1,-1
 *              |
 *              |
 *
 */
static float vertexArry[] =
{
  -1.0 , 1.0 , 0.0 ,//左上第0个点
   1.0 , 1.0 , 0.0 ,//右上第1个点
  -1.0 ,-1.0 , 0.0 ,//左下第2个点
   1.0 ,-1.0 , 0.0  //右下第3个点
};






/*纹理坐标
 *  0,0--------1,0----->
 *  |
 *  |
 *  |
 *  |
 *  |0,1       1,1
 *  |
 *  V
 */
static float textureArry[] =
{
  0.0 , 0.0 , 0.0 ,//左上第0个点
  1.0 , 0.0 , 0.0 ,//右上第1个点
  0.0 , 1.0 , 0.0 ,//左下第2个点
  1.0 , 1.0 , 0.0  //右下第3个点
};


//从图像中心挖出一块贴在窗口上
//static float textureArry[] =
//{
//  0.25 , 0.25 , 0.0 ,//左上第0个点
//  0.75 , 0.25 , 0.0 ,//右上第1个点
//  0.25 , 0.75 , 0.0 ,//左下第2个点
//  0.75 , 0.75 , 0.0  //右下第3个点
//};





/******************************************************************************
 * 上述顶点和纹理建立的正方形顶点顺序如下
 * 0 ------1
 * |       |
 * |       |
 * |       |
 * 2-------3
 * ***************************************************************************/
static unsigned short indexArry[] =
{
   8886,9994,7747,//前三个数是没用的，放在这里是为了更好地理解glDrawelements参数的含义
    0 , 1 ,  2,
    2 , 1 ,  3
};





TextureTestWidget::TextureTestWidget(QWidget *parent):
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(NULL)

{

    m_image = new QImage("test.jpg");
    m_winSize = QSize(m_image->width(),m_image->height());
    m_matrixrix.setToIdentity();
}


TextureTestWidget::~TextureTestWidget()
{

}

//QSize TextureTestWidget::minimumSizeHint() const
//{
//    return m_winSize;
//}



QSize TextureTestWidget::sizeHint() const
{

    return m_winSize;
}



void TextureTestWidget::initializeGL()
{

    initializeOpenGLFunctions();


    m_vertextBuffer.create();
    m_vertextBuffer.bind();
    m_vertextBuffer.allocate(vertexArry,12 * sizeof(float)); //12的含义是：vertexArry的长度是12


    m_textureBuffer.create();
    m_textureBuffer.bind();
    m_textureBuffer.allocate(textureArry, 12 * sizeof(float));//12的含义是：textureArry的长度是12

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexArry,9 * sizeof(unsigned short)); //9的含义是：indexArry 的长度是9





    m_texture = new QOpenGLTexture(*m_image);
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::ClampToBorder);



    QString vertexShaderCode =
           "attribute vec4 position;\n\
            attribute vec3 texcoor;\n\
            uniform   mat4 transMatrix;\n\
            varying vec3 vTexcoor;\n\
            void main()\n\
            {\n\
                    vTexcoor = texcoor;\n\
                    gl_Position = position * transMatrix;\n\
            }\n";




    QString FragmentShaderCode =
            "uniform sampler2D m_sampler;\n\
            varying vec3 vTexcoor;\n\
            void main()\n\
            {\n\
                    vec4 m_tex = texture2D(m_sampler, vec2(vTexcoor.x, vTexcoor.y));\n\
                    gl_FragColor = m_tex;\n\
            }\n";





    if(!m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderCode))
    {
        return ;
    }


    if(!m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,FragmentShaderCode))
    {
        return ;
    }


    if(!m_shaderProgram.link())
    {
        return ;
    }


    if(!m_shaderProgram.bind())
    {
        return ;
    }



}


void  TextureTestWidget::paintGL()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("transMatrix",m_matrixrix);



    m_vertextBuffer.bind();
    int vectlocation = m_shaderProgram.attributeLocation("position");
    m_shaderProgram.enableAttributeArray(vectlocation);
    m_shaderProgram.setAttributeBuffer(vectlocation,GL_FLOAT,0,3,3*sizeof(float));// 0,3,3 分别对应: m_vertextBuffer的偏移地址，解析尺寸，步长


    m_textureBuffer.bind();
    int texcoordLocation = m_shaderProgram.attributeLocation("texcoor");
    m_shaderProgram.enableAttributeArray(texcoordLocation);
    m_shaderProgram.setAttributeBuffer(texcoordLocation,GL_FLOAT,0,3,3*sizeof(float));// 0,3,3 分别对应: m_textureBuffer的偏移地址，解析尺寸，步长

    m_shaderProgram.setUniformValue("sampler",0);

    m_indexBuffer.bind();

    m_texture->bind();


    //参数3:表示从indexArry的第3个数 0 开始,代表数据的偏移位置，即offset
    //参数6:表示从indexArry的第3个数 0 开始，一共填往后的6个数，代表一个片元数据的长度，即size
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT,(const void*) (3 * sizeof(unsigned short)));

}


void TextureTestWidget::resizeGL(int w, int h)
{
    glViewport( 0, 0, w, h);
}


