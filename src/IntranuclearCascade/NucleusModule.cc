#include "IntranuclearCascade/PyBindings.hh"
#include "IntranuclearCascade/FourVector.hh"
#include "IntranuclearCascade/Nucleus.hh"
#include "IntranuclearCascade/Particle.hh"
#include "IntranuclearCascade/ThreeVector.hh"

void NucleusModule(py::module &m) {
    py::class_<IntranuclearCascade::Nucleus, std::shared_ptr<IntranuclearCascade::Nucleus>> nucleus(m, "Nucleus", py::module_local());
    // Constructors
    nucleus.def(py::init<const std::size_t&, const std::size_t&, const double&, const double&,
                const std::string&, const IntranuclearCascade::Nucleus::FermiGasType&,
                const std::function<IntranuclearCascade::Particles()>&>(),
                py::arg("Z"), py::arg("A"), py::arg("binding"), py::arg("kf"),
                py::arg("density_file"),
                py::arg("fg_type"),
                py::arg("density") = std::function<IntranuclearCascade::Particles()>())
        // Setters
        .def("set_nucleons", &IntranuclearCascade::Nucleus::SetNucleons)
        .def("set_binding_energy", &IntranuclearCascade::Nucleus::SetBindingEnergy)
        .def("set_fermi_momentum", &IntranuclearCascade::Nucleus::SetFermiMomentum)
        .def("set_potential", &IntranuclearCascade::Nucleus::SetPotential)
        .def("set_density", &IntranuclearCascade::Nucleus::SetDensity)
        .def("set_radius", &IntranuclearCascade::Nucleus::SetRadius)
        // Getters
        .def("nucleons", &IntranuclearCascade::Nucleus::Nucleons)
        .def("protons", &IntranuclearCascade::Nucleus::Protons)
        .def("neutrons", &IntranuclearCascade::Nucleus::Neutrons)
        .def("n_nucleons", &IntranuclearCascade::Nucleus::NNucleons)
        .def("n_protons", &IntranuclearCascade::Nucleus::NProtons)
        .def("n_neutrons", &IntranuclearCascade::Nucleus::NNeutrons)
        .def("binding_energy", &IntranuclearCascade::Nucleus::BindingEnergy)
        .def("fermi_momentum", &IntranuclearCascade::Nucleus::FermiMomentum,
             py::arg("position") = 0.0)
        .def("potential_energy", &IntranuclearCascade::Nucleus::PotentialEnergy)
        .def("radius", &IntranuclearCascade::Nucleus::Radius)
        // Functions
        .def("escape", &IntranuclearCascade::Nucleus::Escape)
        .def("generate_config", &IntranuclearCascade::Nucleus::GenerateConfig)
        .def("generate_momentum", &IntranuclearCascade::Nucleus::GenerateMomentum,
             py::arg("position") = 0.0)
        // String Methods
        .def("__str__", &IntranuclearCascade::Nucleus::ToString)
        .def("__repr__", &IntranuclearCascade::Nucleus::ToString)
        // Static Methods
        .def_static("make_nucleus", &IntranuclearCascade::Nucleus::MakeNucleus);

    py::enum_<IntranuclearCascade::Nucleus::FermiGasType>(nucleus, "FermiGas", py::module_local())
        .value("Local", IntranuclearCascade::Nucleus::FermiGasType::Local)
        .value("Global", IntranuclearCascade::Nucleus::FermiGasType::Global)
        .export_values();
}

