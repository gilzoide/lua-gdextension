BUILDDIR ?= .
MAKE_LUAJIT_ARGS += TARGET_FLAGS="$(CCFLAGS)" TARGET_LDFLAGS="$(LINKFLAGS)" XCFLAGS=-DLUAJIT_ENABLE_LUA52COMPAT

.PRECIOUS: %/luajit

%/luajit:
	@mkdir -p $*
	cp -r luajit $@

%/libluajit.a %/lua51.dll: %/luajit
	$(MAKE) -C $< amalg TARGET_SYS=$(TARGET_SYS) $(MAKE_LUAJIT_ARGS)
	cp $</src/$(@F) $*

luajit-native:
	$(MAKE) -C luajit/src luajit.h jit/vmdef.lua

luajit-windows: TARGET_SYS = Windows
luajit-windows: $(BUILDDIR)/libluajit.a $(BUILDDIR)/lua51.dll

luajit-macos: TARGET_SYS = Darwin
luajit-macos: $(BUILDDIR)/libluajit.a

luajit-ios: TARGET_SYS = iOS
luajit-ios: $(BUILDDIR)/libluajit.a

luajit-linux: TARGET_SYS = Linux
luajit-linux: $(BUILDDIR)/libluajit.a

luajit-android: TARGET_SYS = Linux
luajit-android: $(BUILDDIR)/libluajit.a
