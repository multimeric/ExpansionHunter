//
// Expansion Hunter
// Copyright 2016-2019 Illumina, Inc.
// All rights reserved.
//
// Author: Egor Dolzhenko <edolzhenko@illumina.com>,
//         Mitch Bekritsky <mbekritsky@illumina.com>, Richard Shaw
// Concept: Michael Eberle <meberle@illumina.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//

#include "region_spec/LocusSpecification.hh"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "thirdparty/json/json.hpp"
#include "thirdparty/spdlog/spdlog.h"

#include "common/Common.hh"
#include "common/Reference.hh"

using boost::optional;
using graphtools::NodeId;
using std::map;
using std::ostream;
using std::string;
using std::to_string;
using std::vector;

using Json = nlohmann::json;

namespace spd = spdlog;

namespace ehunter
{
LocusSpecification::LocusSpecification(
    RegionId locusId, std::vector<GenomicRegion> targetReadExtractionRegions, AlleleCount expectedAlleleCount,
    graphtools::Graph regionGraph, NodeToRegionAssociation referenceRegions)
    : locusId_(std::move(locusId))
    , targetReadExtractionRegions_(std::move(targetReadExtractionRegions))
    , expectedAlleleCount_(expectedAlleleCount)
    , regionGraph_(std::move(regionGraph))
    , referenceRegions_(std::move(referenceRegions))
{
}

void LocusSpecification::addVariantSpecification(
    std::string id, VariantClassification classification, GenomicRegion referenceLocus, vector<NodeId> nodes,
    optional<NodeId> refNode)
{
    variantSpecs_.emplace_back(std::move(id), classification, std::move(referenceLocus), std::move(nodes), refNode);
}

const VariantSpecification& LocusSpecification::getVariantSpecById(const std::string& variantSpecId) const
{
    for (const auto& variantSpec : variantSpecs_)
    {
        if (variantSpec.id() == variantSpecId)
        {
            return variantSpec;
        }
    }

    throw std::logic_error("There is no variant " + variantSpecId + " in locus " + locusId_);
}

}
