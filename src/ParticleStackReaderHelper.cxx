
#include "NuHepMC/ParticleStackReaderHelper.hxx"

// #define NUEVTHELP_DEBUG

#include <unordered_set>
#ifdef NUEVTHELP_DEBUG
#include <iostream>
#endif

namespace NuHepMC {

HepMC3::ConstGenVertexPtr GetLabFrameVertex(HepMC3::GenEvent const &evt) {
  for (auto vtx : evt.vertices()) {
    if (vtx->status() == labels::e2i(labels::VertexState::kLabFrame)) {
      return vtx;
    }
  }
  return nullptr;
}

HepMC3::ConstGenVertexPtr GetHardScatterVertex(HepMC3::GenEvent const &evt) {
  for (auto vtx : evt.vertices()) {
    if (vtx->status() == labels::e2i(labels::VertexState::kHardScatter)) {
      return vtx;
    }
  }
  return nullptr;
}

std::vector<HepMC3::ConstGenParticlePtr>
GetParticles(HepMC3::ConstGenVertexPtr vtx, labels::ParticleState st) {
  std::vector<HepMC3::ConstGenParticlePtr> rtnlist;

  // These methods only deal with the lab frame vertex
  if (st == labels::ParticleState::kOther) {
    return {};
  }

  if (!vtx) {
    return {};
  }

  return (st == labels::ParticleState::kInitialState) ? vtx->particles_in()
                                                      : vtx->particles_out();
}

std::vector<HepMC3::ConstGenParticlePtr>
GetParticles(HepMC3::GenEvent const &evt, labels::ParticleState st) {
  return GetParticles(GetLabFrameVertex(evt), st);
}

std::vector<HepMC3::ConstGenParticlePtr>
GetHardScatterISParticles(HepMC3::GenEvent const &evt) {

  HepMC3::ConstGenVertexPtr vtx = GetHardScatterVertex(evt);

  if (!vtx) {
    return {};
  }

  return vtx->particles_in();
}

std::vector<HepMC3::ConstGenParticlePtr>
GetParticles(HepMC3::GenEvent const &evt, int pid, labels::ParticleState st) {

#ifdef NUEVTHELP_DEBUG
  std::cout << "[INFO]<<: Looking for particles in state: " << st
            << ", with pid: " << pid << std::endl;
#endif

  std::vector<HepMC3::ConstGenParticlePtr> rtnlist;

  for (auto part : GetParticles(evt, st)) {
    if (part->pid() == pid) {
#ifdef NUEVTHELP_DEBUG
      std::cout << "[INFO]: Found one, " << part->pid()
                << ", E = " << part->momentum().e() << std::endl;
#endif
      rtnlist.push_back(part);
    }
  }

  return rtnlist;
}

std::vector<HepMC3::ConstGenParticlePtr>
GetParticles(HepMC3::GenEvent const &evt, std::vector<int> const &pids,
             labels::ParticleState st) {
  std::vector<HepMC3::ConstGenParticlePtr> rtnlist;

  std::unordered_set<int> distinct_pids(pids.begin(), pids.end());

#ifdef NUEVTHELP_DEBUG
  std::cout << "[INFO]: Looking for particles in state: " << st
            << ", with pids in: {";
  for (int pid : distinct_pids) {
    std::cout << pid << ", ";
  }
  std::cout << "}." << std::endl;
#endif

  for (int pid : distinct_pids) {
    for (auto part : GetParticles(evt, pid, st)) {
      rtnlist.push_back(part);
    }
  }

  return rtnlist;
}

HepMC3::ConstGenParticlePtr GetProbe(HepMC3::GenEvent const &evt) {
  return GetHMISParticle(evt, std::vector<int>{12, -12, 14, -14, 11});
}

HepMC3::ConstGenParticlePtr GetFSProbe(HepMC3::GenEvent const &evt, int pid) {

  int matter_sign = pid > 0 ? 1 : -1;

  switch (std::abs(pid)) {
  case 14: {
    return GetHMFSParticle(
        evt, std::vector<int>{matter_sign * (std::abs(pid) - 1), pid});
  }
  case 12: {
    return GetHMFSParticle(
        evt, std::vector<int>{matter_sign * (std::abs(pid) - 1), pid});
  }
  case 11: {
    return GetHMFSParticle(evt, pid);
  }
  default: {
    return nullptr;
  }
  }
}

HepMC3::ConstGenParticlePtr GetFSProbe(HepMC3::GenEvent const &evt) {
  auto ISProbe = GetProbe(evt);

  return GetFSProbe(evt, ISProbe->pid());
}

} // namespace NuHepMC
