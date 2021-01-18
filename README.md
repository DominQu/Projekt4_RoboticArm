# Projekt4_RoboticArm
Aplikacja symulująca robotyczne ramie, napisana z wykorzystaniem biblioteki SFML. Jest to projekt studencki na zajęcia z programowania.

Uruchamianie aplikacji:

Aplikacja wymaga zainstalowanej biblioteki SFML. W celu zainstalowania biblioteki sugeruję skorzystać z oficjalnego poradnika ze strony dystrybutora:
https://www.sfml-dev.org/tutorials/2.5/start-vc.php
To repozytorium zapewnia pliki źródłowe aplikacji oraz wykorzystywane przez nią zasoby. Aby skompilować projekt należy:

-MS Visual Studio: pliki z tego repozytorium należy umieścić w folderze odpowiednio skonfigurowanego projektu Visual Studio. Konfiguracja opisana w poradniku udostępnionym przez dysytrybutora.

-g++: znajdując się w folderze z plikami projektu należy wykonać następujące komendy:
      g++ -c main.cpp Robot.cpp
      g++ main.o Robot.o -o Robot -lsfml-graphics -lsfml-window -lsfml-system
      ./Robot
      
### Eng:

#### Project overview:

Simulation of a Robotic Arm made with SFML. This is a student project.
The arm can grab and move boxes with a magnet, it has limited movements, there is an option of following previosuly recorded moves.

#### Controls:

+ arrows - move the arm
+ W - create new box
+ space - magnet on/off
+ S - start saving moves (only the moves of the arm are saved)
+ E - end saving moves
+ R - load saved moves 

#### What I learned:

+ how to use external libraries in C++
+ how the SFML library works
+ how to model a robotic arm
+ what Object Oriented Programming is and how to write code in it


#### Running the app:
User needs to have the SFML library installed. In order to do that I recommend the official site: https://www.sfml-dev.org/tutorials/2.5/start-vc.php 

#### Compliation:
-MS Visual Studio: put all files from this repo inside a configured VS project. The configuration is described on the distributor's site.
-on Ubuntu using g++: execute this commands inside project folder:
      g++ -c main.cpp Robot.cpp
      g++ main.o Robot.o -o Robot -lsfml-graphics -lsfml-window -lsfml-system
      ./Robot
