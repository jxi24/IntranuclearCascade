#include "IntranuclearCascade/PyBindings.hh"
#include "IntranuclearCascade/Cascade.hh"
#include "IntranuclearCascade/Interactions.hh"
#include "IntranuclearCascade/Nucleus.hh"
#include "IntranuclearCascade/Particle.hh"
#include "IntranuclearCascade/ThreeVector.hh"

// These are for convenience
using IntranuclearCascade::Cascade; 
using IntranuclearCascade::Interactions;

void CascadeModule(py::module &m) {
    constexpr size_t maxSteps = 10000;

    py::class_<Cascade, std::shared_ptr<Cascade>> cascade(m, "Cascade");

        // Constructors
        cascade.def(py::init<const std::shared_ptr<Interactions>, const Cascade::ProbabilityType&, 
                    const double&>(),
                    py::arg("interactions"), py::arg("prob"), py::arg("distance") = 0.03)
        // Functions
        .def("kick", &Cascade::Kick)
        .def("reset", &Cascade::Reset)
        .def("set_kicked", &Cascade::SetKicked)
        .def("evolve", &Cascade::Evolve,
             py::arg("nucleus"), py::arg("max_steps") = maxSteps)
        .def("nuwro", &Cascade::NuWro,
             py::arg("nucleus"), py::arg("max_steps") = maxSteps)
        .def("mean_free_path", &Cascade::MeanFreePath,
             py::arg("nucleus"), py::arg("max_steps") = maxSteps)
        .def("mean_free_path_nuwro", &Cascade::MeanFreePath_NuWro,
             py::arg("nucleus"), py::arg("max_steps") = maxSteps);

    py::enum_<Cascade::ProbabilityType>(cascade, "Probability")
        .value("Gaussian", Cascade::ProbabilityType::Gaussian)
        .value("Pion", Cascade::ProbabilityType::Pion)
        .value("Cylinder", Cascade::ProbabilityType::Cylinder)
        .export_values();
}
