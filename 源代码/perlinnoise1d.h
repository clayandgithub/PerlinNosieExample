#ifndef PERLINNOISE1D_H
#define PERLINNOISE1D_H

#include <vector>
//噪声最大分段数(包括整数点和中间的插值点)
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
    int mBase;//噪声整数采样数量(最大的整数点对应的整数)//此数越大,噪声频率越高
    int mDeltaNum;//每两个整数点噪声之间的间隔所分的段数
    int mOctavesNum;//噪声层数量
    double mLacunarity;//噪声层频率递增速度的度量
    double mPersistence;//噪声层振幅递减速度的度量
    int mSeed;//生成不同伪随机数的种子
    int mInterpolateChoice;//插值函数的选择
    double mMaxNoise;//used to normalize the result
};

#endif // PERLINNOISE1D_H
