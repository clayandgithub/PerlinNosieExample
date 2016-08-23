#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <QPixmap>


#define FPS 40

// ��������20
#define MAX_X_CORDINATE 20
//��λ�����������
#define X_COR_DELTA 20
// ���������1
#define MAX_Y_CORDINATE 1
//��λ�����������
#define Y_COR_DELTA 100


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(500,600);
    mNeedErase = false;
    mState=NO_EVENT;
    mBoard=QPixmap(500,500);
    mBoard.fill(Qt::black);
    //mFire = QPixmap( "D:/fire.jpg" );
    mPerlin1d = NULL;
    mPerlin2d = NULL;
    mTimer = NULL;

    connect(ui->perlin_button,SIGNAL(clicked()),this,SLOT(perlin_button_click()));
}

//draw event
void  Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(&mBoard);
    QPen pen(Qt::red);
    if(mNeedErase)
    {
        mBoard.fill(Qt::black);
        //painter.eraseRect(0,0,500,500);
        mNeedErase = false;
    }
    //???????????????????????????
    QRect temp_rect;
    temp_rect = QRect(0,0,500,500);
    QPixmap pixmapMask;
    //????????????????????????
    switch(mState)
    {
    case NO_EVENT:
        break;
    case PERLIN_1D:
        if(mPerlin1d==NULL)
        {
            break;
        }
        pen.setWidth(1);
        painter.setPen(pen);
        //paintAxis();
        painter.save();
        painter.translate(2, 250);
             //    float deltaX = width / 12.0f;         // x������ÿ�ֵĿ��
            //    float deltaY = (float)height / count; // y������ÿ�ֵĿ��
        // ��������
        painter.drawLine(0, 0, 490, 0);

        for (int i = 0; i <= MAX_X_CORDINATE; ++i)
        {
            QString str_x_num = tr("%1").arg(i);
            //int stringWidth = metrics.width(month);
            painter.drawLine(X_COR_DELTA * i, 0, X_COR_DELTA * i, 8);
            painter.drawText(X_COR_DELTA * i,15,str_x_num);
        }

        // ��up������
        painter.drawLine(0,0,0,-240);
        for (int i = 1; i <= MAX_Y_CORDINATE; ++i)
        {
            QString str_y_num = tr("%1").arg(i);
            painter.drawLine(0,-Y_COR_DELTA * i, 8, -Y_COR_DELTA * i);
            painter.drawText(6,-Y_COR_DELTA * i,str_y_num);
        }
        // ��down������
        painter.drawLine(0,0,0,240);
        for (int i = 1; i <= MAX_Y_CORDINATE; ++i)
        {
            QString str_y_num = tr("-%1").arg(i);
            painter.drawLine(0,Y_COR_DELTA * i, 4, Y_COR_DELTA * i);
            painter.drawText(6,Y_COR_DELTA * i,str_y_num);
        }

        // draw points and lines by axis
        pen.setWidth(2);
        painter.setPen(pen);
        //��ʱ����start
        int temp_size;
        temp_size = mPerlin1d->mRes_of_1D.size();
        int i;
        i = 0;
        int temp_delta;//ÿ����λ�����еķֶ���
        temp_delta = temp_size / MAX_X_CORDINATE;
        //qDebug("temp_size = %d,temp_delat = %d",temp_size,temp_delta);
        //��ʱ����end

        while(i+1<temp_size)
        {
            painter.drawLine(X_COR_DELTA*double(i)/temp_delta,-Y_COR_DELTA *mPerlin1d->mRes_of_1D.at(i),
                             X_COR_DELTA*double(i+1)/temp_delta, -Y_COR_DELTA * mPerlin1d->mRes_of_1D.at(i+1));
            ++i;
        }
        painter.restore();
        break;
    case PERLIN_2D:
        if(mPerlin2d==NULL)
        {
            break;
        }
        //......................
        //500,500 --> size(),
        pixmapMask = QPixmap::fromImage(mPerlin2d->mRes_of_2D.scaled(500,500, Qt::KeepAspectRatio));
        painter.drawPixmap(0,0,pixmapMask);
        //pixmapMask = QPixmap("D:/mask.png");
        //mFire.setAlphaChannel(pixmapMask);
        //mFire = QPixmap( "D:/fire.jpg" );
//        mFire.setMask(QPixmap());
        //mFire.setMask(pixmapMask.mask());

        //mFire.loadFromData(mPerlin2d->mRes_charData,MAX2D_I_NUM*MAX2D_J_NUM);
        //painter.drawPixmap(0,0,mFire);

        break;
    case PERLIN_3D:
        break;
    default:
        break;
    }
    QPainter widget_painter(this);
    widget_painter.drawPixmap(0,0,mBoard);
}

