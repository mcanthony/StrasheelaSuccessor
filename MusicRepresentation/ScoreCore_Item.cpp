//
//  ScoreCore_Item.cpp
//  MusicRepresentation
//
//  Created by torsten on 26/12/2013.
//  Copyright (c) 2013 torsten. All rights reserved.
//

#include "ScoreCore_Item.h"
#include "ScoreCore_Container.h"

// New item variables (parameters and container) are not supported as Args anyway, so no reduction necessary when forwarding
Item::Item(Args as) :
ScoreObject{as}
{}

std::vector<Parameter*> Item::getParameters(void) { return parameters; }

Container* Item::getContainer(void) { return container; }

void Item::setContainer(Container* c) { container = c; }

/*! [aux method] Parameters and Item *this are bidirectional linked. Function must not be called by user (only by designer of class with additional parameters).
 */
void Item::bilinkParameters(std::vector<Parameter*> ps) {
    for (auto p : ps) {
        parameters.push_back(p);
        p->setItem(this);
    }
}




