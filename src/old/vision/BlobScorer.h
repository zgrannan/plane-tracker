#ifndef BLOBSCORER_H
#define BLOBSCORER_H
class BlobScorer{
  public: 
    BlobScorer(){}
    double computeScore(double dRatio, double dPosition, double dSize, double dColor,
                        double ratioWeight, double positionWeight, double sizeWeight,
                        double colorWeight);
    void prepareForScoring(double dRatio, double dPosition, double dSize, double dColor);
  private:
    double getScore(double diff, double& maxdiff);
    double maxdRatio = 0;
    double maxdPosition = 0;
    double maxdSize = 0;
    double maxdColor = 0;
};
#endif
