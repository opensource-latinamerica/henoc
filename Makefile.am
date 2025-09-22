ACLOCAL_AMFLAGS = -I m4

# Variables substituted by configure
QMAKE = @QMAKE@
ODE_PREFIX = @ODE_PREFIX@
ASSET_SIZES = @ASSET_SIZES@

.PHONY: all-local build-src clean-local distclean-local check-deps assets

# Default build calls into src with configured variables
all-local: build-src

build-src:
	$(AM_V_at)$(MAKE) -C src QMAKE="$(QMAKE)" ODE_INSTALL_DIR_ABS="$(ODE_PREFIX)"

clean-local:
	-$(MAKE) -C src clean || true

distclean-local:
	-$(MAKE) -C src clean || true

# Branding assets
if ASSETS

SVG_SRC = $(wildcard branding/*.svg)
PNG_DIR = branding/png

# Expand list of output PNGs for all sizes
PNGS = $(foreach s,$(ASSET_SIZES),$(addprefix $(PNG_DIR)/,$(addsuffix -$(s).png,$(basename $(notdir $(SVG_SRC))))))

assets: $(PNGS)

# Rules per size to map name.svg -> name-SIZE.png
$(PNG_DIR)/%-16.png: branding/%.svg
	$(AM_V_at)$(MKDIR_P) $(PNG_DIR)
	$(AM_V_at)size=16 ; $(ASSET_CONVERT)

$(PNG_DIR)/%-32.png: branding/%.svg
	$(AM_V_at)$(MKDIR_P) $(PNG_DIR)
	$(AM_V_at)size=32 ; $(ASSET_CONVERT)

$(PNG_DIR)/%-64.png: branding/%.svg
	$(AM_V_at)$(MKDIR_P) $(PNG_DIR)
	$(AM_V_at)size=64 ; $(ASSET_CONVERT)

$(PNG_DIR)/%-128.png: branding/%.svg
	$(AM_V_at)$(MKDIR_P) $(PNG_DIR)
	$(AM_V_at)size=128 ; $(ASSET_CONVERT)

$(PNG_DIR)/%-256.png: branding/%.svg
	$(AM_V_at)$(MKDIR_P) $(PNG_DIR)
	$(AM_V_at)size=256 ; $(ASSET_CONVERT)

else

# When assets are not enabled, prevent building them via make
assets:
	$(AM_V_at)echo "Assets are disabled. Re-run ./configure --enable-assets or run branding/export.sh manually." >&2; exit 1

endif

EXTRA_DIST = \
	README.md \
	src \
	branding \
	LICENSE
