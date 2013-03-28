#include "src/vision/BlobScorer.h"

double BlobScorer::getScore(double diff, double& maxDiff){
  if ( diff > maxDiff ){
    maxDiff = diff;
    return 0;
  } else {
    if ( maxDiff == 0 ) return 0;
    return 1 - (diff / maxDiff);
  }
}

void BlobScorer::prepareForScoring(double dRatio,double dPosition,double dSize,double dColor){
  computeScore(dRatio,dPosition,dSize,dColor,0,0,0,0);
}

double BlobScorer::computeScore(double dRatio, double dPosition, double dSize, double dColor,
                                double ratioWeight, double positionWeight, double sizeWeight,
                                double colorWeight){
  
  double positionScore = getScore(dPosition,maxdPosition);
  double ratioScore = getScore(dRatio,maxdRatio);
  double sizeScore = getScore(dSize,maxdSize);
  double colorScore = getScore(dColor,maxdColor);
  /**
  DEBUG("Position Difference: " + boost::lexical_cast<string>(dPosition));
  DEBUG("Position Score: " + boost::lexical_cast<string>(positionScore));
  DEBUG("Weighted Position Score: "  + boost::lexical_cast<string>(positionScore * positionWeight));
  DEBUG("Ratio Difference: " + boost::lexical_cast<string>(dRatio));
  DEBUG("Ratio Score: " + boost::lexical_cast<string>(ratioScore));
  DEBUG("Weighted Ratio Score: "  + boost::lexical_cast<string>(ratioScore * ratioWeight));
  DEBUG("Size Difference: " + boost::lexical_cast<string>(dColor));
  DEBUG("Size Score: " + boost::lexical_cast<string>(sizeScore));
  DEBUG("Weighted Size Score: "  + boost::lexical_cast<string>(sizeScore * sizeWeight));
  DEBUG("Color Difference: " + boost::lexical_cast<string>(dColor));
  DEBUG("Color Score: " + boost::lexical_cast<string>(colorScore));
  DEBUG("Weighted Color Score: "  + boost::lexical_cast<string>(colorScore * colorWeight));
  **/
  return positionScore * positionWeight +
         ratioScore * ratioWeight +
         sizeScore * sizeWeight + 
         colorScore * colorWeight;
}  
