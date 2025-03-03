CMAKE ?= cmake
ifneq (, $(shell which ninja))
CMAKE_ARGS += -GNinja
endif
# Zip distribution
ADDONS_DIR = addons/lua-gdextension
ADDONS_SRC = $(shell find $(ADDONS_DIR) -type f)
COPIED_FILES = LICENSE
# Testing
GODOT_BIN ?= godot
# Download releases
GITHUB_CLI_BIN ?= gh
GITHUB_REPO ?= gilzoide/lua-gdextension
RUN_ID ?=
# Update version
define UPDATE_CHANGELOG_VERSION_SED_SCRIPT
/Unreleased/ {
  s/Unreleased/$(VERSION)/
  s/compare[^)]*/releases\/tag\/$(VERSION)/
  i \#\# [Unreleased](https://github.com/gilzoide/lua-gdextension/compare/$(VERSION)...HEAD)
  i
  i
}
endef

define UPDATE_README_VERSION_SED_SCRIPT
/logo=godotengine/ {
  s/message=[^)]*/message=$(VERSION)/
}
endef

$(ADDONS_DIR)/%: %
	mkdir -p $(@D)
	cp $< $@

build:
	mkdir -p $@

# Build libraries using CMake
build/%/debug/libluagdextension.dll: cmake/toolchains/%.cmake
	$(CMAKE) . -B $(@D) --toolchain $(abspath $<) $(CMAKE_ARGS) -DCMAKE_BUILD_TYPE=Debug
	$(CMAKE) --build $(@D)
build/%/release/libluagdextension.dll: cmake/toolchains/%.cmake
	$(CMAKE) . -B $(@D) --toolchain $(abspath $<) $(CMAKE_ARGS) -DCMAKE_BUILD_TYPE=Release
	$(CMAKE) --build $(@D)

build/%/debug/libluagdextension.so: cmake/toolchains/%.cmake
	$(CMAKE) . -B $(@D) --toolchain $(abspath $<) $(CMAKE_ARGS) -DCMAKE_BUILD_TYPE=Debug
	$(CMAKE) --build $(@D)
build/%/release/libluagdextension.so: cmake/toolchains/%.cmake
	$(CMAKE) . -B $(@D) --toolchain $(abspath $<) $(CMAKE_ARGS) -DCMAKE_BUILD_TYPE=Release
	$(CMAKE) --build $(@D)

build/%/debug/libluagdextension.dylib: cmake/toolchains/%.cmake
	$(CMAKE) . -B $(@D) --toolchain $(abspath $<) $(CMAKE_ARGS) -DCMAKE_BUILD_TYPE=Debug
	$(CMAKE) --build $(@D)
build/%/release/libluagdextension.dylib: cmake/toolchains/%.cmake
	$(CMAKE) . -B $(@D) --toolchain $(abspath $<) $(CMAKE_ARGS) -DCMAKE_BUILD_TYPE=Release
	$(CMAKE) --build $(@D)

windows: \
	addons/lua-gdextension/build/windows/x86_64/debug/libluagdextension.dll \
	addons/lua-gdextension/build/windows/x86_64/release/libluagdextension.dll \
	addons/lua-gdextension/build/windows/x86_32/debug/libluagdextension.dll \
	addons/lua-gdextension/build/windows/x86_32/release/libluagdextension.dll
linux: \
	addons/lua-gdextension/build/linux/x86_64/debug/libluagdextension.so \
	addons/lua-gdextension/build/linux/x86_64/release/libluagdextension.so \
	addons/lua-gdextension/build/linux/x86_32/debug/libluagdextension.so \
	addons/lua-gdextension/build/linux/x86_32/release/libluagdextension.so
macos: \
	addons/lua-gdextension/build/macos/debug/libluagdextension.dylib \
	addons/lua-gdextension/build/macos/release/libluagdextension.dylib
android: \
	addons/lua-gdextension/build/android/x86_64/debug/libluagdextension.so \
	addons/lua-gdextension/build/android/x86_64/release/libluagdextension.so \
	addons/lua-gdextension/build/android/x86_32/debug/libluagdextension.so \
	addons/lua-gdextension/build/android/x86_32/release/libluagdextension.so \
	addons/lua-gdextension/build/android/arm32/debug/libluagdextension.so \
	addons/lua-gdextension/build/android/arm32/release/libluagdextension.so \
	addons/lua-gdextension/build/android/arm64/debug/libluagdextension.so \
	addons/lua-gdextension/build/android/arm64/release/libluagdextension.so
ios: \
	addons/lua-gdextension/build/ios/debug/libluagdextension.dylib \
	addons/lua-gdextension/build/ios/release/libluagdextension.dylib

# Miscelaneous
build/lua-gdextension.zip: $(ADDONS_SRC) $(addprefix $(ADDONS_DIR)/,$(COPIED_FILES)) | build
	zip $@ $^

build/update_changelog_version.sed:
	@if [[ -z "$(VERSION)" ]]; then echo "Error: please set the VERSION variable"; false; fi
	$(file > $@,$(UPDATE_CHANGELOG_VERSION_SED_SCRIPT))

build/update_readme_version.sed:
	@if [[ -z "$(VERSION)" ]]; then echo "Error: please set the VERSION variable"; false; fi
	$(file > $@,$(UPDATE_README_VERSION_SED_SCRIPT))


test/.godot:
	$(GODOT_BIN) --headless --quit --path test --editor || true


.PHONY: zip test download-latest-build bump-version build/update_changelog_version.sed
zip: build/lua-gdextension.zip

test: test/.godot
	$(GODOT_BIN) --headless --quit --path test --script test_entrypoint.gd

download-latest-build:
	$(eval TMPDIR := $(shell mktemp -d))
	$(GITHUB_CLI_BIN) run download $(RUN_ID) --repo $(GITHUB_REPO) -n lua-gdextension -D $(TMPDIR)
	cp $(TMPDIR)/addons/lua-gdextension/build/libluagdextension* addons/lua-gdextension/build
	$(RM) -r $(TMPDIR)

bump-version: build/update_changelog_version.sed build/update_readme_version.sed
	sed -i CHANGELOG.md -f build/update_changelog_version.sed
	sed -i README.md -f build/update_readme_version.sed
