#include <iostream>
#include <map>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "IntranuclearCascade/Constants.hh"
#include "IntranuclearCascade/FourVector.hh"
#include "IntranuclearCascade/Interactions.hh"
#include "IntranuclearCascade/Particle.hh"
#include "IntranuclearCascade/ThreeVector.hh"
#include "IntranuclearCascade/Utilities.hh"

using namespace H5;
using namespace IntranuclearCascade;

REGISTER_INTERACTION(GeantInteractions);
REGISTER_INTERACTION(NasaInteractions);
REGISTER_INTERACTION(ConstantInteractions);

const std::map<std::string, double> HZETRN = {
    {"a", 5.0_MeV},
    {"b", 0.199/sqrt(1_MeV)},
    {"c", 0.451 * pow(1_MeV, -0.258)},
    {"d", 25.0_MeV},
    {"e", 134.0_MeV},
    {"f", 1.187 * pow(1_MeV, -0.35)},
    {"g", 0.1_MeV},
    {"h", 0.282_MeV}
};

const std::map<std::string, double> PDG = {
    {"Zpp", 33.45_mb},  
    {"Zpn", 35.80_mb},  
    {"Y1pp", 42.53_mb}, 
    {"Y1pn", 40.15_mb}, 
    {"Y2pp", 33.34_mb}, 
    {"Y2pn", 30.00_mb}, 
    {"B", 0.308_mb},    
    {"s1", 1.0 * pow(1_GeV, 2)},
    {"s0", pow(5.38_GeV, 2)},
    {"n1", 0.458},
    {"n2", 0.545}
};

const std::map<std::string, double> JWN = {
    {"gamma", 52.5 * pow(1_GeV, 0.16)}, //mb
    {"alpha", 0.00369 / 1_MeV},
    {"beta", 0.00895741 * pow(1_MeV, -0.8)}
};

double IntranuclearCascade::CrossSection(bool, const double&) {
    // TODO: Implement GEANT4 cross-section or something else
    throw std::domain_error("Invalid energy!");
}

double IntranuclearCascade::CrossSectionLab(bool samePID, const double& pLab) noexcept {
    const double tLab = sqrt(pow(pLab, 2) + pow(Constant::mN, 2)) - Constant::mN;
    if(samePID) {
        if(pLab < 1.8_GeV) {
            if(tLab >= 25_MeV)
                return (1.0+HZETRN.at("a")/tLab) * (40+109.0*std::cos(HZETRN.at("b")*sqrt(tLab))
                        * exp(-HZETRN.at("c")*pow(tLab-HZETRN.at("d"), 0.258)));
            else
                return exp(6.51*exp(-pow(tLab/HZETRN.at("e"), 0.7)));
        } else if(pLab <= 4.7_GeV) {
            return JWN.at("gamma")/pow(pLab, 0.16);
        } else {
            double ecm2 = 2*Constant::mN*(Constant::mN+sqrt(pow(pLab, 2) + pow(Constant::mN, 2)));
            return PDG.at("Zpp") + PDG.at("B")*pow(log(ecm2/PDG.at("s0")), 2)
                + PDG.at("Y1pp")*pow(PDG.at("s1")/ecm2, PDG.at("n1"))
                - PDG.at("Y2pp")*pow(PDG.at("s1")/ecm2, PDG.at("n2"));
        }
    } else {
        if(pLab < 0.5_GeV) {
            if(tLab >= 0.1_MeV)
                return 38.0 + 12500.0*exp(-HZETRN.at("f")*pow(tLab-HZETRN.at("g"), 0.35));
            else
                return 26000 * exp(-pow(tLab/HZETRN.at("h"), 0.3));
        } else if(pLab <= 2.0_GeV) {
            return 40 + 10*cos(JWN.at("alpha")*pLab - 0.943)
                * exp(-JWN.at("beta")*pow(pLab, 0.8)+2);
        } else {
            double ecm2 = 2*Constant::mN*(Constant::mN+sqrt(pow(pLab, 2) + pow(Constant::mN, 2)));
            return PDG.at("Zpn") + PDG.at("B")*pow(log(ecm2/PDG.at("s0")), 2)
                + PDG.at("Y1pn")*pow(PDG.at("s1")/ecm2, PDG.at("n1"))
                - PDG.at("Y2pn")*pow(PDG.at("s1")/ecm2, PDG.at("n2"));
        }
    }
}

double IntranuclearCascade::CrossSectionAngle(bool, const double&, const double& ran) {
    // For testing right now
    // TODO: Implement GEANT4 angular distribution or something else
    return std::acos(2*ran - 1);
}

