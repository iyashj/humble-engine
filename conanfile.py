# This file is managed by Conan, contents will be overwritten.
# To keep your changes, remove these comment lines, but the plugin won't be able to modify your requirements

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, cmake_layout

class ConanApplication(ConanFile):
    name = "humble_engine"
    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"
    options = {"build_tests": [True, False]}
    default_options = {"build_tests": False}
    generators = "CMakeDeps"

    def layout(self):
        # Flat layout: generators, build, and source all in build/Debug (or build/Release)
        from pathlib import Path
        build_folder = f"build/{self.settings.build_type}"
        self.folders.build = build_folder
        self.folders.generators = build_folder
        self.folders.source = "."

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.generate()

    def requirements(self):
        requirements = self.conan_data.get('requirements', [])
        for requirement in requirements:
            self.requires(requirement)
    
    def build_requirements(self):
        if self.options.build_tests:
            for requirement in self.conan_data.get('test_requirements', []):
                self.requires(requirement)