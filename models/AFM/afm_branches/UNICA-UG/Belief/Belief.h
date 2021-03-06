#ifndef BEL_INCL
#define BEL_INCL
#ifndef sigma
#define sigma 0.01
#endif
#define Pbins_number 5
//#include "Histogram.h"
#include "Asset.h"
#include "CAsset.h"
#include "Stock.h"
#include <stdlib.h>
#include "Random.h"
#include "Histogram.h"
#ifndef  DAYINMONTH 
#define  DAYINMONTH 20
#endif
#ifndef Belief_dty
#define Belief_dty
typedef struct Belief Belief;
struct Belief 
{
       double expectedPriceReturns;//dinamic array;
       double expectedTotalReturns;
       double expectedCashFlowYield;
       double volatility;
       Histogram hist;
       //int forwardWindow;
       //int backwardWindow;
       //int binsNumber;
       //double randomReturnWeigth;
       //double fundametalReturnWeigth;
       //double chartistReturnWeigth;
       //int holdingPeriodToForwardW;
       
};
#endif
/* it computes the belief about assets price returns and cash flow yealds and volatilities*/ 
Belief *newBelief();
void initializeBelief(Belief *belief);
void  assetBeliefFormation(Belief *belief, CAsset *assets,Random *rnd);

/*it compute the price return of the stocks as a weighted sum of different components*/
void computeStockExpectedPriceReturns(Belief *belief, Asset *asset);

double stockExpectedPriceReturns(Belief *belief, Asset *asset);
/*restituisce un istogramma delle frequenze dei expectedTotalReturn degli asset);*/

//Histogram *computePTHistogram(Belief *belief); 




void  assetBeliefFormation(Belief *belief, CAsset *assets,Random *rnd);

void computeStockExpectedPriceReturns(Belief *belief, Asset *asset);

double stockExpectedPriceReturns(Belief *belief,Asset *anAsset);

int forwardMonths(Belief *belief, int currentDay, int forwardWindow);
    
double horizonRetainedEarnings(Belief *belief,Stock *stock, int currentDay,int forwardWindow);

double futureFundamentalReturn(Belief *belief,Stock *stock,int currentDay,int forwardWindow);

double randomReturn(Belief *belief, Stock *stock,int backwardWindow,int forwardWindow, Random *rnd);

void  stockBeliefFormation(Belief *belief, Stock *stock,int backwardWindow,int forwardWindow, double randomWeight,double  fundamentalWeight,double chartistWeight, int bins ,int currentDay, Random *rnd);

 
#endif
