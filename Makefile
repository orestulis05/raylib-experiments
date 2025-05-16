default:
	g++ src/main.cpp src/utils.cpp src/physics/physics.cpp -o program -Llib -Iinclude -lraylib -Isrc
