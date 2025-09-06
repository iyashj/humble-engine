from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout

class HumbleEngineConan(ConanFile):
    name = "humble-engine"
    version = "1.0.0"
    package_type = "library"
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_tests": [True, False],
        "build_examples": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_tests": False,
        "build_examples": False
    }

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("raylib/5.5")

    def build_requirements(self):
        if self.options.build_tests:
            self.requires("catch2/3.5.2")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        
        tc = CMakeToolchain(self)
        tc.variables["HUMBLEENGINE_BUILD_SHARED"] = self.options.shared
        tc.variables["HUMBLEENGINE_BUILD_TESTS"] = self.options.build_tests
        tc.variables["HUMBLEENGINE_BUILD_EXAMPLES"] = self.options.build_examples
        # Don't generate user presets to avoid conflicts
        tc.user_presets_path = False
        tc.generate()
