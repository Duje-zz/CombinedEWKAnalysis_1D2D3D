/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooAbsData.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooACSemiAnalyticPdf_1D.h" 

#include <math.h> 
#include "TMath.h" 
#include "RooFormulaVar.h"
#include "RooRealVar.h"
#include "RooFit.h"

#include "TFile.h"

//using namespace RooFit ;

ClassImp(RooACSemiAnalyticPdf_1D) 

RooACSemiAnalyticPdf_1D::RooACSemiAnalyticPdf_1D() : 
  type_(notype), P_par1_histo(0), P_par1_TF(0)
{
  initializeProfiles();
}

RooACSemiAnalyticPdf_1D::RooACSemiAnalyticPdf_1D(const char *name, 
						 const char *title, 
						 RooAbsReal& _x,
						 RooAbsReal& _par1,
						 RooAbsReal& _SM_shape,
						 const char * parFilename,
						 const unsigned& lt) :
  RooAbsPdf(name,title),
  x("observable","observable",this,_x),
  par1("par1","par1",this,_par1),
  SM_shape("SM_shape","SM_shape",this,_SM_shape),
  type_((LimitType)lt),
  profileFilename(parFilename),
  P_par1_histo(0),
  P_par1_TF(0)
{ 
  initializeProfiles();
  initializeNormalization(std::string(""),_x,_SM_shape);
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(parFilename,"READ");  
  gDirectory->cd(pwd);

  std::vector<double> bins_;

  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(_x);
  int N_bins=rdep.getBinning("").numBins();
  
  for (int i=0;i<N_bins;i++){
    if (i==0){
      bins_.push_back(rdep.getBinning("").binLow(i));
    }
    bins_.push_back(rdep.getBinning("").binHigh(i));
  }
  
  readProfiles(bins_,*f);
  f->Close();
} 

RooACSemiAnalyticPdf_1D::RooACSemiAnalyticPdf_1D(const RooACSemiAnalyticPdf_1D& other, 
					       const char* name) :  
  RooAbsPdf(other,name),
  x("observable",this,other.x),
  par1("par1",this,other.par1),
  SM_shape("SM_shape",this,other.SM_shape),
  type_(other.type_),
  integral_basis(other.integral_basis),
  bins(other.bins),
  profileFilename(other.profileFilename),
  P_par1_histo(0),
  P_par1_TF(0)
{ 
  initializeProfiles();
  initializeBins(x.arg());
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(profileFilename,"READ");  
  gDirectory->cd(pwd);  
  readProfiles(bins,*f);
  f->Close();
} 

void RooACSemiAnalyticPdf_1D::initializeProfiles() {
  int const N_bins=20;
  
  P_par1_histo = new TH1D*[N_bins]();
  P_par1_TF = new TF1*[N_bins]();
  
}


void RooACSemiAnalyticPdf_1D::initializeBins(const RooAbsReal& dep) const {

  std::vector<double> bins;
  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(dep);
  int N_bins=rdep.getBinning("").numBins();
  for(int i=0; i<N_bins; ++i) {
    if (i==0){
      bins.push_back(rdep.getBinning("").binLow(i));
    }
    bins.push_back(rdep.getBinning("").binHigh(i));

  }
}


void RooACSemiAnalyticPdf_1D::initializeNormalization(const std::string& rName,
						     const RooAbsReal& dep,
						     const RooAbsReal& shape) const {
  integral_basis[rName] = std::vector<double>();


  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(dep);
  int N_bins=rdep.getBinning("").numBins();
  int i;
  for(i=0; i<N_bins; ++i) {


    Double_t bin_low=rdep.getBinning("").binLow(i);
    Double_t bin_high=rdep.getBinning("").binHigh(i);

    if (i==0){
      bins.push_back(rdep.getBinning("").binLow(i));
    }
    bins.push_back(rdep.getBinning("").binHigh(i));

    RooRealVar& b = const_cast<RooRealVar&>(rdep);

    const string intRange="integRange";

    b.setRange((const char*)intRange.c_str(),bin_low,bin_high); 
    RooAbsReal* integral = shape.createIntegral(RooArgSet(rdep),RooArgSet(),(const char*)intRange.c_str());

    integral_basis[rName].push_back(integral->getVal());

    delete integral;
  }


}

void RooACSemiAnalyticPdf_1D::readProfiles(std::vector<double> bins,TDirectory& dir) const {

  int N_bins;
  int i;
  switch(type_) {
  case par1_TH1:
  case par1_TF1:
    N_bins=bins.size()-1;
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;
  }
  for(i=0; i<N_bins; ++i) {
    if (P_par1_histo[i]) delete P_par1_histo[i];
    if (P_par1_TF[i]) delete P_par1_TF[i];
    TString par2name = TString::Format("bin_content_par1_%i",i+1);

    switch(type_) {
    case par1_TH1:
      P_par1_histo[i] = dynamic_cast<TH1D *>(dir.Get(par2name)->Clone(par2name+"new"));
      P_par1_histo[i]->SetDirectory(0);
      break;
    case par1_TF1:
      P_par1_TF[i] = dynamic_cast<TF1 *>(dir.Get(par2name)->Clone(par2name+"new"));
      break;
    default:
      assert(NULL && "invalid limit type!");
      break;
    }

  }

}

