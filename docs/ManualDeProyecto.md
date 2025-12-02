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

Durante la **primera semana** nos concentramos en familiarizarnos con las herramientas nuevas del proyecto. Realizamos los tutoriales brindados por la cátedra, armamos el CMakefile base y clonamos el repositorio descargando todas las dependencias necesarias para poder compilar sin problemas.

En la **segunda semana** comenzamos a implementar el lobby y a desarrollar las primeras piezas del protocolo, los hilos y el gameloop del cliente y del servidor. A la vez comenzamos con las tareas de empezar a dibujar un vehículo con el mapa y a estructurar las físicas que usaremos luego.

La **tercera semana** la dedicamos a pulir el lobby, estructurar mejor la navegación y consolidar toda la comunicación entre cliente y servidor a través del protocolo que habíamos diseñado. Las colisiones y el movimiento ya estaban funcionando.

En la **cuarta semana** ya teníamos el juego conectado: múltiples clientes podían unirse al lobby, crear o entrar a salas, seleccionar autos e iniciar una partida correctamente. También logramos implementar el editor de niveles. Además, ya teníamos varios jugadores en una partida completando checkpoints, y chocando entre sí durante las carreras; también logramos que la salud del auto disminuyera dependiendo del tipo de choque y que se cumpla la condición de que un jugador pierda si pierde toda la salud.

Durante la **quinta semana** nos dedicamos a modularizar el código y a intentar implementar las funcionalidades faltantes. Logramos que el lobby fuera completamente responsive y perfeccionamos el editor para mejorar la experiencia del usuario. En este punto, un usuario ya podía crear sus propios recorridos y elegirlos al crear una partida.

Durante la **sexta y séptima semana** implementamos los últimos detalles y requisitos faltantes. Logramos que el servidor sea capaz de leer los recorridos predeterminados y creados por el usuario con el editor de mapas, que pueda procesarlos en YAML y convertirlos a SDL para renderizarlos en el juego. También logramos crear una animación de explosión, agregar las pantallas de finalización de carrera, la lógica para renderizar correctamente el auto debajo de puentes, la incorporación de NPCs, la integración de cheats y la adición de mejoras para los autos al terminar los recorridos. Finalmente, elaboramos toda la documentación requerida con sus respectivos diagramas, desarrollamos los tests y generamos el instalador del juego.


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
    https://github.com/Taller-de-Programacion/clases/tree/feature/bibliotecas-gui/bibliotecas-gui/qt5
- **Google Test**: https://github.com/google/googletest

---
## Consideraciones


### Reflexión sobre el proyecto y las dificultades

Desde el lado de los hilos y el protocolo, los primeros dos TPs fueron de mucha ayuda para entender la base y plantear las ideas del proyecto. Lo más complicado fue mantener el modelo monohilo hasta que iniciara la partida, sin crear un sender ni receiver antes de tiempo.

Desde el lado del Cliente el principio fue complicado porque habia que aprender a utilizar la libreria SDL2pp la cual tiene una documentacion complicada y escueta. Sin embargo una vez aprendidas las bases del funcionamiento de la libreria todo fue mas sencillo y se pudo sacar funcionalidades a la par del servidor.

Desde el lado del lobby y el editor de mapas, no tuve demasiadas dificultades para avanzar. Siguiendo los tutoriales, leyendo la documentación y conociendo la programación orientada a objetos, gran parte del trabajo resultó bastante llevadero. Las complicaciones más notorias aparecieron al momento de familiarizarme con Qt, especialmente con la estructura de los .ui, los layouts y los estilos; ajustar detalles visuales o de usabilidad en Qt, que a veces llevan mucho tiempo para algo pequeño. Pero son desafíos normales cuando se aprende una herramienta completamente nueva.

Desde el lado del Servidor las complicaciones fueron que habia demasiadas responsabilidades diferentes. El tener que aprender el funcionamiento de una libreria compleja como Box2D, el aprender a como se deben definir las colisiones desde cero y el manejar toda la logica del juego contribuyeron en una sobrecarga de responsabilidades en una persona.

### ¿Qué contenidos podrían incorporarse en Taller?

Estrategias de trabajo colaborativo y organización básica de un proyecto.

### ¿Pudimos llegar con todo?

Si, pero se podrían mejorar varios aspectos del juego con más tiempo. Nos faltó terminar de editar el video promocional para la audiencia potencial, falta mostrar el editor de mapas, mostrar las mejoras disponibles, los cheats y el podio al finalizar la aprtida.

### Errores conocidos

- No se puede dar marcha atrás y girar
- Algunos puentes se dibujan por debajo del jugador
- Los puentes en los que se puede cruzar por arriba y por abajo no tienen la colision correcta
- Hay un error que a veces los checkpoints quedan desactualizados en la ui y entonces la flecha que apunta al próximo no funciona correctamente

### ¿Qué cambiaríamos si lo hiciéramos de nuevo?

La división de tareas. Consideramos que la carga horaria no fue equitativa y por tanto si lo hiciesemos de nuevo apostaríamos más por un enfoque colaborativo en varias partes del trabajo en vez de ir por separado en cuatro partes distintas.
