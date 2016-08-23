#ifndef WIDGET_H
#define WIDGET_H

#include "perlinnoise1d.h"
#include "perlinnoise2d.h"

#include <QWidget>
#include <QMouseEvent>
#include <QtGui>

//GameState Constantant
#define NO_EVENT 0
#define PERLIN_1D 1
#define PERLIN_2D 2
#define PERLIN_3D 3

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    //functions
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    //void translateAxisCorToQt(double x,double y);
    //double translateXCorToQt(double d_x);
    //double translateYCorToQt(double d_y);

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

    //variables
    bool mNeedErase;
    QPixmap mBoard;
    //QPixmap mFire;
    int mState;//当前所处状态,根据不同状态进行绘制
    PerlinNoise1D *mPerlin1d;
    PerlinNoise2D *mPerlin2d;

private:
    Ui::Widget *ui;
    QTimer *mTimer;

private slots:
    void perlin_button_click();
    void timerEvent();
};

#endif // WIDGET_H
