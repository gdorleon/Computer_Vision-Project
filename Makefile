
OB += \
./DetectionPeau.o 
	

%.o:
	g++  -O0 -g3 -Wall -c  DetectionPeau.cpp  `pkg-config --cflags --libs opencv`

all:$(OB)
	@echo 'Building target: $@'
	g++  -o "DetectionPeau" $(OB)  `pkg-config --cflags --libs opencv`
	@echo 'Finished building target: $@'


clean:
	-rm $(OB) DetectionPeau


