#ifndef AliMultiplicityEstimator_cxx
#define AliMultiplicityEstimator_cxx

//class TH1F;
//class TH1I;
//class TGraphErrors;
class AliMCEvent;
class AliMCParticle;
class AliHeader;
class AliStack;
class AliMCParticle;

#include "TNamed.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"
#include "TNtuple.h"

class AliMultiplicityEstimator : public TNamed {
 public:
  AliMultiplicityEstimator();
  AliMultiplicityEstimator(const char* name, const char* title,
			       Float_t eta_min_backwards, Float_t eta_max_backwards,
			       Float_t eta_min_forwards, Float_t eta_max_forwards);
  // Constructor making an estimator bypassing the physics selection
  AliMultiplicityEstimator(const char* name, const char* title);
  
  virtual ~AliMultiplicityEstimator() {}

  enum {
    kPROTON,
    kANTIPROTON,
    kLAMBDA,
    kANTILAMBDA,
    kK0S,
    kKPLUS,
    kKMINUS,
    kPIPLUS,
    kPIMINUS,
    kPI0,
    kXI,
    kANTIXI,
    kOMEGAMINUS,
    kOMEGAPLUS,
    kALLCHARGED,
    kNPID
  };

  Int_t pid_enum_to_pdg(Int_t pid_enum);
  void SetReferenceEstimator(const AliMultiplicityEstimator* e){fReferenceEstimator = e;}
  void SetMeasuresCharged(Bool_t b){fMeasuresCharged = b;}
  void SetNegateEstimatorRegion(Bool_t b){fnegate_estimator_region = b;}
  void RegisterHistograms(TList* outputList);
  Int_t GetNch() const {return fnch_in_estimator_region;}
  //get the ending of the name common to all histograms from this estimator:
  TString GetNamePostfix() {return TString("_") + fName;};
  //get the ending of the title common to all histograms from this estimator:
  TString GetTitlePostfix() {return TString(" ") + fTitle;};
  void PreEvent(Float_t ev_weight);
  Bool_t TrackSelection(AliMCParticle* track);
  void ProcessTrackForMultiplicityEstimation(AliMCParticle* track);
  void ProcessTrackWithKnownMultiplicity(AliMCParticle* track);
  void PostEvent(Int_t nMPI, Float_t q2, Bool_t fillNtuple);
  void Terminate(TList* sum);
  const Bool_t fuseWeights;   // Use event weights?

 protected:
  /*
    Header dependent logic is done here. This function is supposed to be called in the
    beginning of PreEvent().
   */
  void ReadEventHeaders(AliMCEvent* event);

  Int_t fnch_in_estimator_region;   // counter for charged particles in current event
  Float_t feta_min_forwards, feta_max_forwards;  // range in eta for mult. estimation (positive values)
  Float_t feta_min_backwards, feta_max_backwards;  // range in eta for mult. estimation (positive values)
  Bool_t fnegate_estimator_region; // negate the given region; ie. count particle outside of the region
  Bool_t fbypass_eta_selection;  // bypass the eta selection (used to get full eta range)
  Bool_t fMeasuresCharged;       // Do we accept charged or neutral particles in this acceptance?

  TH2F  *feta_Nch;          // dNdEta distributions; multiplicity is on the y-axis
  TH3F  *fNch_pT_pid;  // multiplicity class; pT; pid
  TNtuple *fNchTuple;
  TH2F  *fcorr_thisNch_vs_refNch;   // correlation between Nch in this estimator and the ref. estimator
  TH2F  *fNch_vs_nMPI;   // correlation between Nch in this estimator and the ref. estimator
  TH2F  *fNch_vs_Q2;   // correlation between Nch in this estimator and the ref. estimator
  Float_t feventWeight;        // weight of the event read from the header

 private:
  const AliMultiplicityEstimator* fReferenceEstimator;
  AliMultiplicityEstimator(const AliMultiplicityEstimator&);           // not implemented
  AliMultiplicityEstimator& operator=(const AliMultiplicityEstimator&);// not implemented


  ClassDef(AliMultiplicityEstimator, 1);
};

#endif
