# for self
SOURCE = test.cpp

#PREDPEND = socket nsl
# PREDPEND = pthread

TARGET = test

# for normal

CXX = g++
CXXFLAG = -ggdb -c -m64
LDFLAG = -m64

LD_LIBARY = $(addprefix -l, $(PREDPEND) )

OBJECT= $(SOURCE:.cpp=.o)
DEPEND= $(SOURCE:.cpp=.d)

$(TARGET):$(OBJECT)
	$(CXX) $(LDFLAG) -o $(TARGET) $(LD_LIBARY) $(OBJECT)

$(DEPEND):%.d:%.cpp
	$(CXX) -MM $< > $@; \
	echo "	$(CXX) $(CXXFLAG) $< -o $(@:.d=.o)" >> $@		

-include $(DEPEND)

.PHONY : clean clean_all
clean:
	-rm -f $(TARGET) $(OBJECT) $(DEPEND) core*