ThreeVector IntranuclearCascade::MakeMomentumAngular(bool samePID, const double& p1CM, const double& pcm,
        const std::array<double, 2>& rans) {
    double pR = p1CM;
    double pTheta = IntranuclearCascade::CrossSectionAngle(samePID, pcm, rans[0]);
    double pPhi = 2*M_PI*rans[1];

    return ThreeVector(ToCartesian({pR, pTheta, pPhi}));
}

double Interactions::CrossSectionLab(bool samePID, const double& pLab) const noexcept {
    const double tLab = sqrt(pow(pLab, 2) + pow(Constant::mN, 2)) - Constant::mN;
    if(samePID) {
        if(pLab < 1.8_GeV) {
            if(tLab >= 25_MeV)
                return (1.0+HZETRN.at("a")/tLab) * (40+109.0*std::cos(HZETRN.at("b")*sqrt(tLab))
                        * exp(-HZETRN.at("c")*pow(tLab-HZETRN.at("d"), 0.258)));
            else
                return exp(6.51*exp(-pow(tLab/HZETRN.at("e"), 0.7)));
        } else if(pLab <= 4.7_GeV) {
            return JWN.at("gamma")/pow(pLab, 0.16);
        } else {
            double ecm2 = 2*Constant::mN*(Constant::mN+sqrt(pow(pLab, 2) + pow(Constant::mN, 2)));
            return PDG.at("Zpp") + PDG.at("B")*pow(log(ecm2/PDG.at("s0")), 2)
                + PDG.at("Y1pp")*pow(PDG.at("s1")/ecm2, PDG.at("n1"))
                - PDG.at("Y2pp")*pow(PDG.at("s1")/ecm2, PDG.at("n2"));
        }
    } else {
        if(pLab < 0.5_GeV) {
            if(tLab >= 0.1_MeV)
                return 38.0 + 12500.0*exp(-HZETRN.at("f")*pow(tLab-HZETRN.at("g"), 0.35));
            else
                return 26000 * exp(-pow(tLab/HZETRN.at("h"), 0.3));
        } else if(pLab <= 2.0_GeV) {
            return 40 + 10*cos(JWN.at("alpha")*pLab - 0.943)
                * exp(-JWN.at("beta")*pow(pLab, 0.8)+2);
        } else {
            double ecm2 = 2*Constant::mN*(Constant::mN+sqrt(pow(pLab, 2) + pow(Constant::mN, 2)));
            return PDG.at("Zpn") + PDG.at("B")*pow(log(ecm2/PDG.at("s0")), 2)
                + PDG.at("Y1pn")*pow(PDG.at("s1")/ecm2, PDG.at("n1"))
                - PDG.at("Y2pn")*pow(PDG.at("s1")/ecm2, PDG.at("n2"));
        }
    }
}

GeantInteractions::GeantInteractions(const std::string& filename) {
    // Initialize theta vector
    constexpr double thetaMin = 0.5;
    constexpr double thetaMax = 179.5;
    constexpr size_t nTheta = 180;
    m_theta = Linspace(thetaMin, thetaMax, nTheta);

    constexpr double cdfMin = -3;
    constexpr double cdfMax = 0;
    constexpr size_t nCDF = 200;
    m_cdf = Logspace(cdfMin, cdfMax, nCDF);

    // Read in the Geant4 hdf5 file and get the np and pp groups
    spdlog::info("GeantInteractions: Loading Geant4 data from {0}.", filename);
    H5File file(filename, H5F_ACC_RDONLY);
    Group dataNP(file.openGroup("np")); 
    Group dataPP(file.openGroup("pp"));

    // Get the datasets for np and load into local variables
    LoadData(false, dataNP);

    // Get the datasets for pp and load into local variables
    LoadData(true, dataPP);
}

