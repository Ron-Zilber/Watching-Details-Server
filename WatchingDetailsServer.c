/** Project 2 at System Programming course
 * Author: Ron Zilber
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAB 9
#define SPACE 32
#define letterY 89
#define true 1

/// Declarations:

typedef struct serie {
    char serieName[20];
    int numSeasons;
    int rank;
    int* watchingDetails;
    int code;
    char isActive;
}serie;

int showMenu();
int power(int ,int );
int unifyDatabase();
serie ** getUnifyDatabaseDetails();

void getWatchingDetails(serie**);
void printWatchingDetails();
void printSeriesByCode(serie**);
void printSeriesByRank(serie**);
void makeSeriesFileSortedByRank(serie**);
void FSM(int);
int getState();
int binarySearch(int, serie **);
int stringToNumConvert(char[]);
int updateRank(serie **);
int unified = 0, seriesAmount = 0;
serie ** pointersArr;

int main() {
    while(true) {
        int state;
        showMenu();
        fflush(stdin);fflush(stdout);
        state = getState(); fflush(stdin);fflush(stdout);
        FSM(state);
    }
}
// Function Definitions
int showMenu() {
    fflush(stdin); fflush(stdout);
    printf("**********Main Menu **************\n"
           "1. Unify Database.\n"
           "2. Get Unify Database Details.\n"
           "3. Print The Series List Sorted By Code.\n"
           "4. Get Watching Details.\n"
           "5. Show All Watching Details.\n"
           "6. Print Series Sorted By Rank.\n"
           "7. Make Series File Sorted By Rank.\n"
           "8. Exit.\n"
           "Enter Your Selection:");
            fflush(stdin);fflush(stdout);
            return 0;
}
void FSM(int a) {
    switch (a) {
        case 1:{
            seriesAmount = unifyDatabase();
            break;
        }
        case 2:{
            pointersArr = getUnifyDatabaseDetails();
            break;
            }
        case 3:{
            printSeriesByCode(pointersArr);
            break;
        }
        case 4:{
            getWatchingDetails(pointersArr);
            break;
        }
        case 5:{
            printWatchingDetails();
            break;
        }
        case 6:{
            printSeriesByRank(pointersArr);
            break;
        }
        case 7:{
            makeSeriesFileSortedByRank(pointersArr);
            break;
        }
        case 8:{
            if(unified==2){
                int i;
                for(i=0;i<seriesAmount;i++){
                   free(pointersArr[i]->watchingDetails);
                   //free(pointersArr[i]);
                }
                free(pointersArr);
            }
            printf("End Of The Program...");
            exit(0);
        }
        default :
            printf("Wrong Option, Try again !!!\n");
            break;
    }
}
int unifyDatabase() {
    if (unified != 0) {
        printf("Unify Database Has Been Done Already !!!\n");
    } else {
        FILE *output_fp = fopen("series.txt", "w");
        char temp1[20] = {'\0'}, temp2[20] = {'\0'};
        int i, seriesCount = 0; char c = 0;
        FILE *fp1 = NULL, *fp2 = NULL;
        while (fp1 == NULL) {
            fflush(stdin); fflush(stdout);
            printf("Enter First File Name:\n");
            fgets(temp1, 20, stdin);
            for (i = 0; i <= strlen(temp1); i++) {
                if (temp1[i] == '\n') temp1[i] = '\0';
            }
            fp1 = fopen(temp1, "r");
            if (fp1 == NULL) printf("error in opening file %s !!!\n", temp1);
        }
        while (fp2 == NULL) {
            fflush(stdin); fflush(stdout);
            printf("Enter Second File Name:\n");
            fgets(temp2, 20, stdin);
            for (i = 0; i <= strlen(temp2); i++) {
                if (temp2[i] == '\n') temp2[i] = '\0';
            }
            fp2 = fopen(temp2, "r");
            if (fp2 == NULL) printf("error in opening file %s !!!\n", temp2);
        }

        char * line1 = malloc(100); char * line2 = malloc(100);
        serie * serie1 = malloc(sizeof(serie)); serie * serie2 = malloc(sizeof(serie));
        char * scan1 =  fgets(line1, 100, fp1); char * scan2 =  fgets(line2, 100, fp2);
        while ((scan1 != NULL) && (scan2 != NULL)) {
            sscanf(line1, "%19[^,] , %d , %d  , %1s  ", serie1->serieName, &serie1->code, &serie1->numSeasons, &serie1->isActive);
            fflush(stdin); fflush(stdout);
            sscanf(line2, "%19[^,] , %d , %d  , %1s  ", serie2->serieName, &serie2->code, &serie2->numSeasons, &serie2->isActive);
            if (serie1->code < serie2->code) {
                if (serie1->isActive == letterY) {
                    fprintf(output_fp, "%s,%d,%d\n", serie1->serieName, serie1->code, serie1->numSeasons);
                    scan1 = (fgets(line1, 100, fp1));
                } else {
                    scan1 = (fgets(line1, 100, fp1));
                }
            } else if (serie1->code > serie2->code) {
                if (serie2->isActive == letterY) {
                    fprintf(output_fp, "%s,%d,%d\n", serie2->serieName, serie2->code, serie2->numSeasons);
                    scan2 = (fgets(line2, 100, fp2));
                } else {
                    scan2 = (fgets(line2, 100, fp2));
                }
            }
            else if (serie1->code == serie2->code) {
                if ((serie1->isActive == letterY) && (serie2->isActive == letterY)) {
                    if (serie1->numSeasons > serie2->numSeasons) {
                        fprintf(output_fp, "%s,%d,%d\n", serie1->serieName, serie1->code, serie1->numSeasons);
                        scan1 = (fgets(line1, 100, fp1));
                        scan2 = (fgets(line2, 100, fp2));
                    }
                    else if (serie1->numSeasons <= serie2->numSeasons) {
                        fprintf(output_fp, "%s,%d,%d\n", serie2->serieName, serie2->code, serie2->numSeasons);
                        scan1 = (fgets(line1, 100, fp1));
                        scan2 = (fgets(line2, 100, fp2));
                    }
                }
                else if (serie1->isActive == letterY) {
                    fprintf(output_fp, "%s,%d,%d\n", serie1->serieName, serie1->code, serie1->numSeasons);
                    scan1 = (fgets(line1, 100, fp1));
                    scan2 = (fgets(line2, 100, fp2));
                }
                else if (serie2->isActive == letterY) {
                    fprintf(output_fp, "%s,%d,%d\n", serie2->serieName, serie2->code, serie2->numSeasons);
                    scan1 = (fgets(line1, 100, fp1));
                    scan2 = (fgets(line2, 100, fp2));
                }
                else {
                    scan1 = (fgets(line1, 100, fp1));
                    scan2 = (fgets(line2, 100, fp2));
                }
            }
        }
        scan1 = (fgets(line1, 100, fp1)); scan2 = (fgets(line2, 100, fp2));
        while (scan1 != NULL) {
            if (serie1->isActive == letterY) {
                fprintf(output_fp, "%s,%d,%d\n", serie1->serieName, serie1->code, serie1->numSeasons);
                scan1 = (fgets(line1, 100, fp1));
            }
            else{
                scan1 = (fgets(line1, 100, fp1));
            }
            }
        while (scan2 != NULL) {
            if (serie2->isActive == letterY) {
                fprintf(output_fp, "%s,%d,%d\n", serie2->serieName, serie2->code, serie2->numSeasons);
                scan2 = (fgets(line2, 100, fp2));
            }
            else{
                scan2 = (fgets(line2, 100, fp2));
            }
        }
        fclose(fp1); fclose(fp2); fclose(output_fp);
        free(line1); free(line2); free(serie1); free(serie2);
        FILE * fSeries = fopen("series.txt", "r");
        while (c != EOF) {
            c = (char) getc(fSeries);
            if (c == '\n') {
                seriesCount += 1;
            }
        }
        fclose(fSeries);
        unified = 1;
        printf("Unify Succeeded\n");
        return seriesCount;
    }
        return 0;
    }
serie ** getUnifyDatabaseDetails(){
    if(unified == 0) {
        fflush(stdin); fflush(stdout);
        printf("You must unify database before using this option!!!\n");
    }
    else if(unified == 2) {
        fflush(stdin); fflush(stdout);
        printf("Get Unify Database Details Has Been Done Already !!!\n");
    }
    else if(unified==1){
        FILE   * fp_input  = fopen("series.txt","r");
        FILE   * fp_output = fopen("watching.txt", "w");
        serie  * seriesArr = malloc(seriesAmount * sizeof(serie ));
        serie ** p2pArray  = malloc(seriesAmount * sizeof(serie *));
        char   * line_p    = malloc(100);
        int i = 0;
        while (fgets(line_p, 100, fp_input) != NULL) /* read a line from a file */ {
            sscanf(line_p, "%19[^,] , %d , %d  , %1s  ", seriesArr[i].serieName, &seriesArr[i].code, &seriesArr[i].numSeasons, &seriesArr[i].isActive);
            seriesArr[i].watchingDetails = calloc(seriesArr[i].numSeasons , sizeof(int));
            fflush(stdin); fflush(stdout);
            p2pArray[i] = &seriesArr[i];
            i++;
        }
        printf("Get Unify Database Details Succeeded\n");
        free(line_p);
        fclose(fp_input); fclose(fp_output);

        unified = 2;
        return p2pArray;
    }
    return NULL;
}

