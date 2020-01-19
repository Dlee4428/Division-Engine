#ifndef NOISE_H
#define NOISE_H

#include "../../DivisionPCH.h"

class Noise {

private:
	int Noise3DTexSize = 128;
	GLubyte* Noise3DTexPtr;

#define MAXB 0x100
#define N 0x1000
#define NP 12   // 2^N
#define NM 0xfff

	static int p[MAXB + MAXB + 2];
	static double g3[MAXB + MAXB + 2][3];
	static double g2[MAXB + MAXB + 2][2];
	static double g1[MAXB + MAXB + 2];

	int start;
	int B;
	int BM;

public:
	Noise();
	~Noise();
	GLuint CreateNoise3D();
	void SetNoiseFrequency(int frequency);
	double noise1(double arg);
	double noise2(double vec[2]);
	double noise3(double vec[3]);
	void normalize2(double v[2]);
	void normalize3(double v[3]);
	void initNoise();
	double PerlinNoise1D(double x, double alpha, double beta, int n);
	double PerlinNoise2D(double x, double y, double alpha, double beta, int n);
	double PerlinNoise3D(double x, double y, double z, double alpha, double beta, int n);
	void make3DNoiseTexture();
	GLuint init3DNoiseTexture(int texSize, GLubyte* texPtr);
};

#endif 