#ifndef PERLINNOISE1D_H
#define PERLINNOISE1D_H

#include <vector>
//�������ֶ���(������������м�Ĳ�ֵ��)
#define MAX1D_ALL_NUM 100
#define LINER_INTERPOLATE 1
#define THIRD_INTERPOLATE 2
#define FIFTH_INTERPOLATE 3

using std::vector;

class PerlinNoise1D
{
public:
    //functions
    PerlinNoise1D(int base = 20,int octavesNum = 2,double lacunarity = 2.0,
                  double persistence = 0.5,int seed = 1);
    //interpolate functions
    double Liner_Interpolate(double a, double b, double u);
    double third_Interpolate(double a, double b, double u);
    double fifth_Interpolate(double a, double b, double u);

    void GenPerlinNoise_1D(int base = 20,int octavesNum = 2,double lacunarity = 2.0,
                           double persistence = 0.5,int seed = 1);
    //get&&set functions
    void setmBase(int base);
    void setmOctavesNum(int num);
    void setmLacunarity(double l);
    void setmPersistence(double p);
    void setmSeed(int s);
    void setmInterpolateChoice(int i);
    int getmDeltaNum();
    int getmBase();

    //variables
    vector<double> mRes_of_1D;
private:
    //functions
    double Noise_1D(long i);
    double SmoothedNoise_1D(long x);
    double InterpolatedNoise_1D(double x);
    double GetPerlinNoise_1D(double x);
    void normalizeResult(double maxNoise);

    //variables
    int mBase;//����������������(�����������Ӧ������)//����Խ��,����Ƶ��Խ��
    int mDeltaNum;//ÿ��������������֮��ļ�����ֵĶ���
    int mOctavesNum;//����������
    double mLacunarity;//������Ƶ�ʵ����ٶȵĶ���
    double mPersistence;//����������ݼ��ٶȵĶ���
    int mSeed;//���ɲ�ͬα�����������
    int mInterpolateChoice;//��ֵ������ѡ��
    double mMaxNoise;//used to normalize the result
};

#endif // PERLINNOISE1D_H
