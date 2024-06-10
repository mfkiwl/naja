// SPDX-FileCopyrightText: 2024 The Naja authors <https://github.com/najaeda/naja/blob/main/AUTHORS>
//
// SPDX-License-Identifier: Apache-2.0

#include "gtest/gtest.h"

#include "SNLUniverse.h"
#include "SNLDB.h"
#include "SNLLibrary.h"
#include "SNLDesign.h"
#include "SNLScalarTerm.h"
#include "SNLBusTerm.h"
#include "SNLBusTermBit.h"
#include "SNLInstTerm.h"
#include "SNLScalarNet.h"
#include "SNLBusNet.h"
#include "SNLException.h"
using namespace naja::SNL;

class SNLInstanceSetModelTest1: public ::testing::Test {
  protected:
    void SetUp() override {
      auto universe = SNLUniverse::create();
      auto db = SNLDB::create(universe);
      auto library = SNLLibrary::create(db, SNLName("MYLIB"));
      model0_ = SNLDesign::create(library, SNLName("model0"));

      //model0 terms
      SNLScalarTerm::create(model0_, SNLTerm::Direction::Input, SNLName("term0"));
      SNLBusTerm::create(model0_, SNLTerm::Direction::Input, 4, 0, SNLName("term1"));
      SNLScalarTerm::create(model0_, SNLTerm::Direction::Output, SNLName("term2"));
      SNLBusTerm::create(model0_, SNLTerm::Direction::Output, -2, 5, SNLName("term3"));
      SNLScalarTerm::create(model0_, SNLTerm::Direction::InOut, SNLName("term4"));
      SNLParameter::create(model0_, SNLName("param0"), SNLParameter::Type::Binary, "0b1010");
      SNLParameter::create(model0_, SNLName("param1"), SNLParameter::Type::Decimal, "42");
      SNLParameter::create(model0_, SNLName("param2"), SNLParameter::Type::Boolean, "true");

      top_ = SNLDesign::create(library, SNLName("top"));
      //Top terms
      auto topi0 = SNLScalarTerm::create(top_, SNLTerm::Direction::Input, SNLName("topi0"));
      auto topi1 = SNLBusTerm::create(top_, SNLTerm::Direction::Input, 4, 0, SNLName("topi1"));
      auto topo0 = SNLScalarTerm::create(top_, SNLTerm::Direction::Output, SNLName("topo0"));
      
      ins0_ = SNLInstance::create(top_, model0_, SNLName("ins0"));
      //instParams
      SNLInstParameter::create(ins0_, model0_->getParameter(SNLName("param0")), "0b0101");
      ins1_ = SNLInstance::create(top_, model0_, SNLName("ins1"));

      //nets
      auto net0 = SNLScalarNet::create(top_, SNLName("topi0"));
      topi0->setNet(net0);
      auto net1 = SNLBusNet::create(top_, 4, 0, SNLName("topi1"));
      topi1->setNet(net1);
      ins0_->getInstTerm(model0_->getScalarTerm(SNLName("term0")))->setNet(net0);
      ins1_->getInstTerm(model0_->getScalarTerm(SNLName("term0")))->setNet(net0);
    }
    void TearDown() override {
      SNLUniverse::get()->destroy();
    }
    SNLDesign*    model0_ {nullptr};
    SNLDesign*    model1_ {nullptr};
    SNLDesign*    top_    {nullptr};
    SNLInstance*  ins0_   {nullptr};
    SNLInstance*  ins1_   {nullptr};
};