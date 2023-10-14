ADDONS_DIR = addons/lua-gdextension
ADDONS_SRC = $(shell find $(ADDONS_DIR) -type f)
COPIED_FILES = LICENSE
GODOT_BIN ?= godot

$(ADDONS_DIR)/%: %
	cp $< $@

build:
	mkdir -p $@

build/lua-gdextension.zip: $(ADDONS_SRC) $(addprefix $(ADDONS_DIR)/,$(COPIED_FILES)) | build
	zip $@ $^

test/.godot:
	$(GODOT_BIN) --headless --quit --path test --editor || true


.PHONY: zip test
zip: build/lua-gdextension.zip

test: test/.godot
	$(GODOT_BIN) --headless --quit --path test --script test_entrypoint.gd

