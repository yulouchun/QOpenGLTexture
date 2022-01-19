
#include <QApplication>



#include <QWidget>
#include <QGridLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>


#include "texturetestwidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;



    QSize size(880, 720);
    TextureTestWidget *m_textureWidget = new TextureTestWidget();
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_textureWidget);


    w.setLayout(layout);
    w.show();
    return a.exec();
}
