CXX      = g++

EXE		 = easyscript
DLL		 = easyscript.dll

SRC		 = ../src
INCLUDE	 = ../include

FLAGS	 = -std=c++17 -I$(INCLUDE) -L../lib -lgdi32
CXXFLAGS = -c -Wall $(FLAGS)
LDFLAGS	 =  $(FLAGS) -o
DLLFLAGS = -I$(INCLUDE) -DDLL -DEASYSCRIPT_EXPORTS -DPIXEL_EXPORTS -o

OBJECTS	 = Easyscript.o Pixel.o

all: main.o $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(EXE)

dll: $(OBJECTS)
	ar rcs libeasyscriptdll.a $(OBJECTS) -lgdi32
	$(CXX) $(DLLFLAGS) $(DLL) $(SRC)/Easyscript.cpp $(SRC)/Pixel.cpp -lgdi32

Easyscript.o: $(SRC)/Easyscript.cpp $(INCLUDE)/Easyscript.h Pixel.o
	$(CXX) $(CXXFLAGS) $<

Pixel.o: $(SRC)/Pixel.cpp $(INCLUDE)/Pixel.h
	$(CXX) $(CXXFLAGS) $<

main.o: $(SRC)/main.cpp
	$(CXX) $(CXXFLAGS) $<

clean:
	del -q *.o *.exe *.dll *.a

