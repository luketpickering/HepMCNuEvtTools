#include "NuHepMC/Kinematics.hxx"
#include "NuHepMC/ParticleStackReaderHelper.hxx"

#include <iostream>

namespace NuHepMC {

HepMC3::FourVector GetFourMomentumTransfer(HepMC3::GenEvent const &evt) {
  auto ISProbe = GetProbe(evt);
  if (!ISProbe) {
    return HepMC3::FourVector::ZERO_VECTOR();
  }

  auto FSProbe = GetFSProbe(evt, ISProbe->pid());
  if (!FSProbe) {
    return HepMC3::FourVector::ZERO_VECTOR();
  }

  return (ISProbe->momentum() - FSProbe->momentum());
}

double GetQ2(HepMC3::GenEvent const &evt) {
  return -GetFourMomentumTransfer(evt).m2();
}

} // namespace NuHepMC