void getWatchingDetails(serie** aPointersArr) {
    if (unified == 0) {
        fflush(stdin); fflush(stdout);
        printf("You must unify database before using this option!!!\n");
    } else if (unified == 1) {
        fflush(stdin); fflush(stdout);
        printf("You must Get Unify Database Details before using this option!!!\n");
    } else {
        int serieCode, seasonNum, k;
        char serieCodeInput[5], seasonNumInput[5];
        FILE * pWatching;
        printf("Insert Serie Code:"); fflush(stdin);
        fgets(serieCodeInput,5, stdin);
        serieCode = stringToNumConvert(serieCodeInput);
        k = binarySearch(serieCode, aPointersArr);

        while (k < 0) {
            printf("wrong serie code, try again!!!\n"); fflush(stdin);
            fgets(serieCodeInput,10, stdin); fflush(stdin);
            serieCode = stringToNumConvert(serieCodeInput);
            k = binarySearch(serieCode, aPointersArr);
        }
        printf("Insert Season Number (1 - %d):", aPointersArr[k]->numSeasons); fflush(stdin);
        fgets(seasonNumInput,5, stdin); fflush(stdin);
        seasonNum = stringToNumConvert(seasonNumInput);
        while ((seasonNum < 1) || (seasonNum > aPointersArr[k]->numSeasons)) {
            printf("wrong season number, try again!!!\n"); fflush(stdin);
            fgets(seasonNumInput,5, stdin); fflush(stdin);
            seasonNum = stringToNumConvert(seasonNumInput);
        }
        aPointersArr[k]->watchingDetails[seasonNum-1] += 1;
        pWatching = fopen("watching.txt","a");
        fprintf(pWatching,"%03d,%d\n", aPointersArr[k]->code, seasonNum);
        fclose(pWatching);

        printf("series: %d, season: %d accepted!!!\n", aPointersArr[k]->code, seasonNum);
    }
}
void printWatchingDetails() {
    if (unified == 0) {
        fflush(stdin); fflush(stdout);
        printf("You must unify database before using this option!!!\n");
    } else if (unified == 1) {
        fflush(stdin); fflush(stdout);
        printf("You must Get Unify Database Details before using this option!!!\n");
    } else {
        FILE * pWatching; int code, season;
        pWatching = fopen("watching.txt","r");
        char * pLine = (char *)malloc(100*sizeof(char)); fflush(stdin);
        printf("Watching Details\n");
        while (fgets(pLine, 100, pWatching) != NULL) /* read a line from a file */ {
            sscanf(pLine,"%d,%d", &code, &season); fflush(stdin);
            printf("Series Code:%03d, Season Number:%d\n", code, season);
        }
        fclose(pWatching); free(pLine);
    }
}
int updateRank(serie ** aPointersArr) {
    int i, j, temp, maxRank = 0;
    for (i = 0; i < seriesAmount; i++) {
        temp =0;
        for (j = 0; j < aPointersArr[i]->numSeasons; j++) temp += aPointersArr[i]->watchingDetails[j];
        aPointersArr[i]->rank = temp;
        if(temp>maxRank) maxRank = temp;

    }
    return maxRank;
}
void printSeriesByCode(serie** aPointersArr){
    if(unified == 0) {
        fflush(stdin); fflush(stdout);
        printf("You must unify database before using this option!!!\n");
    }
    else if(unified == 1) {
        fflush(stdin); fflush(stdout);
        printf("You must Get Unify Database Details before using this option!!!\n");
    }
    else {
        serie temp;
        int i, j;
        for (i = 0;
             i < seriesAmount - 1; i++) {
            for (j = 0; j < seriesAmount - i - 1; j++) {
                if (aPointersArr[j]->code > aPointersArr[j + 1]->code) {
                    temp = *aPointersArr[j];
                    aPointersArr[j] = aPointersArr[j + 1];
                    aPointersArr[j + 1] = &temp;
                }
            }
        }
        printf("Series List\n");
        for (i = 0; i < seriesAmount; i++) {
            printf("%03d-%s(%d)\n", aPointersArr[i]->code, aPointersArr[i]->serieName, aPointersArr[i]->numSeasons);
        }
        printf("\n");
    }
}
void printSeriesByRank(serie ** aPointersArr){
    if (unified == 0) {
        fflush(stdin); fflush(stdout);
        printf("You must unify database before using this option!!!\n");
    } else if (unified == 1) {
        fflush(stdin); fflush(stdout);
        printf("You must Get Unify Database Details before using this option!!!\n");
    } else {
        int maxRank = updateRank(aPointersArr); int i,j,s;
        for(i=maxRank;i>=0;i--){
            printf("RANK %d\n", i);
            for(j=0;j<seriesAmount;j++){
                if(aPointersArr[j]->rank==i){
                    printf("%03d-%s\n", aPointersArr[j]->code, aPointersArr[j]->serieName);
                    for(s=0;s<aPointersArr[j]->numSeasons;s++){
                        printf("Season %d:%d\n", s+1 ,aPointersArr[j]->watchingDetails[s]);
                    }
                }
            }
        }
    }
}
void makeSeriesFileSortedByRank(serie** aPointersArr){
    if (unified == 0) {
        fflush(stdin); fflush(stdout);
        printf("You must unify database before using this option!!!\n");
    } else if (unified == 1) {
        fflush(stdin); fflush(stdout);
        printf("You must Get Unify Database Details before using this option!!!\n");
    } else {
        int maxRank = updateRank(aPointersArr); int i,j;
        FILE * fp_output = fopen("seriesRank.txt","w");
        for(i=maxRank;i>=0;i--){
            for(j=0;j<seriesAmount;j++){
                if(aPointersArr[j]->rank==i){
                    fprintf(fp_output,"%03d(Rank:%d)-%s\n", aPointersArr[j]->code,aPointersArr[j]->rank, aPointersArr[j]->serieName);
                }
            }
        }
        fclose(fp_output);
    }
}
int binarySearch(int num, serie ** aPointersArr){ // Finds the index of a series in the series array, searched for by given code
    int left = 0, right = seriesAmount - 1, middle = (left + right) / 2;
    if(aPointersArr[left]->code == num){
        return left;
    }
    else if(aPointersArr[right]->code == num){
        return right;
    }
    while(right-left>1){
        if(aPointersArr[middle]->code == num){
            return middle;
        }
        else if(aPointersArr[middle]->code > num) {
            right = middle;
            middle = (left + right)/2;
        }
        else if(aPointersArr[middle]->code < num){
            left = middle;
            middle = (left + right) / 2;
        }
        }
    return -1;
}
int getState(){
    char temp[200];
    fflush(stdin); fflush(stdout);
    fgets(temp,200,stdin);
    int state = stringToNumConvert(temp);
    return state;
}
int power(int a,int b) { // Compute a^b
    int i, result = 1;
    for (i = b; i > 0; i--) result *= a;
    return result;
}
int stringToNumConvert(char string[]) {
    int i, num = 0, spaceOrTab = 0, digitFound = 0;
    for(i=0;i<strlen(string);i++){
        if(string[i]=='\n'){
            string[i]='\0';
        }
    }
    for (i = 0; i < strlen(string); i++) {
        if ((string[i] == TAB) || string[i] == SPACE) {
            if (digitFound == 1) {
                spaceOrTab += 1;
            }
        } else {
            digitFound = 1;
            num += (int) (power(10, (int) (strlen(string) - i - 1))) * (string[i] - 48);
        }
    }
    if (spaceOrTab > 0) {
        num = num / (int) power(10, (spaceOrTab));
    }
    return num;
}
