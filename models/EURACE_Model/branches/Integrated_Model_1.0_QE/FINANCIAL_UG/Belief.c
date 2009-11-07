#include "my_library_header.h"





double expectedPriceReturns(Belief *belief)
{   
       return belief->expectedPriceReturns;
}

int forwardMonths( int currentDay, int forwardWindow)
{   
    int nrForwardMonths;
    //printf("%d %d %d",currentDay,forwardWindow,DAYS_IN_MONTH);
    nrForwardMonths=((currentDay+forwardWindow)/ DAYS_IN_MONTH)-(currentDay/DAYS_IN_MONTH);
    return nrForwardMonths;
}
    
double horizonRetainedEarnings(Belief *belief, int currentDay,int forwardWindow)
{
       int forwardmonths;
       double hRetainedEarnings;
       double earningExp;
       double earningPayoutExp;
       earningExp=belief->expectedEarning;
       earningPayoutExp=belief->expectedEarningPayout;
       forwardmonths=forwardMonths(currentDay,forwardWindow);
       hRetainedEarnings= forwardmonths*(earningExp-earningPayoutExp);  
       return hRetainedEarnings;
}

double futureFundamentalReturn(Belief *belief,Stock *stock,int currentDay,int forwardWindow, double equity)
{ double futureFundPrice;
  double fundamentalReturn;
  int hRetainedEarnings;
  hRetainedEarnings=horizonRetainedEarnings(belief,currentDay,forwardWindow);
  futureFundPrice = (equity+hRetainedEarnings)/(stock->nrOutStandingShares);
 // printf(" return%f\n",futureFundPrice);
  fundamentalReturn = (futureFundPrice-lastPriceStock(stock))/lastPriceStock(stock);
  return fundamentalReturn;
       }

double randomReturnStock(Belief *belief, Stock *stock, int forwardWindow, int backwardWindow,double *rndvect)
{  double rndReturn;
   double volatility;
   double randn;
   double sqrtvol;
   int i;
   volatility=volatilityStock(stock,backwardWindow);
  // printf("volatility %f\n",volatility);
   volatility=max(0.0001,volatility);
   rndReturn=0;
   sqrtvol=sqrt(forwardWindow)*volatility;
   for(i=0;i<backwardWindow;i++)
    { 
     randn=fast_gauss();
     rndvect[i]= sqrtvol*randn;
     rndReturn=rndReturn+rndvect[i];
             
    }
   return rndReturn/backwardWindow;
}

double randomReturnBond(Bond *bond,int backwardWindow,int holding_period,double *rndvect)
{  double rndReturn;
   double volatility;
   double randn;
    double factor;
   int i;
   
   volatility=volatilityBond(bond,backwardWindow);
   //printf("\n backwardWindow %d",backwardWindow);
   volatility=max(0.0001,volatility);
   rndReturn=0;
   factor=sqrt(holding_period)*volatility;
   for(i=0;i<backwardWindow;i++)
    { randn=fast_gauss();
     rndvect[i]= factor*randn;
     rndReturn=rndReturn+rndvect[i];
     }
        
   return rndReturn/backwardWindow;
}

void dividendYield(Belief *belief,Stock *stock,int currentDay, int forwardWindow,double dividendExp)
   {  int fwmonths;
      double divYield;
      fwmonths=forwardMonths(currentDay, forwardWindow);
      divYield=(fwmonths*(dividendExp))/lastPriceStock(stock);
      belief->expectedCashFlowYield=divYield;
   }


void firmBeliefFormation(Belief *belief,Stock *stock,int currentDay,int forwardWindow,double dividendExp,double earning,double earningPayout)
{
  belief->expectedEarning=earning;
  belief->expectedEarningPayout=earningPayout;
  dividendYield(belief,stock,currentDay, forwardWindow,dividendExp);
}

void  stockBeliefFormation(Belief *belief, Stock *stock,int backwardWindow,int forwardWindow, double randomWeight,double  fundamentalWeight,double chartistWeight, int bins ,int currentDay,double equity,double lossaversion)
{ 
  double fundreturn;
  double rndreturn[100];
  double total_returns_avg;
  double price_returns_avg;
  double returns_avg;
  double rnd_avg;
  double value,factor,dividend_yield_annualized,annual_coeff;
 
  annual_coeff=(NRDAYSINYEAR/forwardWindow);
 /*some of the  behavior weights are usually zero so some computation could be unuseful, this increase certanly the performance,*/
  if(randomWeight!=0) rnd_avg=randomReturnStock(belief, stock,forwardWindow,backwardWindow,rndreturn);
     else rnd_avg=0;
  if (fundamentalWeight!=0) fundreturn= futureFundamentalReturn(belief,stock,currentDay,forwardWindow, equity);
     else fundreturn=0;
  if(chartistWeight!=0) returns_avg=expectedReturnStock(stock,backwardWindow);
     else returns_avg=0;
  value= fundamentalWeight*fundreturn + belief->expectedCashFlowYield; 
  factor=forwardWindow*chartistWeight;
  price_returns_avg=((randomWeight*rnd_avg)+(fundamentalWeight*fundreturn)+(chartistWeight*forwardWindow*returns_avg))*annual_coeff;
  dividend_yield_annualized=belief->expectedCashFlowYield*annual_coeff;
  total_returns_avg=price_returns_avg+ dividend_yield_annualized;
  belief->utility= computeStockUtilityFunction(stock, backwardWindow, factor,  value, lossaversion,rndreturn, randomWeight);
  belief->expectedPriceReturns=price_returns_avg;
  belief->expectedTotalReturns=total_returns_avg;
  belief->last_price=lastPriceStock(stock);
}



