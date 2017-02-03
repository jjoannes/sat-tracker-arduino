// Tracker header file

// Includes
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define pi 3.14159265358979323846
#define J2 0.00108263 // Duh
#define mu 398600 // Earth Grav Param [km3/s2]
#define R_e 6378.14 // Earth radius [km]
#define lamb_gw_Ep 1.73278033467249
#define We 6.300388098700644


//PROTOTYPES ==============================================================
typedef struct
{
	int EpochYr;
	float EpochDay; //day of year & day fraction
	float Inc; //inclination
	float RAAN; //right ascension of ascending node
	float Ecc; //eccentricity
	float AOP; //argument of periapsis
	float M; //mean anomaly
	float N; //mean motion
	//others can be binned
}TLE;

// File Protos ------------------------
float intitialiseTime();
TLE readTLE(const char *filename);
void ReadSettingsFile(const char *filename, float *FileLat, float *FileLong, int *howmanypasses, int *dtseconds, float *minEl);
void printInputs(float FileLat, float FileLong, int ReqPasses, int dt, float minEl);
FILE *initialiseOutput();
void printTLE(TLE inputTLE);

// Maths Protos ------------------------
float WrapTo2Pi(float Angle);
float WrapToPi(float Angle);
float DegToRad(float Deg);
float RadToDeg(float Rad);
void FracToTime(float DayAndFrac,int *hh,int *mm,int *ss);
float TimeToFrac(int hh,int mm,int ss);

// Orbit Protos ----------------------
float CalcSMA(TLE InputTle);
void TimeCalc(float T_now, TLE InputTLE, float SMA, float *RAAN, float *AOP, float *lamb_gw, float *nu, float *r);
void GetLatLong(TLE InputTLE,float AOP, float RAAN, float nu, float lamb_gw, float *Lat, float *Long);
void GetAzEl(float Lat,float Long,float GSLat,float GSLong, float r, float *Az,float *El, float *R);
