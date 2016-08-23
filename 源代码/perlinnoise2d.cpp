#include "perlinnoise2d.h"
#include <cmath>
#include <QColor>
#include <time.h>

#include <QDebug>

PerlinNoise2D::PerlinNoise2D(int baseI,int baseJ,int octavesNum,double lacunarity,
                             double persistence,int seed)
{
    mInterpolateChoice = LINER_INTERPOLATE;
    mBaseI = baseI;
    mBaseJ = baseJ;
    mDeltaJNum = MAX2D_J_NUM / mBaseJ;
    mDeltaINum = MAX2D_I_NUM / mBaseI;
    mOctavesNum = octavesNum;//噪声层数量,不要过大,否则计算量会成倍上涨
    mLacunarity = lacunarity;//噪声层频率递增速度的度量
    mPersistence = persistence;//噪声层振幅递减速度的度量
    mSeed = seed;//生成不同伪随机数的种子
    mMaxNoise = 0;

    mRes_charData = new uchar[MAX2D_I_NUM*MAX2D_J_NUM];
    memset(mRes_charData,0,sizeof(uchar)*MAX2D_I_NUM*MAX2D_J_NUM);
//    mRes_of_2D = NULL;
    //finish initialization
}

PerlinNoise2D::~PerlinNoise2D()
{
    if(mRes_charData!=NULL)
    {
        delete mRes_charData;
        mRes_charData = NULL;
    }
}

int PerlinNoise2D::mPermutation[] = { 151,160,137,91,90,15,
                            131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
                            190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
                            88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
                            77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
                            102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
                            135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
                            5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
                            223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
                            129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
                            251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
                            49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};

//线性插值方法函数
int PerlinNoise2D::Liner_Interpolate(int a, int b, double u)
{
    return int(a*(1-u) + b*u);
}

//3次插值函数
int PerlinNoise2D::third_Interpolate(int a, int b, double u)
{
    double f = (3-2*u)*u*u;
    return int((1-f)*a+b*f);
}
//5次插值函数
int PerlinNoise2D::fifth_Interpolate(int a, int b, double u)
{
    double f = u * u * u * (u * (u * 6 - 15) + 10);
    return int((1-f)*a+b*f);
}

//生成0到255的随机噪声
int PerlinNoise2D::Noise_2D(long i,long j)
{
//    srand((unsigned)time( NULL ));//用当前时间作种子
//    if(rand()%16<6)
//    {
//        j+=mSeed;
//    }
//    else
//    {
//        j-=mSeed;
//        if(j<0)
//        {
//            j = -j;
//        }
//    }
    i+=mSeed;
    return mPermutation[(mPermutation[i % 255]+mPermutation[j % 255]) % 255];
//  {}
//            mPermutation;
//    i = long(i*ui->spin_seed->value());
//    j = long(j*ui->spin_seed->value());
//    long n = i*mSeed + j * 57;

//    n = (n<<13) ^ n;
//    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
//    long n = x + y * 57;
//    n = (n<<13) ^ n;
////    double t =(1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff)/1073741824.0);
////    return floor((t+1)/2*255);
//    //return 144;
//    return floor((((n*(n*n*15731+789221) + 1376312589) & 0x7fffffff) / 1073741824.0)*255/2);
}

//2D平滑函数
int PerlinNoise2D::SmoothNoise_2D(long i, long j)
{
//    if (x==0||y==0||x==MAX2D_NUM-1||y==MAX2D_NUM-1)
//    {
//        return Noise_2D(x,y);
//    }
    double corners= 1.0*(Noise_2D(i-1,j-1)+Noise_2D(i+1,j-1)+Noise_2D(i-1,j+1)+Noise_2D(i+1, j+1))/16;
    double sides  = 1.0*(Noise_2D(i-1, j)+Noise_2D(i+1, j)+Noise_2D(i, j-1)+Noise_2D(i, j+1)) /8;
    double center =  1.0*Noise_2D(i, j) / 4;
    return int(corners + sides + center);
}
//2D计算插值点的值的函数
int PerlinNoise2D::InterpolatedNoise_2D(double fi, double fj)
{
      long integer_I    = int(fi);//#floor
      double fractional_I = fi - integer_I;

      long integer_J    = int(fj);//#floor
      double fractional_J = fj - integer_J;

      int v1 = SmoothNoise_2D(integer_I,     integer_J);
      int v2 = SmoothNoise_2D(integer_I + 1, integer_J);
      int v3 = SmoothNoise_2D(integer_I,     integer_J + 1);
      int v4 = SmoothNoise_2D(integer_I + 1, integer_J + 1);

      //插值函数选择
      if(mInterpolateChoice == LINER_INTERPOLATE)
      {
          int i1 = Liner_Interpolate(v1 , v2 , fractional_I);
          int i2 = Liner_Interpolate(v3 , v4 , fractional_I);
          return Liner_Interpolate(i1 , i2 , fractional_J);
      }
      else if(mInterpolateChoice == THIRD_INTERPOLATE)
      {
          int i1 = third_Interpolate(v1 , v2 , fractional_I);
          int i2 = third_Interpolate(v3 , v4 , fractional_I);
          return third_Interpolate(i1 , i2 , fractional_J);
      }
      else
      {
          //默认
          int i1 = fifth_Interpolate(v1 , v2 , fractional_I);
          int i2 = fifth_Interpolate(v3 , v4 , fractional_I);
          return fifth_Interpolate(i1 , i2 , fractional_J);
      }
}
int PerlinNoise2D::GetPerlinNoise_2D(double fi, double fj)
{
    double total_res = 0;
    double frequency,amplitude;
    for(int k = 0;k<mOctavesNum;++k)
    {
        frequency = pow(mLacunarity,k);
        amplitude = pow(mPersistence,k);
        total_res = total_res + InterpolatedNoise_2D(fi*frequency,fj*frequency)*amplitude;
    }
    return int(total_res/mOctavesNum);
}

