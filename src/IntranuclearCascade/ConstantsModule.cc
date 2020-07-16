#include "IntranuclearCascade/PyBindings.hh"

#include "IntranuclearCascade/Constants.hh"

namespace Constant = IntranuclearCascade::Constant;

void ConstantsModule(py::module &m) {
    py::module mconstant = m.def_submodule("constants", "IntranuclearCascade constants");
    mconstant.attr("c") = py::float_(Constant::C);
    mconstant.attr("hbarc") = py::float_(Constant::HBARC);
    mconstant.attr("hbarc2") = py::float_(Constant::HBARC2);
    mconstant.attr("mp") = py::float_(Constant::mp);
    mconstant.attr("mn") = py::float_(Constant::mn);
    mconstant.attr("mN") = py::float_(Constant::mN);
}
