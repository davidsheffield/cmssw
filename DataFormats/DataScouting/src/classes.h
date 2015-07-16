#include <vector>
#include "DataFormats/DataScouting/interface/ScoutingCaloJet.h"
#include "DataFormats/DataScouting/interface/ScoutingPFJet.h"
#include "DataFormats/DataScouting/interface/ScoutingParticle.h"
#include "DataFormats/DataScouting/interface/ScoutingVertex.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace DataFormats_DataScouting { 
    struct dictionary {
        edm::Wrapper<ScoutingCaloJetCollection> w1;
        edm::Wrapper<ScoutingParticleCollection> w2;
        edm::Wrapper<ScoutingPFJetCollection> w3;
        edm::Wrapper<ScoutingVertexCollection> w4;
    };
}
