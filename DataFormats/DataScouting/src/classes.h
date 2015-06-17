#include <vector>
#include "DataFormats/DataScouting/interface/ScoutingCaloJet.h"
#include "DataFormats/DataScouting/interface/ScoutingPFJet.h"
#include "DataFormats/DataScouting/interface/ScoutingPFCandidate.h"
#include "DataFormats/DataScouting/interface/ScoutingBeamSpot.h"
#include "DataFormats/DataScouting/interface/ScoutingVertex.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace DataFormats_DataScouting { 
    struct dictionary {
        ScoutingCaloJet a1;
        ScoutingBeamSpot a2;
        ScoutingPFCandidate a3;
        ScoutingPFJet a4;
        ScoutingVertex a5;
        ScoutingCaloJetCollection v1;
        ScoutingPFCandidateCollection v3;
        ScoutingPFJetCollection v4;
        ScoutingVertexCollection v5;
        edm::Wrapper<ScoutingCaloJetCollection> w1;
        edm::Wrapper<ScoutingBeamSpot> w2;
        edm::Wrapper<ScoutingPFCandidateCollection> w3;
        edm::Wrapper<ScoutingPFJetCollection> w4;
        edm::Wrapper<ScoutingVertexCollection> w5;
    };
}