void PerlinNoise2D::GenPerlinNoise_2D(int baseI,int baseJ,int octavesNum,double lacunarity,
                                        double persistence,int seed)
{
    mBaseI = baseI;
    mBaseJ = baseJ;
    mDeltaINum = MAX2D_I_NUM / mBaseI;
    mDeltaJNum = MAX2D_J_NUM / mBaseJ;
    mOctavesNum = octavesNum;//噪声层数量,不要过大,否则计算量会成倍上涨
    mLacunarity = lacunarity;//噪声层频率递增速度的度量
    mPersistence = persistence;//噪声层振幅递减速度的度量
//    mSeed = seed;//生成不同伪随机数的种子
    ++mSeed;//生成不同伪随机数的种子

    //memset(mRes_charData,0,sizeof(uchar)*MAX2D_I_NUM*MAX2D_J_NUM);
    int temp_res;
    mMaxNoise = 0;

    for(int i = 0;i < mBaseI;++i)
    {
        for(int j = 0;j < mBaseJ;++j)
        {
            for(int ii = 0;ii< mDeltaINum;++ii)
            {
                for(int jj = 0;jj < mDeltaJNum;++jj)
                {
                    temp_res = GetPerlinNoise_2D(i+(double)ii/mDeltaINum,
                                              j+(double)jj/mDeltaJNum);
                    mRes_charData[(i*mDeltaINum+ii)*MAX2D_I_NUM+(j*mDeltaJNum+jj)] = temp_res;
                    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    //setPixel传入的是坐标值而不是i和j
                    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    //mRes_of_2D.setPixel(j*mDeltaJNum+jj,i*mDeltaINum+ii,temp_res);
                    if(abs(temp_res) > mMaxNoise)
                    {
                        mMaxNoise = abs(temp_res);
                    }
//                        qDebug("(%f,%f) = %d",i+(double)ii/mDeltaINum,j+(double)jj/mDeltaJNum,
//                               int((temp_res+1)/2*255));
                }
            }
        }
    }
    normalizeResult(mMaxNoise);
    mRes_of_2D = QImage(mRes_charData,MAX2D_J_NUM,MAX2D_I_NUM,QImage::Format_Indexed8);

    QVector<QRgb>  colorTable;
    for(int k=0;k<256;++k)
    {
        colorTable.push_back(qRgba(k,abs(k-100),0,k));
//        if(k>100)
//        {
//            colorTable.push_back(qRgba(k,abs(k-100),0,255));
//        }
//        else
//        {
//            colorTable.push_back(qRgba(k,abs(k-100),0,0));
//        }
    }
    mRes_of_2D.setColorTable(colorTable);//颜色表
}

void PerlinNoise2D::normalizeResult(int maxNoise)
{
    for(int i = 0;i < MAX2D_I_NUM;++i)
    {
        for(int j = 0;j < MAX2D_J_NUM;++j)
        {
            mRes_charData[i*MAX2D_I_NUM+j] = int(255*mRes_charData[i*MAX2D_I_NUM+j]*1.0/maxNoise);
        }
    }
}

//get&&set functions
void PerlinNoise2D::setmBase(int baseI,int baseJ)
{
    mBaseI = baseI;
    mBaseJ = baseJ;
    mDeltaJNum = MAX2D_J_NUM / mBaseJ;
    mDeltaINum = MAX2D_I_NUM / mBaseI;
}
int PerlinNoise2D::getmDeltaINum()
{
    return mDeltaINum;
}
int PerlinNoise2D::getmDeltaJNum()
{
    return mDeltaJNum;
}
int PerlinNoise2D::getmBaseI()
{
    return mBaseI;
}
int PerlinNoise2D::getmBaseJ()
{
    return mBaseJ;
}
void PerlinNoise2D::setmOctavesNum(int num)
{
    mOctavesNum = num;
}
void PerlinNoise2D::setmLacunarity(double l)
{
    mLacunarity = l;
}
void PerlinNoise2D::setmPersistence(double p)
{
    mPersistence = p;
}
void PerlinNoise2D::setmSeed(int s)
{
    mSeed = s;
}
void PerlinNoise2D::setmInterpolateChoice(int i)
{
    mInterpolateChoice = i;
}
