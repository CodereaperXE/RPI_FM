#selecting compiler
CMP = g++ 

#setting compiler flags
CMPFLAGS = -std=c++17

#source files
SRCS = transmitter.cpp wav_read.cpp rpifm.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)
	
#executable
TARGET = rpifm

#building the target
$(TARGET):$(OBJS)
	$(CMP) $(CMPFLAGS) -o $(TARGET) $(OBJS)

#build object files
%.o: %.cpp
	$(CMP) $(CMPFLAGS) -c $< -o $@


# Clean up
clean:
	rm -f $(TARGET) $(OBJS)