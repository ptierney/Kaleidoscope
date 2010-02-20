
CPP := g++
FLAGS := -g

# the find upwards is not needed, it searches ., .., ../.., etc
INCLUDE_DIR := $(find_upwards include)

INCLUDES := -I$(INCLUDE_DIR)

%.o : %.cpp
	$(CPP) $(FLAGS) $(INCLUDES) -c $(input) -o $(output) 

