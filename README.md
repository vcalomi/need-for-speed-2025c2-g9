# Need for speed
### Proyecto Final — Taller de Programación [TA045]

➤ **Cátedra Veiga, 2C2025**

![Captura de pantalla página de conexion](assets/images/Imagen1.png)

### Integrantes del Equipo 

* **Calomino, Valentín** — Padrón: 109665 
* **Galli Orellana, Juana Eva** — Padrón: 111710 
* **Lema, Mora** — Padrón: 111831
* **Scopel, Ignacio Nicolás** — Padrón: 111305

---

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
Ejecutar los siguientes comandos en terminales separadas desde la raíz del proyecto:

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
---

## Video Promocional

https://www.youtube.com/watch?v=JTv96m46JTE

## Documentación

- [Documentación técnica](/docs/DocumentacionTecnica.md)
- [Manual de proyecto](/docs/ManualProyecto.md)
- [Manual de usuario](/docs/ManualDeUsuario.md)

## Créditos

Las clases Socket, Queue<T>, Thread y las funciones utilizadas en ellas fueron diseñadas y creadas por Di Paola Martín, docente de Taller de Programación [TA045], curso Veiga, Facultad de Ingeniería, Universidad de Buenos Aires.

Licencia: GPL v2

El código original puede consultarse en el repositorio oficial: [Threads](https://github.com/eldipa/hands-on-threads) y [Sockets](https://github.com/eldipa/hands-on-sockets-in-cpp).