void  bondBeliefFormation(Belief *belief, Bond *bond,int backwardWindow,int forwardWindow, double randomWeight,double  fundamentalWeight,double chartistWeight, int bins ,int currentDay,int holdingPeriodToForwardW, double lossaversion)
{
  FILE *file1=NULL;
  char * filename="";
  //int bins_number;
  int holding_period;
  int nrCoupons;
  double coupon, coupon_stream, coupon_yield, coupon_yield_annualized, last_market_price;
  double random_return_weight_bond, fundamental_return_weight_bond, chartist_return_weight_bond;
  double return_rnd;
  return_rnd =0.0;
  double return_fundamental, return_chartist, return_random;
  double days2maturity;
  double factor_chartist, factor_random, factor_fundamental, factor_chartist2;
  double annual_coeff;
  double rndreturns[100];

  holding_period=holdingPeriodToForwardW*forwardWindow;
  nrCoupons=coupons_payment_days(bond,currentDay,holding_period);
  //printf("numero di coupon%d\n",nrCoupons);
  last_market_price=lastPriceBond(bond);
  annual_coeff=(NRDAYSINYEAR/holding_period);

if (nrCoupons>0)
    {
    coupon = ((bond->nominal_yield*bond->face_value)*(COUPONPERIODICITYNRMONTHS)/12);
    coupon_stream = nrCoupons*coupon;
    coupon_yield = coupon_stream/last_market_price;
    }
else coupon_yield = 0;

coupon_yield_annualized = annual_coeff*coupon_yield;

// Fundamental Return
days2maturity = bond->maturity_day-currentDay;

// weights rescaling for bonds
fundamental_return_weight_bond = max(FUNDAMENTAL_RETURN_WEIGHT_MIN,fundamentalWeight);
if (fundamental_return_weight_bond==1)
    {
    random_return_weight_bond = 0;
    chartist_return_weight_bond = 0;
    }
else
    {
    random_return_weight_bond = (randomWeight/(randomWeight+chartistWeight))*(1-fundamental_return_weight_bond);
    chartist_return_weight_bond = (chartistWeight/(randomWeight+chartistWeight))*(1-fundamental_return_weight_bond);
    }

// factor computation
if (holding_period>=days2maturity)
    {factor_fundamental = 1; 
     factor_chartist=0;
     factor_random=0;
    }
else
   {
     factor_fundamental = fundamental_return_weight_bond;
     factor_chartist = chartist_return_weight_bond;
     factor_random = random_return_weight_bond;
   }

// The three components of returns expected in the holding period
return_fundamental = (min(holding_period,days2maturity)/days2maturity)*(bond->face_value-last_market_price)/last_market_price;
return_chartist = holding_period*expectedReturnBond(bond,backwardWindow);
return_random = randomReturnBond(bond, backwardWindow, holding_period, rndreturns);
   

//return_rnd = sumvector(rndreturns,backwardWindow)/backwardWindow;
belief->expectedPriceReturns=annual_coeff*(factor_fundamental*return_fundamental+factor_chartist*return_chartist+ factor_random*return_random);
//if (abs(belief->expectedPriceReturns)/NRDAYSINYEAR > 1)
 //  printf("\n bondBeliefFormation \n exp_price_ret %f fund %f char %f rnd %f ",belief->expectedPriceReturns, value, factor_chartist*returns_char,factor_random*return_rnd);

belief->expectedTotalReturns = belief->expectedPriceReturns + coupon_yield_annualized;

factor_chartist2 = holding_period*factor_chartist;
belief->utility = computeBondUtilityFunction(bond, rndreturns, backwardWindow, return_fundamental, factor_chartist2, factor_random, factor_fundamental, lossaversion);

belief->last_price=lastPriceBond(bond);


      if (PRINT_DEBUG_FILE_EXP1)
    {                       
        filename = malloc(40*sizeof(char));
        filename[0]=0;
        strcpy(filename, "its/households_bonds_beliefs.txt");      
        file1 = fopen(filename,"a");
        fprintf(file1,"%d %f %d %f %f %f",CURRENTDAY,bond->nominal_yield,nrCoupons,coupon_stream,last_market_price,coupon_yield_annualized);
        fprintf(file1," %f %f %f %f %f %f",factor_random,return_random,factor_chartist,return_chartist,factor_fundamental,return_fundamental);
        fprintf(file1," %f %f\n",belief->expectedPriceReturns,belief->expectedTotalReturns);
        fclose(file1);
        free(filename);        
    }
  
}
