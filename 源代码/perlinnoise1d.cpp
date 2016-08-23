#include "perlinnoise1d.h"
#include <QDebug>
#include <cmath>

PerlinNoise1D::PerlinNoise1D(int base,int octavesNum,double lacunarity,
                             double persistence,int seed)
{
    mInterpolateChoice = LINER_INTERPOLATE;
    mBase = base;//噪声整数采样数量(最大的整数点对应的整数)//此数越大,噪声频率越高
    mDeltaNum = MAX1D_ALL_NUM / mBase;//每两个整数点噪声之间的间隔所分的段数
    mOctavesNum = octavesNum;//噪声层数量,不要过大,否则计算量会成倍上涨
    mLacunarity = lacunarity;//噪声层频率递增速度的度量
    mPersistence = persistence;//噪声层振幅递减速度的度量
    mSeed = seed;//生成不同伪随机数的种子
    mRes_of_1D.reserve(MAX1D_ALL_NUM);
    mRes_of_1D.clear();
    mMaxNoise = 0;
    //finish initialization
}

//线性插值方法函数
double PerlinNoise1D::Liner_Interpolate(double a, double b, double u)
{
    return a*(1-u) + b*u;
}

//3次插值函数
double PerlinNoise1D::third_Interpolate(double a, double b, double u)
{
    double f = (3-2*u)*u*u;
    return (1-f)*a+b*f;
}
//5次插值函数
double PerlinNoise1D::fifth_Interpolate(double a, double b, double u)
{
    double f = u * u * u * (u * (u * 6 - 15) + 10);
    return (1-f)*a+b*f;
}

//生成-1到1的随机噪声
double PerlinNoise1D::Noise_1D(long i)
{
    i = i * mSeed;
    i = (i << 13) ^ i;
    return ( 1.0 - ( (i * (i * i * 15731 + 789221) + 1376312589) & 0x7fffffff)/1073741824.0);
}

//平滑噪声函数
double PerlinNoise1D::SmoothedNoise_1D(long x)
{
    return Noise_1D(x)/2  +  Noise_1D(x-1)/4  +  Noise_1D(x+1)/4;
}

//1D计算插值点的值的函数
double PerlinNoise1D::InterpolatedNoise_1D(double x)
{
    long integer_X    = int(x);//#floor
    double fractional_X = x - integer_X;

    double v1 = SmoothedNoise_1D(integer_X);
    double v2 = SmoothedNoise_1D(integer_X + 1);

    //插值函数选择
    if(mInterpolateChoice == LINER_INTERPOLATE)
    {
        return Liner_Interpolate(v1 , v2 , fractional_X);
    }
    else if(mInterpolateChoice == THIRD_INTERPOLATE)
    {
        return third_Interpolate(v1 , v2 , fractional_X);
    }
    else
    {
        //默认使用五次插值函数
        return fifth_Interpolate(v1 , v2 , fractional_X);
    }
}

//得到单个噪声的函数
double PerlinNoise1D::GetPerlinNoise_1D(double x)
{
      double total_res = 0;
      double frequency,amplitude;
      for(int i = 0;i<mOctavesNum;++i)
      {
          frequency = pow(mLacunarity,i);
          amplitude = pow(mPersistence,i);
          total_res = total_res + InterpolatedNoise_1D(x * frequency) * amplitude;
      }
      return total_res;
}
//生成最终噪声结果的函数
void PerlinNoise1D::GenPerlinNoise_1D(int base,int octavesNum,double lacunarity,
                       double persistence,int seed)
{
    mBase = base;//噪声整数采样数量(最大的整数点对应的整数)//此数越大,噪声频率越高
    mDeltaNum = MAX1D_ALL_NUM / mBase;//每两个整数点噪声之间的间隔所分的段数
    mOctavesNum = octavesNum;//噪声层数量,不要过大,否则计算量会成倍上涨
    mLacunarity = lacunarity;//噪声层频率递增速度的度量
    mPersistence = persistence;//噪声层振幅递减速度的度量
    mSeed = seed;//生成不同伪随机数的种子
    //mRes_of_1D.reserve(MAX1D_ALL_NUM);
    mRes_of_1D.clear();

    double temp_res;

    mMaxNoise = 0;
    for(int i=0;i < mBase; ++i)
    {
        for(int j=0;j < mDeltaNum;++j)
        {
            temp_res = GetPerlinNoise_1D(i+(double)j/(mDeltaNum));
            mRes_of_1D.push_back(temp_res);
            if(fabs(temp_res) > mMaxNoise)
            {
                mMaxNoise = fabs(temp_res);
            }
        }
    }
    temp_res = GetPerlinNoise_1D(mBase);
    mRes_of_1D.push_back(temp_res);
    if(fabs(temp_res) > mMaxNoise)
    {
        mMaxNoise = fabs(temp_res);
    }
    normalizeResult(mMaxNoise);
}
void PerlinNoise1D::normalizeResult(double maxNoise)
{
//    qDebug("maxNoise = %f",maxNoise);
    for(unsigned int i = 0;i < mRes_of_1D.size();++i)
    {
//        qDebug("before res[%d] = %f",i,mRes_of_1D.at(i));
        mRes_of_1D.at(i)/=maxNoise;
//        qDebug("res[%d] = %f",i,mRes_of_1D.at(i));
    }
}

void PerlinNoise1D::setmBase(int base)
{
    mBase = base;
    mDeltaNum = MAX1D_ALL_NUM / mBase;
}

int PerlinNoise1D::getmBase()
{
    return mBase;
}

int PerlinNoise1D::getmDeltaNum()
{
    return mDeltaNum;
}

void PerlinNoise1D::setmOctavesNum(int num)
{
    mOctavesNum = num;
}
void PerlinNoise1D::setmLacunarity(double l)
{
    mLacunarity = l;
}
void PerlinNoise1D::setmPersistence(double p)
{
    mPersistence = p;
}
void PerlinNoise1D::setmSeed(int s)
{
    mSeed = s;
}
void PerlinNoise1D::setmInterpolateChoice(int i)
{
    mInterpolateChoice = i;
}
