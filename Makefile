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

$(ADDONS_DIR)/%: %
	cp $< $@

build:
	mkdir -p $@

build/lua-gdextension.zip: $(ADDONS_SRC) $(addprefix $(ADDONS_DIR)/,$(COPIED_FILES)) | build
	zip $@ $^

build/update_changelog_version.sed:
	@if [[ -z "$(VERSION)" ]]; then echo "Error: please set the VERSION variable"; false; fi
	$(file > $@,$(UPDATE_CHANGELOG_VERSION_SED_SCRIPT))


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

bump-version: build/update_changelog_version.sed
	sed -i CHANGELOG.md -f $<
