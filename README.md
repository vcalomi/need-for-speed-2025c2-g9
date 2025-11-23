## 2. 🔨 Instalación y Compilación

Para instalar las dependencias necesarias, en la raíz del proyecto ejecuta:

```bash
make install
```
El target se encargará de instalar las dependencias necesarias para el juego, compilarlo y finalmente instalar el juego en el sistema.

Limpiar compilación
```bash
make clean
```

Desinstalar
```bash
make uninstall
```
---

## 3. 🚀 Ejecución del Software
Todos los comandos se ejecutan desde la carpeta `build/`

**Levantar el servidor**
```bash
nfs_server <puerto>
```

Ejemplo:
```bash
nfs_server 8080
```

**Abrir el cliente**
En otra terminal:
```bash
nfs_client <host> <puerto>
```

Ejemplo:
```bash
nfs_client 127.0.0.1 8080
```

**Abrir el editor de mapas**
Podés abrirlo desde el lobby del cliente o directamente desde la terminal:
```bash
nfs_editor
```

**Ejecutar tests**
```bash
nfs_tests
```