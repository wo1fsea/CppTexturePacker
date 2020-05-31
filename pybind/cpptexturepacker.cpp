#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "cpp_texture_packer.h"

namespace py = pybind11;
using namespace CppTexturePacker;

PYBIND11_MODULE(cpptexturepacker_pybind, m)
{
    m.doc() = "cpptexturepacker";

    py::class_<TexturePacker>(m, "TexturePacker")
        .def(py::init<unsigned int,
                      unsigned int,
                      bool,
                      bool,
                      int,
                      int,
                      int,
                      bool,
                      unsigned char,
                      unsigned char>(),
             py::arg("max_width") = 4096,
             py::arg("max_height") = 4096,
             py::arg("enable_rotated") = false,
             py::arg("force_square") = false,
             py::arg("border_padding") = 0,
             py::arg("shape_padding") = 0,
             py::arg("inner_padding") = 0,
             py::arg("reduce_border_artifacts") = false,
             py::arg("trim_mode") = 0,
             py::arg("extrude") = 0)
        .def("pack",
             py::overload_cast<const std::string &,
                               const std::string &,
                               const std::string &,
                               const std::string &,
                               const std::string &>(&TexturePacker::pack),
             "pack",
             py::arg("images_dir"),
             py::arg("output_name"),
             py::arg("output_path"),
             py::arg("base_image_path") = "",
             py::arg("image_format") = "png")
        .def("pack",
             py::overload_cast<const std::vector<std::string> &,
                               const std::string &,
                               const std::string &,
                               const std::string &,
                               const std::string &>(&TexturePacker::pack),
             "pack",
             py::arg("images_paths"),
             py::arg("output_name"),
             py::arg("output_path"),
             py::arg("base_image_path") = "",
             py::arg("image_format") = "png");
}
