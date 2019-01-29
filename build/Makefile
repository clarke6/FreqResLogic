CC := g++

SRCDIR := src
BUILDDIR := obj
TARGETDIR := bin/debug
TARGET := $(TARGETDIR)/$(SRC)

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

ifeq ($(CPU), x86)
	CPUFLAGS := -m32
elseif ($(CPU), arm)
	CPUFLAGS :=
else
	CPUFLAGS :=
endif

CFLAGS := -Wall -pipe -std=c++11 -Wno-long-long -Wno-deprecated -g -DQCC_OS_LINUX -DQCC_OS_POSIX -DQCC_DBG $(CPUFLAGS)
LIB := -lstdc++ -lpthread -lrt -lm
INC := -I src/include

$(TARGET) : $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo "\n\tLinking $(TARGET)\n"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "\n\tCompiling $<...\n"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "\n\tCleaning $(TARGET)\n"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean