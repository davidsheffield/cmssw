#include <vector>
#include "DataFormats/DataScouting/interface/ScoutingCaloJet.h"
#include "DataFormats/DataScouting/interface/ScoutingPFJet.h"
#include "DataFormats/DataScouting/interface/ScoutingPFCandidate.h"
#include "DataFormats/DataScouting/interface/ScoutingVertex.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace DataFormats_DataScouting { 
    struct dictionary {
        ScoutingCaloJet a1;
        ScoutingPFCandidate a2;
        ScoutingPFJet a3;
        ScoutingVertex a4;
        ScoutingCaloJetCollection v1;
        ScoutingPFCandidateCollection v2;
        ScoutingPFJetCollection v3;
        ScoutingVertexCollection v4;
        edm::Wrapper<ScoutingCaloJetCollection> w1;
        edm::Wrapper<ScoutingPFCandidateCollection> w2;
        edm::Wrapper<ScoutingPFJetCollection> w3;
        edm::Wrapper<ScoutingVertexCollection> w4;
    };
}
