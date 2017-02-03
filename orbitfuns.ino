// Orbit functions for Main Tracker code
#include "trackerheader.h"

//==================================================================================================
//==================================================================================================
float CalcSMA(TLE InputTle){
	// Specific Vars to SMA calc
	float T_Sat, SMA, diffSMA, dNa, dNda, SMA_next;
	T_Sat = 86400/InputTle.N; // Period of sat in [s]
	SMA = pow(mu*pow(T_Sat/(2*pi),2),1/3.0); // Initial guess
	diffSMA = 1;
	while (diffSMA > 1e-6){
		dNa = pi*1/T_Sat*-2.0-(J2*(R_e*R_e)*1.0/(SMA*SMA)*1.0/pow(-InputTle.Ecc*InputTle.Ecc+1.0,3.0/2.0)*(pow(sin(InputTle.Inc),2.0)*(3.0/2.0)-1.0)*(3.0/2.0)-1.0)*sqrt(1.0/(SMA*SMA*SMA)*mu);
		dNda = 1.0/pow(SMA,5.0/2.0)*sqrt(mu)*(-3.0/2.0)+J2*(R_e*R_e)*1.0/pow(SMA,9.0/2.0)*sqrt(mu)*1.0/pow(-InputTle.Ecc*InputTle.Ecc+1.0,3.0/2.0)*(pow(sin(InputTle.Inc),2.0)*(3.0/2.0)-1.0)*(2.1E1/4.0);
		SMA_next = SMA - dNa/dNda;
    	diffSMA = fabs(SMA_next-SMA);
		SMA = SMA_next;
	}
	return(SMA);
}
//==================================================================================================
//==================================================================================================
void TimeCalc(float T_now, TLE InputTLE, float SMA, float *RAAN, float *AOP, float *lamb_gw, float *nu, float *r){
	float fE, dfE, E_next,diffE = 1;
	float M = InputTLE.M + 2*pi*InputTLE.N*(T_now-InputTLE.EpochDay);
    float E = M;
	while (diffE > 1e-6){
		fE = E-InputTLE.Ecc*sin(E)-M;
		dfE = 1-InputTLE.Ecc*cos(E);
		E_next=E-fE/dfE;
		diffE = fabs(E_next-E);
		E = E_next;
    }
	*nu = WrapTo2Pi(2*atan(sqrt(1+InputTLE.Ecc)/sqrt(1-InputTLE.Ecc)*tan(E/2)));
    *r = SMA*(1-pow(InputTLE.Ecc,2))/(1+InputTLE.Ecc*cos(*nu));
    *lamb_gw = WrapTo2Pi(lamb_gw_Ep + We*T_now);
    *RAAN = WrapTo2Pi(InputTLE.RAAN+J2*InputTLE.N*pi*(R_e*R_e)*1.0/(SMA*SMA)*cos(InputTLE.Inc)*1.0/pow(InputTLE.Ecc*InputTLE.Ecc-1.0,2.0)*(InputTLE.EpochDay-T_now)*3.0);
    *AOP =  WrapTo2Pi(InputTLE.AOP+J2*InputTLE.N*pi*(R_e*R_e)*1.0/(SMA*SMA)*1.0/pow(InputTLE.Ecc*InputTLE.Ecc-1.0,2.0)*(InputTLE.EpochDay-T_now)*(pow(sin(InputTLE.Inc),2.0)*(5.0/2.0)-2.0)*3.0);

}
//==================================================================================================
//==================================================================================================
void GetLatLong(TLE InputTLE,float AOP, float RAAN, float nu, float lamb_gw, float *Lat, float *Long){
	float C,S;
	*Lat = asin(sin(AOP+nu)*sin(InputTLE.Inc));
	while (*Lat > pi/2.0){*Lat -= 2*pi;}
	while (*Lat < -pi/2.0){*Lat += 2*pi;}
	S = tan(*Lat)/tan(InputTLE.Inc);
	C = cos(AOP+nu)/cos(*Lat);
	*Long = WrapToPi(atan2(S,C)+RAAN-lamb_gw);
}
//==================================================================================================
//==================================================================================================
void GetAzEl(float Lat,float Long,float GSLat,float GSLong, float r, float *Az,float *El,float *R){
	float Beta,S_Az,C_Az;
	Beta = acos(sin(Lat)*sin(GSLat)+cos(Lat)*cos(GSLat)*cos(Long-GSLong));
	*R = sqrt(R_e*R_e+r*r-R_e*r*cos(Beta)*2.0);
	*El = WrapToPi(asin((r*r-*R**R-R_e*R_e)/(2*R_e**R)));

	S_Az = sin(Long-GSLong)*sin(pi/2-Lat)/sin(Beta);
	C_Az = (sin(Lat)-sin(GSLat)*cos(Beta))/(sin(pi/2-GSLat)*sin(Beta));
	*Az = WrapTo2Pi(atan2(S_Az,C_Az));
}
//==================================================================================================
//==================================================================================================
