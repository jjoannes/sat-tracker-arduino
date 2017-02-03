// File functions file for Tracker Main
// Contains functions pertaining to file or i/o
#include "trackerheader.h"

//==================================================================================================
//==================================================================================================
void printInputs(float FileLat, float FileLong, int ReqPasses, int dt, float minEl){
    printf("Settings file inputs:\n");
    printf("GS latitude \t %.2f \t deg\n",RadToDeg(FileLat));
    printf("GS longitude \t %.2f \t deg\n",RadToDeg(FileLong));
    printf("No of passes \t %d \t [-]\n",ReqPasses);
    printf("dt increment \t %d \t s\n",dt);
    printf("Min Elev \t %.1f \t deg\n",RadToDeg(minEl));
    printf("==================================\n");
}
//==================================================================================================
//==================================================================================================
FILE *initialiseOutput(){
    FILE *outf = fopen("outputfile.csv", "w");
    fprintf(outf,"Pass No,UTC Time,Lat,Long,Az,El,R\n");
    return outf;
}
//==================================================================================================
//==================================================================================================
void ReadSettingsFile(const char *filename, float *FileLat, float *FileLong, int *howmanypasses, int *dtseconds, float *minEl){
    FILE *infile = fopen(filename, "r");
    if (infile != NULL ){
        char line[50];
        int lineno = 0;
        while (fgets(line, sizeof line, infile) != NULL){
            if (lineno == 4){
                sscanf(line, "%f", FileLat);
                *FileLat = DegToRad(*FileLat);
                lineno++;
            }
            else if (lineno == 6){
                sscanf(line, "%f", FileLong);
                *FileLong = DegToRad(*FileLong);
                lineno++;
            }
            else if (lineno == 8){
                sscanf(line, "%d", howmanypasses);
                lineno++;
            }
            else if (lineno == 10){
                sscanf(line, "%d", dtseconds);
                lineno++;
            }
            else if (lineno == 12){
                sscanf(line, "%f", minEl);
                *minEl = DegToRad(*minEl);
                lineno++;
            }
            else{lineno++;}
        }
        fclose(infile);
    }
    else{printf("No settings input file\n");}
}
//==================================================================================================
//==================================================================================================
float intitialiseTime(){
    time_t tt = time(NULL);
    struct tm *res = gmtime(&tt);
    float date = res -> tm_yday+1+(tt % 86400)/86400.;
    return date;
}
//==================================================================================================
//==================================================================================================
TLE readTLE(const char *filename){
    FILE *tlefile;
    tlefile = fopen(filename,"r");
    TLE answerTLE;

    if(tlefile != NULL){
        char data[72];
        fgets(data, 72, tlefile);
        printf("%s",data);
        if(strlen(data) < 60) {
            /* To get string
            int i;
            for(i = strlen(data); isspace(data[i--]););
            data[i] = 0;
            printf("%s", data);
            */
            fgets(data, 72, tlefile);
        }
        printf("%s",data);
        sscanf(data, "%*d %*d%*c %*d%*c %2d%f %*f %*f%*f %*f%*f %*d %*d", &(answerTLE.EpochYr), &(answerTLE.EpochDay));
        fgets(data, 72, tlefile);
        printf("%s==================================\n",data);
        sscanf(data, "%*d %*d %f %f %f %f %f %f %*d %*d", &(answerTLE.Inc), &(answerTLE.RAAN), &(answerTLE.Ecc), &(answerTLE.AOP), &(answerTLE.M), &(answerTLE.N));

        if (answerTLE.EpochYr>90){answerTLE.EpochYr += 1900;} else{answerTLE.EpochYr += 2000;}
        answerTLE.Ecc /= 10000000;
        answerTLE.RAAN = DegToRad(answerTLE.RAAN);
        answerTLE.AOP = DegToRad(answerTLE.AOP);
        answerTLE.M = DegToRad(answerTLE.M);
        answerTLE.Inc = DegToRad(answerTLE.Inc);

        fclose(tlefile);
        return answerTLE;
    }
    else{printf("TLE file doesn't exist\n");exit(1);}
}
//==================================================================================================
//==================================================================================================
void printTLE(TLE inputTLE){
    printf("Parsed TLE data:\n");
    printf("EpYear \t %d\n",inputTLE.EpochYr);
    printf("EpDay \t %03.3f\n",(inputTLE.EpochDay));
    printf("ECC \t %0.5f \t [-]\n",(inputTLE.Ecc));
    printf("INC \t %03.3f \t deg\n",RadToDeg(inputTLE.Inc));
    printf("RAAN \t %03.3f \t deg\n",RadToDeg(inputTLE.RAAN));
    printf("AOP \t %03.3f \t deg\n",RadToDeg(inputTLE.AOP));
    printf("M \t %03.3f \t deg\n",RadToDeg(inputTLE.M));
    printf("N \t %0.4f \t rev/day\n",(inputTLE.N));
    printf("==================================\n");
}
