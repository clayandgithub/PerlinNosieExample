#ifndef PERLINNOISE2D_H
#define PERLINNOISE2D_H

#include<QImage>
//ͼ�����߶�(����)
#define MAX2D_I_NUM 100
//ͼ�������(����)
#define MAX2D_J_NUM 100
#define LINER_INTERPOLATE 1
#define THIRD_INTERPOLATE 2
#define FIFTH_INTERPOLATE 3

class PerlinNoise2D
{
public:
    //funcitons
    PerlinNoise2D(int baseI = 30,int baseJ = 30,int octavesNum = 2,double lacunarity = 2.0,
                  double persistence = 0.5,int seed = 1);
    ~PerlinNoise2D();
    void GenPerlinNoise_2D(int baseI = 30,int baseJ = 30,int octavesNum = 2,double lacunarity = 2.0,
                             double persistence = 0.5,int seed = 1);
    //get&&set functions
    void setmBase(int baseI,int baseJ);
    void setmOctavesNum(int num);
    void setmLacunarity(double l);
    void setmPersistence(double p);
    void setmSeed(int s);
    void setmInterpolateChoice(int i);
    int getmDeltaINum();
    int getmDeltaJNum();
    int getmBaseI();
    int getmBaseJ();

    //variables
    uchar *mRes_charData;
    QImage mRes_of_2D;
private:
    //funcitons
    int Liner_Interpolate(int a, int b, double u);
    int third_Interpolate(int a, int b, double u);
    int fifth_Interpolate(int a, int b, double u);
    int Noise_2D(long i,long j);
    int SmoothNoise_2D(long fi, long fj);
    int InterpolatedNoise_2D(double fi, double fj);
    int GetPerlinNoise_2D(double fi, double fj);
    void normalizeResult(int maxNoise);

    //variables
    int mBaseI;//(row)����������������(�����������Ӧ������)//����Խ��,����Ƶ��Խ��
    int mBaseJ;//(col)����������������(�����������Ӧ������)//����Խ��,����Ƶ��Խ��
    int mDeltaINum;//(row(direction:y))ÿ��������������֮��ļ�����ֵĶ���
    int mDeltaJNum;//(col(direction:x))ÿ��������������֮��ļ�����ֵĶ���
    int mOctavesNum;//����������
    double mLacunarity;//������Ƶ�ʵ����ٶȵĶ���
    double mPersistence;//����������ݼ��ٶȵĶ���
    int mSeed;//���ɲ�ͬα�����������
    int mInterpolateChoice;//��ֵ������ѡ��
    int mMaxNoise;//used to normalize the result
    static int mPermutation[];
};


#endif // PERLINNOISE2D_H
