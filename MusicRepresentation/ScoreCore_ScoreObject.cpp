//
//  ScoreScore_ScoreObject.cpp
//  MusicRepresentation
//
//  Created by torsten on 26/12/2013.
//  Copyright (c) 2013 torsten. All rights reserved.
//

#include "ScoreCore_ScoreObject.h"

using namespace std;

/** [constructor with map argument for optional/named arguments]
 Args:
 string info: arbitrary user information for this score object (additional infos can be added with nmember function addInfo)
 */
// TODO: handle argument could be added later: arg is *ScoreObject, which is bound in constructor to *this
ScoreObject::ScoreObject(args as) {
    // process arg info
    if (as.count("info")){
        info.push_back(boost::apply_visitor(getStringArg(), as.at("info")));
        as.erase("info");
    }
    // raise exception if there are any remaining args
    for(auto x: as)
        throw std::invalid_argument{"ScoreObject::ScoreObject(args as)" + x.first};
};

/** Returns vectors of all info strings stored. */
vector<string> ScoreObject::getInfo(void) { return info; }

/** [destructive method] Adds myInfo to vector of stored infos. */
void ScoreObject::addInfo(string myInfo) { info.push_back(myInfo); }

/** Returns bool whether internal vector of info strings contains myInfo. */
bool ScoreObject::hasThisInfo(string myInfo) {
    return any_of(info.begin(), info.end(), [&](string s){return s.compare(myInfo) == 0;});
}