//mouse event
void Widget::mousePressEvent(QMouseEvent *)
{
}

//button event
void Widget::perlin_button_click()
{
    if(ui->perlin_kind_box->currentIndex() == 0)
    {
        //1-D perlin
        if(mPerlin1d==NULL)
        {
            mPerlin1d = new PerlinNoise1D();
        }

        //�����û�����Ĳ���
        int base = ui->spin_baseX->value();
        int octavesNum = ui->spin_oct->value();
        double lacunarity = ui->spin_lac->value();
        double persistence = ui->spin_per->value();
        int seed = ui->spin_seed->value();
        //��ֵ��ļ���
        mPerlin1d->GenPerlinNoise_1D(base,octavesNum,lacunarity,persistence,seed);
        mState = PERLIN_1D;
        mNeedErase = true;
        update();
    }
    else if(ui->perlin_kind_box->currentIndex() == 1)
    {
        //perlin 2d
        if(mPerlin2d==NULL)
        {
            mPerlin2d = new PerlinNoise2D();
        }
        if(mTimer==NULL)
        {
            mTimer = new QTimer(this);
        }
        QObject::connect(mTimer,SIGNAL(timeout()),this,SLOT(timerEvent()));
        mTimer->start(1000/FPS);
    }
    else
    {
        //3-D
    }
}

void Widget::timerEvent()
{
    if(ui->perlin_kind_box->currentIndex() == 1)
    {

        //�����û�����Ĳ���(ע��i��j ��x��y�Ĺ�ϵ)
        int baseI;
        baseI= ui->spin_baseY->value();
        int baseJ;
        baseJ= ui->spin_baseX->value();
        int octavesNum;
        octavesNum= ui->spin_oct->value();
        double lacunarity;
        lacunarity= ui->spin_lac->value();
        double persistence;
        persistence= ui->spin_per->value();
//        srand((unsigned)time( NULL ) );//�õ�ǰʱ��������
        int seed;
        seed = ui->spin_seed->value();
//        seed = rand()%256;
        //�����ֵ���ֵ
        mPerlin2d->GenPerlinNoise_2D(baseI,baseJ,octavesNum,lacunarity,persistence,seed);
        mState = PERLIN_2D;
        mNeedErase = true;
        update();
    }

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

Widget::~Widget()
{
    if(mPerlin1d!=NULL)
    {
        delete mPerlin1d;
        mPerlin1d = NULL;
    }

    if(mPerlin2d!=NULL)
    {
        delete mPerlin2d;
        mPerlin2d = NULL;
    }
    if(mTimer==NULL)
    {
        delete mTimer;
        mTimer = NULL;
    }
    delete ui;
}

//Additions:

//qDebug("��ʼ�����!temp=%f",temp_d);
//ui->gen_btn->setText(tr("��������"));
//ui->spin_a->setVisible(true);
//mA =ui->spin_a->value();

// QString s = QString::number(temp_d,'f',6);
//ui->perlin_label->setText(s);
//  QImage my_img = QImage(100,100,QImage::Format_RGB32);
//qDebug("��ʼ�����!");
//QMessageBox::warning(this,tr("Warning"),tr("���������������100!"),QMessageBox::Yes);
////setPixel�����������ֵ������i��j
