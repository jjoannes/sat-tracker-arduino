//=========================== T R A C K E R ===================================
//=============================================================================
#include "trackerheader.h"

//=========================== M A I N  C O D E ================================
//=============================================================================
int main(){

	float t_today = intitialiseTime(); // Initialise current date/time

	TLE ThisSat = readTLE("tle.txt"); // Read TLE file
	printTLE(ThisSat);

	float GSLat, GSLong, minEl;
	int dtseconds, ReqPasses;
	ReadSettingsFile("settings.txt", &GSLat, &GSLong, &ReqPasses, &dtseconds, &minEl); // Read settings file
	printInputs(GSLat,GSLong,ReqPasses,dtseconds,minEl); // Print out what was read from Settings File

	FILE *outf;
	outf = initialiseOutput(); //Open output file

	// Start Vars --> J2-perturbed SMA
	float SMA = CalcSMA(ThisSat);

	// SMA + other Vars + TIMEPOINT --> nu, RAAN, AOP, lamb_gw FOR GIVEN TIMEPOINT
	float RAAN, AOP, lamb_gw, r, nu;
	float Lat, Long, Az, El, R;

	int hh, mm, ss; //for time display

	float T_now = t_today; // Initialise time before loop
	int PassNo = 0; // Pass number, starting 1
	int isPass = 0;
	int wasPass = 1;

	int lineCounter = 1;
	int iterationCounter = 1;
	while (PassNo <= ReqPasses){
		TimeCalc(T_now, ThisSat, SMA, &RAAN, &AOP, &lamb_gw, &nu, &r);

		GetLatLong(ThisSat, AOP, RAAN, nu, lamb_gw, &Lat, &Long);
		FracToTime(T_now,&hh,&mm,&ss);

		GetAzEl(Lat, Long, GSLat, GSLong, r, &Az, &El, &R);

		if (El >= minEl){
			isPass = 1;
			if (isPass > wasPass){PassNo+=1;}
			fprintf(outf,"%d,%02d:%02d:%02d,%.1f,%.1f,%.1f,%.1f,%.1f\n",PassNo,hh,mm,ss,RadToDeg(Lat),RadToDeg(Long),RadToDeg(Az),RadToDeg(El),R);
			//printf("%d\t%02d:%02d:%02d\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\n",PassNo,hh,mm,ss,RadToDeg(Lat),RadToDeg(Long),RadToDeg(Az),RadToDeg(El),R);
			if (lineCounter == 1 && iterationCounter != 1){printf("Next pass at ~ %02d:%02d:%02d\n",hh,mm,ss);}
			if (iterationCounter == 1){printf("Currently passing overhead as of %02d:%02d:%02d\n",hh,mm,ss);}
			lineCounter++;
		}
		else{
			isPass = 0;
		}

		wasPass = isPass;
		T_now += TimeToFrac(0,0,dtseconds);// Increment time by dt
		iterationCounter ++;
	}

	//END TIME LOOP
	printf("%i lines printed to file\n",lineCounter);
	fclose(outf);
	printf("Done!\n");
	return 0;
}
//=========================== F U N C T I O N S ===============================
//=============================================================================
