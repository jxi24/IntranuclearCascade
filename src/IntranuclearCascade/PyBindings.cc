#include "IntranuclearCascade/PyBindings.hh"

PYBIND11_MODULE(_intranuclear_cascade, m) {
    // Utilities
    py::module utilities = m.def_submodule("utilities", "Intranuclear Cascade utilities");
    LoggingModule(utilities);
    ConstantsModule(utilities);
    InterpolationModule(utilities);

    // Physics Objects
    py::module physics = m.def_submodule("physics", "Intranuclear Cascade physics objects");
    VectorModule(physics);
    ParticleInfoModule(physics);
    ParticleModule(physics);
    NucleusModule(physics);

    // Calculation Objects
    InteractionsModule(m);
    CascadeModule(m);
}
