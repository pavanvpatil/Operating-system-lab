all: part1 part2_1a part2_1b part2_2 part2_3

part1: 1
	./1 car.ppm output_part1.ppm

part2_1a: 2_1_a
	./2_1_a car.ppm output_part2_1a.ppm

part2_1b: 2_1_b
	./2_1_b car.ppm output_part2_1b.ppm

part2_2: 2_2
	./2_2 car.ppm output_part2_2.ppm

part2_3: 2_3
	./2_3 car.ppm output_part2_3.ppm



1: part1.cpp
	g++ part1.cpp -o 1
	
2_1_a: part2_1a.cpp
	g++ part2_1a.cpp -o 2_1_a

2_1_b: part2_1b.cpp
	g++ part2_1b.cpp -o 2_1_b

2_2: part2_2.cpp
	g++ part2_2.cpp -o 2_2

2_3: part2_3.cpp
	g++ part2_3.cpp -o 2_3

clean:
	rm -f 1 2_1_a 2_1_b 2_2 2_3 output_part1.ppm output_part2_1a.ppm output_part2_1b.ppm output_part2_2.ppm output_part2_3.ppm
