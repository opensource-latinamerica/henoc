## Top-level Makefile for the Henoc project

# Allow overriding qmake from environment/command line
QMAKE ?= qmake

# Subdirectories
HENOC_UNIVERSE_DIR = HenocUniverse
FULL_HENOC_DIR = FullHenoc
ODE_DIR = $(HENOC_UNIVERSE_DIR)/ode
ODE_INSTALL_DIR_ABS = $(abspath $(HENOC_UNIVERSE_DIR)/ode_install)

# Phony targets
.PHONY: all ode henochuniverse henoc clean

# Default target
all: henoc

# Target to build the FullHenoc application

henoc: henochuniverse
	@echo "--- Building henoc application ---"
	@cd $(FULL_HENOC_DIR) && $(QMAKE) Henoc.pro && $(MAKE)

# Target to build the HenocUniverse library
henochuniverse: ode
	@echo "--- Building HenocUniverse library ---"
	@$(MAKE) -C $(HENOC_UNIVERSE_DIR)

# Target to build the ODE library if it's not already built
ode:
	@if [ ! -f "$(HENOC_UNIVERSE_DIR)/ode_install/lib/libode.a" ]; then \
		echo "--- ODE library not found, building it now. ---"; \
		cd $(ODE_DIR) && \
		./bootstrap && \
		./configure --prefix=$(ODE_INSTALL_DIR_ABS) --enable-double-precision && \
		$(MAKE) && \
		$(MAKE) install; \
	else \
		echo "--- ODE library found. ---"; \
	fi

# Target to clean the project
clean:
	@echo "--- Cleaning project ---"
	@$(MAKE) -C $(HENOC_UNIVERSE_DIR) clean
	@if [ -f "$(FULL_HENOC_DIR)/Makefile" ]; then \
		$(MAKE) -C $(FULL_HENOC_DIR) clean; \
	fi
	@if [ -f "$(ODE_DIR)/Makefile" ]; then \
		$(MAKE) -C $(ODE_DIR) distclean; \
	fi
	@rm -rf $(HENOC_UNIVERSE_DIR)/ode_install
	@rm -f henoc
