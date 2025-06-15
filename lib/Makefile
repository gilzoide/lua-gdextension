BUILDDIR ?= .
MAKE_LUAJIT_ARGS += \
	TARGET_SYS='$(TARGET_SYS)' \
	TARGET_FLAGS='$(CCFLAGS)' \
	TARGET_LDFLAGS='$(LINKFLAGS)' \
	STATIC_CC='$(STATIC_CC)' \
	DYNAMIC_CC='$(DYNAMIC_CC)' \
	TARGET_LD='$(TARGET_LD)' \
	XCFLAGS=-DLUAJIT_ENABLE_LUA52COMPAT
ifneq ($(HOST_CC),)
MAKE_LUAJIT_ARGS += HOST_CC='$(HOST_CC)'
endif 
ifneq ($(TARGET_STRIP),)
MAKE_LUAJIT_ARGS += TARGET_STRIP='$(TARGET_STRIP)'
endif 

.PRECIOUS: %/luajit

%/luajit:
	@mkdir -p '$*'
	cp -r luajit '$@'
	$(MAKE) -C '$@' clean

%/libluajit.a %/lua51.dll: %/luajit
	$(MAKE) -C '$<' amalg $(MAKE_LUAJIT_ARGS)
	cp '$</src/$(@F)' '$*'

luajit-native:
	$(MAKE) -C luajit/src luajit.h jit/vmdef.lua

luajit-windows: TARGET_SYS = Windows
luajit-windows: $(BUILDDIR)/libluajit.a $(BUILDDIR)/lua51.dll

luajit-windows-msvc: TARGET_SYS = Windows
luajit-windows-msvc: $(BUILDDIR)/luajit
	cd "$</src" && cmd /c msvcbuild.bat

luajit-macos: TARGET_SYS = Darwin
luajit-macos: $(BUILDDIR)/libluajit.a

luajit-ios: TARGET_SYS = iOS
luajit-ios: $(BUILDDIR)/libluajit.a

luajit-linux: TARGET_SYS = Linux
luajit-linux: $(BUILDDIR)/libluajit.a

luajit-android: TARGET_SYS = Linux
luajit-android: $(BUILDDIR)/libluajit.a