void GeantInteractions::LoadData(bool samePID, const Group& group) {
    // Load datasets
    DataSet pcm(group.openDataSet("pcm"));
    DataSet sigTot(group.openDataSet("sigtot"));
    DataSet sig(group.openDataSet("sig"));
  
    // Get data for center of momentum
    DataSpace pcmSpace = pcm.getSpace();
    std::array<hsize_t, 1> dimPcm{};
    pcmSpace.getSimpleExtentDims(dimPcm.data(), nullptr);
    std::vector<double> pcmVec(dimPcm[0]);
    pcm.read(pcmVec.data(), PredType::NATIVE_DOUBLE, pcmSpace, pcmSpace);

    // Get data for total cross-section
    DataSpace sigTotSpace = sigTot.getSpace();
    std::array<hsize_t, 1> dimSigTot{};
    sigTotSpace.getSimpleExtentDims(dimSigTot.data(), nullptr);
    std::vector<double> sigTotVec(dimSigTot[0]);
    sigTot.read(sigTotVec.data(), PredType::NATIVE_DOUBLE, sigTotSpace, sigTotSpace);

    // Get data for angular cross-section
    DataSpace sigSpace = sig.getSpace();
    std::array<hsize_t, 2> dimSig{};
    sigSpace.getSimpleExtentDims(dimSig.data(), nullptr);
    hsize_t dims = dimSig[0] * dimSig[1];
    std::vector<double> sigAngular(dims);
    sig.read(sigAngular.data(), PredType::NATIVE_DOUBLE, sigSpace, sigSpace);

    // Perform interpolation for angles
    IntranuclearCascade::Interp2D interp;
    interp.BicubicSpline(pcmVec, m_theta, sigAngular);

    std::vector<double> theta(pcmVec.size()*m_cdf.size());
    constexpr double accuracy = 1E-6;
    for(size_t i = 0; i < pcmVec.size(); ++i) {
        for(size_t j = 0; j < m_cdf.size(); ++j) {
            auto func = [interp, pcmVec, this, i, j](double x){
                return interp(pcmVec[i], x) - this -> m_cdf[j];
            };
            IntranuclearCascade::Brent brent(func, accuracy);
            if(j != m_cdf.size() - 1)
                try{
                    theta[i*m_cdf.size() + j] = brent.CalcRoot(m_theta.front(), m_theta.back());
                } catch (std::domain_error &e) {
                    theta[i*m_cdf.size() + j] = *m_theta.begin()/sigAngular[i*180 + j]*m_cdf[j];
                }
            else
                theta[i*m_cdf.size() + j] = *m_theta.end();
        }
    }

    if(samePID) {
        m_pcmPP = pcmVec;
        m_xsecPP = sigTotVec;
        m_crossSectionPP.CubicSpline(pcmVec, sigTotVec);
        m_thetaDistPP.BicubicSpline(pcmVec, m_cdf, theta);
    } else {
        m_pcmNP = pcmVec;
        m_xsecNP = sigTotVec;
        m_crossSectionNP.CubicSpline(pcmVec, sigTotVec);
        m_thetaDistNP.BicubicSpline(pcmVec, m_cdf, theta);
    }
}

double GeantInteractions::CrossSection(const Particle& particle1,
                                       const Particle& particle2) const {
    bool samePID = particle1.ID() == particle2.ID();
    ThreeVector boostCM = (particle1.Momentum() + particle2.Momentum()).BoostVector();
    FourVector p1Lab = particle1.Momentum(), p2Lab = particle2.Momentum();
    FourVector p1CM = p1Lab.Boost(-boostCM);
    // Generate outgoing momentum
    const double pcm = p1CM.Vec3().Magnitude();

    try {
        if(samePID) {
            return m_crossSectionPP(pcm/1_GeV);
        } else {
            return m_crossSectionNP(pcm/1_GeV);
        }
    } catch (std::domain_error &e) {
        spdlog::debug("Using Nasa Interaction");
        double s = (p1Lab+p2Lab).M2();
        double plab = sqrt(pow(s, 2)/(4*pow(Constant::mN, 2)) - s);
        return Interactions::CrossSectionLab(samePID, plab);
    }
}

ThreeVector GeantInteractions::MakeMomentum(bool samePID,
                                            const double& pcm,
                                            const std::array<double, 2>& rans) const {
    double pR = pcm;
    double pTheta = CrossSectionAngle(samePID, pcm/1_GeV, rans[0]);
    double pPhi = 2*M_PI*rans[1];

    return ThreeVector(ToCartesian({pR, pTheta, pPhi}));
}

double GeantInteractions::CrossSectionAngle(bool samePID, const double& energy,
                                            const double& ran) const {
    try{
        if(samePID) return m_thetaDistPP(energy, ran);
        else return m_thetaDistNP(energy, ran);
    } catch(std::domain_error &e) {
        spdlog::debug("Using flat angular distribution");
        return acos(2*ran-1);
    }
}

double NasaInteractions::CrossSection(const Particle& particle1,
                                       const Particle& particle2) const {
    bool samePID = particle1.ID() == particle2.ID();
    FourVector p1Lab = particle1.Momentum(), p2Lab = particle2.Momentum();
    // Generate outgoing momentum
    double s = (p1Lab+p2Lab).M2();
    double plab = sqrt(pow(s,2)/(4.0*pow(Constant::mN, 2))-s);
    return CrossSectionLab(samePID,plab); 
}

ThreeVector NasaInteractions::MakeMomentum(bool, const double& pcm,
                                           const std::array<double, 2>& rans) const {
    double pR = pcm;
    double pTheta = acos(2*rans[0]-1);
    double pPhi = 2*M_PI*rans[1];

    return ThreeVector(ToCartesian({pR, pTheta, pPhi}));
}
