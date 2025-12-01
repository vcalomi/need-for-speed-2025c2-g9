# 📘 Manual del Proyecto – Need for Speed 

## Introducción
El presente informe busca **documentar cómo fue la división del trabajo práctico** de la materia Taller de Programación, cátedra Veiga, del segundo cuatrimestre del año 2025.

Desarrollamos el juego **Need for Speed**, en el lenguaje C++. El cual incluía tres programas:
1. Un programa **cliente**, que sería utilizado por los jugadores, constando de una interfaz gráfica divida primero en la librería QT para realizar la etapa de lobby (donde los jugadores se registran, crean o se unen a una partida y eligen su auto); y luego la etapa del juego en sí sería desarrollado en la librería SDL.
2. Un programa **servidor**, que manejaría los múltiples clientes y sus partidas, y la lógica del juego en sí.
3. Un programa **editor**, en el cual los usuarios pueden crear recorridos en las tres ciudades disponibles para después jugar en ellos, desarrollado al igual que la etapa de lobby con la librería QT.

---

## División de tareas 
Luego de tener la primera reunión con nuestro corrector, decidimos seguir sus indicaciones respecto a la división de tareas. Y decicimos que sea de la siguiente manera:
- **Lógica de juego y Box2D**: Scopel, Ignacio Nicolás. 
- **Arquitectura del servidor y protocolo**: Galli, Juana Eva.
- **Interfaz gráfica del juego (SDL)**: Calomino, Valentín.
- **Lobby y editor de niveles**: Lema, Mora.

---

## Organización de trabajo
Para garantizar una comunicación efectiva y un desarrollo coordinado, implementamos un documento compartido donde detallamos cada parte del proyecto. Esta documentación nos permitió clarificar las responsabilidades de cada integrante, facilitar el entendimiento de las tareas interconectadas, mantener el acceso constante a la información del proyecto para todos los miembros y coordinar las dependencias entre módulos que requerían avance continuo de otras partes.
La comunicación fue fundamental dado que múltiples componentes del sistema estaban vinculados y dependían del progreso paralelo de los demás integrantes. También realizamos reuniones periódicas para mostrar nuestros avances, consultar inquietudes y organizar futuras implementaciones.

---

## Desarrollo

---

## Herramientas utilizadas
- **IDEs**: VS Code y CLion 
- **SDL2pp**
- **QTCreator**
- **Live Share** (Visual Studio Code)
- **Google Tests**
- **Herramientas de desarrollo**: linters ofrecidos por la cátedra (clang-format, cppcheck, cpplint) y CMake.

---

### Documentación requerida para utilización de las herramientas:
- **Box2D**: https://www.iforce2d.net/b2dtut/introduction
- **SLD2pp**: https://github.com/libSDL2pp/libSDL2pp
- **QT**: https://doc.qt.io/
    clases/bibliotecas-gui/qt5 at feature/bibliotecas-gui · Taller-de-Programacion/clases
- **Google Test**: https://github.com/google/googletest

---
## Consideraciones

Hay algo que crean que debería darse en Taller? Indiquenlos también.
¿Cuáles fueron los puntos más problemáticos? ¿Pudieron llegar con todo? ¿Hay errores conocidos? Si volvieran a hacer el proyecto, que cambiarían (a nivel código o a nivel organizacional).
