clear
rm -r "bin"
mkdir "bin"

g++ -c -Iinclude "src/main.cpp" -o "bin/main.o"
g++ -c -Iinclude "src/app/window.cpp" -o "bin/window.o"
g++ -c -Iinclude "src/machine/bus.cpp" -o "bin/bus.o"
g++ -c -Iinclude "src/machine/arch.cpp" -o "bin/arch.o"
g++ -c -Iinclude "src/machine/cpu.cpp" -o "bin/cpu.o"
g++ -c -Iinclude "src/machine/machine.cpp" -o "bin/machine.o"
g++ -Iinclude "bin/main.o" "bin/window.o" "bin/bus.o" "bin/arch.o" "bin/machine.o" "bin/cpu.o" -o "bin/lilsys_vm" -lsfml-graphics -lsfml-window -lsfml-system # -fsanitize=undefined -fsanitize=address
./bin/lilsys_vm