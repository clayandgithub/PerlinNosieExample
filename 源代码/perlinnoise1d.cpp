#include "perlinnoise1d.h"
#include <QDebug>
#include <cmath>

PerlinNoise1D::PerlinNoise1D(int base,int octavesNum,double lacunarity,
                             double persistence,int seed)
{
    mInterpolateChoice = LINER_INTERPOLATE;
    mBase = base;//����������������(�����������Ӧ������)//����Խ��,����Ƶ��Խ��
    mDeltaNum = MAX1D_ALL_NUM / mBase;//ÿ��������������֮��ļ�����ֵĶ���
    mOctavesNum = octavesNum;//����������,��Ҫ����,�����������ɱ�����
    mLacunarity = lacunarity;//������Ƶ�ʵ����ٶȵĶ���
    mPersistence = persistence;//����������ݼ��ٶȵĶ���
    mSeed = seed;//���ɲ�ͬα�����������
    mRes_of_1D.reserve(MAX1D_ALL_NUM);
    mRes_of_1D.clear();
    mMaxNoise = 0;
    //finish initialization
}

//���Բ�ֵ��������
double PerlinNoise1D::Liner_Interpolate(double a, double b, double u)
{
    return a*(1-u) + b*u;
}

//3�β�ֵ����
double PerlinNoise1D::third_Interpolate(double a, double b, double u)
{
    double f = (3-2*u)*u*u;
    return (1-f)*a+b*f;
}
//5�β�ֵ����
double PerlinNoise1D::fifth_Interpolate(double a, double b, double u)
{
    double f = u * u * u * (u * (u * 6 - 15) + 10);
    return (1-f)*a+b*f;
}

//����-1��1���������
double PerlinNoise1D::Noise_1D(long i)
{
    i = i * mSeed;
    i = (i << 13) ^ i;
    return ( 1.0 - ( (i * (i * i * 15731 + 789221) + 1376312589) & 0x7fffffff)/1073741824.0);
}

//ƽ����������
double PerlinNoise1D::SmoothedNoise_1D(long x)
{
    return Noise_1D(x)/2  +  Noise_1D(x-1)/4  +  Noise_1D(x+1)/4;
}

//1D�����ֵ���ֵ�ĺ���
double PerlinNoise1D::InterpolatedNoise_1D(double x)
{
    long integer_X    = int(x);//#floor
    double fractional_X = x - integer_X;

    double v1 = SmoothedNoise_1D(integer_X);
    double v2 = SmoothedNoise_1D(integer_X + 1);

    //��ֵ����ѡ��
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
        //Ĭ��ʹ����β�ֵ����
        return fifth_Interpolate(v1 , v2 , fractional_X);
    }
}

//�õ����������ĺ���
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
//����������������ĺ���
void PerlinNoise1D::GenPerlinNoise_1D(int base,int octavesNum,double lacunarity,
                       double persistence,int seed)
{
    mBase = base;//����������������(�����������Ӧ������)//����Խ��,����Ƶ��Խ��
    mDeltaNum = MAX1D_ALL_NUM / mBase;//ÿ��������������֮��ļ�����ֵĶ���
    mOctavesNum = octavesNum;//����������,��Ҫ����,�����������ɱ�����
    mLacunarity = lacunarity;//������Ƶ�ʵ����ٶȵĶ���
    mPersistence = persistence;//����������ݼ��ٶȵĶ���
    mSeed = seed;//���ɲ�ͬα�����������
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
