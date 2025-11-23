.PHONY: all install clean uninstall dependencies configure compile test

NAME := nfs

UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
    ifneq ($(wildcard /etc/debian_version),)
        DISTRO := debian
    else ifneq ($(wildcard /etc/arch-release),)
        DISTRO := arch
    else
        DISTRO := unknown
    endif
else
    DISTRO := unknown
endif

BIN_DIR := /usr/local/bin
SHARE_DIR := /usr/local/share/$(NAME)
CONFIG_DIR := /etc/$(NAME)

dependencies:
	@echo "Instalando dependencias básicas del sistema..."
	@if [ "$(DISTRO)" = "debian" ]; then \
		echo "Instalando compilador y herramientas..."; \
		sudo apt-get update; \
		sudo apt-get install -y build-essential cmake pkg-config git; \
		echo "Instalando Qt6..."; \
		sudo apt-get install -y qt6-base-dev qt6-multimedia-dev; \
		echo "Instalando SDL2..."; \
		sudo apt-get install -y libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev libsdl2-mixer-2.0-0 libsdl2-mixer-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev; \
		echo "Configurando archivos CMake de SDL2..."; \
		if [ ! -e "/usr/lib/x86_64-linux-gnu/cmake/SDL2_image" ]; then \
			sudo mkdir -p /usr/lib/x86_64-linux-gnu/cmake/SDL2_image; \
			sudo cp ./cmake/sdl2/sdl2_image-config.cmake /usr/lib/x86_64-linux-gnu/cmake/SDL2_image/ 2>/dev/null || true; \
		fi; \
		if [ ! -e "/usr/lib/x86_64-linux-gnu/cmake/SDL2_mixer" ]; then \
			sudo mkdir -p /usr/lib/x86_64-linux-gnu/cmake/SDL2_mixer; \
			sudo cp ./cmake/sdl2/sdl2_mixer-config.cmake /usr/lib/x86_64-linux-gnu/cmake/SDL2_mixer/ 2>/dev/null || true; \
		fi; \
		if [ ! -e "/usr/lib/x86_64-linux-gnu/cmake/SDL2_ttf" ]; then \
			sudo mkdir -p /usr/lib/x86_64-linux-gnu/cmake/SDL2_ttf; \
			sudo cp ./cmake/sdl2/sdl2_ttf-config.cmake /usr/lib/x86_64-linux-gnu/cmake/SDL2_ttf/ 2>/dev/null || true; \
		fi; \
		echo "✓ Dependencias instaladas."; \
	elif [ "$(DISTRO)" = "arch" ]; then \
		sudo pacman -Syu --noconfirm; \
		sudo pacman -S --noconfirm base-devel cmake pkg-config git; \
		sudo pacman -S --noconfirm qt6-base qt6-multimedia; \
		sudo pacman -S --noconfirm sdl2 sdl2_image sdl2_mixer sdl2_ttf; \
		echo "✓ Dependencias instaladas."; \
	else \
		echo "Distro no soportada"; \
		exit 1; \
	fi

configure:
	@echo "Configurando proyecto (descargando dependencias automáticamente)..."
	@mkdir -p build
	@cd build && cmake .. -DCMAKE_BUILD_TYPE=Release

compile: configure
	@echo "Compilando proyecto..."
	@cd build && make -j$(shell nproc)

install: dependencies compile
	@echo "Instalando proyecto en el sistema..."
	@cd build && sudo make install
	@echo "Actualizando cache de librerías..."
	@sudo ldconfig
	@echo ""
	@echo "🎉 ¡INSTALACIÓN COMPLETADA!"
	@echo ""
	@echo "PARA JUGAR:"
	@echo "  1. Abrir terminal: nfs_server 8080"
	@echo "  2. Abrir otra terminal: nfs_client 127.0.0.1 8080"
	@echo ""
	@echo "OTROS PROGRAMAS:"
	@echo "  - Editor de mapas: nfs_editor"
	@echo "  - Tests: nfs_tests"

dev: clean
	@mkdir -p build
	@cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug
	@cd build && make -j$(shell nproc)
	@echo "Compilación de desarrollo lista"

test: compile
	@echo "Ejecutando tests..."
	@./build/nfs_tests

clean:
	@echo "Limpiando..."
	@rm -rf build
	@echo "Limpieza completada"

uninstall:
	@echo "Desinstalando..."
	@sudo rm -f $(BIN_DIR)/nfs_server 2>/dev/null || true
	@sudo rm -f $(BIN_DIR)/nfs_client 2>/dev/null || true
	@sudo rm -f $(BIN_DIR)/nfs_editor 2>/dev/null || true
	@sudo rm -f $(BIN_DIR)/physics_sandbox 2>/dev/null || true
	@sudo rm -f $(BIN_DIR)/nfs_tests 2>/dev/null || true
	@sudo rm -rf $(SHARE_DIR) 2>/dev/null || true
	@sudo rm -rf $(CONFIG_DIR) 2>/dev/null || true
	@echo "Desinstalación completada"

check-install:
	@echo "=== VERIFICANDO INSTALACIÓN ==="
	@echo "Binarios:"
	@ls $(BIN_DIR)/nfs_* 2>/dev/null && echo "✓ Binarios instalados" || echo "✗ No hay binarios"
	@echo ""
	@echo "Datos:"
	@ls $(SHARE_DIR) 2>/dev/null >/dev/null && echo "✓ Datos instalados" || echo "✗ No hay datos"
	@echo "==============================="

run-server: compile
	@echo "Ejecutando servidor (desde build)..."
	@./build/nfs_server 8080

run-client: compile
	@echo "Ejecutando cliente (desde build)..."
	@./build/nfs_client 127.0.0.1 8080

run-editor: compile
	@echo "Ejecutando editor (desde build)..."
	@./build/nfs_editor

run-physics: compile
	@echo "Ejecutando physics sandbox (desde build)..."
	@./build/physics_sandbox

run-tests: test
