from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class Project(ConanFile):
    name            = "magpie"
    description     = "Conan package for the magpie engine."
    version         = "1.0.0"                
    url             = "https://github.com/acdemiralp/magpie"
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    requires        = (("acd/1.2.0@acdemiralp/makina"          ),
                       ("boost/1.69.0@conan/stable"            ),
                       ("catch2/2.2.0@bincrafters/stable"      ),
                       ("di/1.4.0@acdemiralp/makina"           ),
                       ("ec/1.3.0@acdemiralp/makina"           ),
                       ("fg/1.1.0@acdemiralp/makina"           ),
                       ("gl/1.1.2@acdemiralp/makina"           ),
                       ("glm/0.9.8.5@g-truc/stable"            ),
                       ("im3d/master@acdemiralp/makina"        ),
                       ("imgui/1.53@acdemiralp/makina"         ),
                       ("jsonformoderncpp/3.1.1@vthiery/stable"),
                       ("spdlog/1.3.1@bincrafters/stable"      ),
                       ("stb/20180214@conan/stable"            ),
                       ("zlib/1.2.8@conan/stable"              )) 
    options         = {"shared": [True, False]} 
    default_options = "shared=True"

    def configure(self):
        self.options["boost"      ].without_test = True
        self.options["gl"         ].shared       = False
        self.options["websocketpp"].asio         = "standalone"

    def imports(self):
        self.copy("*.dylib*", dst="", src="lib")
        self.copy("*.dll"   , dst="", src="bin")

    def source(self):
        zip_name = "%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else "-DBUILD_SHARED_LIBS=OFF"
        self.run("cmake %s-%s %s %s" % (self.name, self.version, cmake.command_line, shared_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/include" % (self.name, self.version)
        self.copy("*.h"     , dst="include", src=include_folder)
        self.copy("*.hpp"   , dst="include", src=include_folder)
        self.copy("*.inl"   , dst="include", src=include_folder)
        self.copy("*.dylib*", dst="lib"    , keep_path=False   )
        self.copy("*.lib"   , dst="lib"    , keep_path=False   )
        self.copy("*.so*"   , dst="lib"    , keep_path=False   )
        self.copy("*.dll"   , dst="bin"    , keep_path=False   )

    def package_info(self):
        self.cpp_info.libs = [self.name]
        if self.settings.os == "Windows":
            if not self.options.shared:
                self.cpp_info.defines.append("%s_STATIC" % self.name.upper())