void RooACSemiAnalyticPdf_1D::readProfiles(RooACSemiAnalyticPdf_1D const& other) {


  int N_bins;
  switch(type_) {
  case par1_TH1:
  case par1_TF1:
    N_bins=bins.size()-1;
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;
  }


  for (int i = 0; i<N_bins; ++i) {
    TString par2name = TString::Format("bin_content_par1_%i",i+1);

    switch(type_) {
    case par1_TH1:
      P_par1_histo[i] = dynamic_cast<TH1D *>(other.P_par1_histo[i]->Clone(par2name+"new"));
      P_par1_histo[i]->SetDirectory(0);
      break;
    case par1_TF1:
      P_par1_TF[i] = dynamic_cast<TF1 *>(other.P_par1_histo[i]->Clone(par2name+"new"));
      //      P_par1_TF[i]->SetDirectory(0);
      break;
    default:
      assert(NULL && "invalid limit type!");
      break;
    }

  }
}

RooACSemiAnalyticPdf_1D::~RooACSemiAnalyticPdf_1D() {
  int const N_bins=20;

  for(int i = 0; i<N_bins; ++i) {
    if (P_par1_histo[i])
      delete P_par1_histo[i];
    if (P_par1_TF[i])
      delete P_par1_TF[i];
  }
  delete[] P_par1_histo;
  delete[] P_par1_TF;
}

Double_t RooACSemiAnalyticPdf_1D::evaluate() const 
{ 


  TH1D ** P_histo = NULL;
  TF1 ** P_TF = NULL;
  double v1(0.0);
  
  P_histo = P_par1_histo;
  P_TF = P_par1_TF;
  v1 = par1;
  
  if (not P_histo[0] and not P_TF[0]) {
    TFile f(profileFilename);
    readProfiles(bins,f); 
    f.Close();
  }

  switch(type_) {
  case par1_TH1:
    if (v1 < P_histo[0]->GetXaxis()->GetXmin())
      v1 = P_histo[0]->GetXaxis()->GetXmin();
    if (v1 > P_histo[0]->GetXaxis()->GetXmax())
      v1 = P_histo[0]->GetXaxis()->GetXmax();
    break;
  case par1_TF1:    
    if (v1 < P_TF[0]->GetXmin())
      v1 = P_TF[0]->GetXmin();
    if (v1 > P_TF[0]->GetXmax())
      v1 = P_TF[0]->GetXmax();
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;

  }  

  double ret(0.);

  int N_bins=bins.size()-1;

    int bin_with_x=1;
    for (int j=0;j<N_bins;j++)
      if (x<bins[j+1] && x>bins[j]) bin_with_x=j+1;
    
    switch(type_) {
    case par1_TH1:
      ret = P_histo[bin_with_x-1]->Interpolate(v1)*SM_shape;
      break;
    case par1_TF1:
      ret = P_TF[bin_with_x-1]->Eval(v1)*SM_shape;
      break;
    case notype:
      assert(NULL && "invalid limit type!");
      break;
    }

  
  if (ret < 0.) ret = 0.;
  return ret; 
}

Int_t RooACSemiAnalyticPdf_1D::
getAnalyticalIntegral(RooArgSet& allVars,RooArgSet& analVars, 
		      const char* /*rangeName*/) const {  
  if (matchArgs(allVars,analVars,x)) return 1 ;
  return 0 ;
}

Double_t RooACSemiAnalyticPdf_1D::
analyticalIntegral(Int_t code, const char* rangeName) const {  

  assert(code==1 && "invalid analytic integration code!");

  std::string rName = ( rangeName == 0 ? std::string("") : std::string(rangeName) );
  if( integral_basis.find(rName) == integral_basis.end() ) {
    initializeNormalization(rName,x.arg(),SM_shape.arg());
  }

  TH1D ** P_histo = NULL;
  TF1 ** P_TF = NULL;
  double v1(0.0);
  
  P_histo = P_par1_histo;
  P_TF = P_par1_TF;
  v1 = par1;
  

  if (not P_histo[0] and not P_TF[0]) {
    TFile f(profileFilename);
    readProfiles(bins,f);
    f.Close();
  }

  double ret(0.);

  int N_bins=bins.size()-1;
  for(Int_t i=0 ; i<N_bins ; i++) {
    switch(type_) {
    case par1_TH1:
      ret += P_histo[i]->Interpolate(v1)*integral_basis[rName][i];
      break;
    case par1_TF1:
      ret += P_TF[i]->Eval(v1)*integral_basis[rName][i];
      break;
    default:
      assert(code==1 && "invalid limit type!");
      break;
    }

  }
  cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@--> par1(v1): "<<v1 <<" ret: "<<ret<< endl;
  
  cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@---> integral: "<<ret<< endl;

  return ret;
}