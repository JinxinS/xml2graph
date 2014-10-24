SHELL = /bin/bash

LSIDECONF_DIR 		:= #./lib/libsideconf
LSIDECONF_HEADER_DIR := #./lib/libsideconf/include

CPP_SRC_DIRS   := $(filter ./src/%, $(sort $(dir $(shell /usr/bin/find . -name '*.cpp' ))))
CPP_HEADER_DIRS:= $(filter ./src/%, $(sort $(dir $(shell /usr/bin/find . -name '*.h'   ))))

CPP_SRCS   := $(wildcard  $(addsuffix *.cpp, $(CPP_SRC_DIRS)))
CPP_HEADERS:= $(wildcard $(addsuffix *.h, $(CPP_HEADER_DIRS)))

OUTPUT_DIR := build
OBJ_DIR 	:= $(OUTPUT_DIR)/obj
BIN_DIR 	:= $(OUTPUT_DIR)/bin
OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(notdir $(CPP_SRCS:.cpp=.o)))
DEP_FILES := $(OBJ_FILES:.o=.d)

BIN := $(BIN_DIR)/xml2graph
RELEASE_DIR := release
RELEASE_BIN := $(RELEASE_DIR)/$(notdir $(BIN))
RELEASE_HEADERS := $(addprefix $(RELEASE_DIR)/, $(notdir $(CPP_HEADERS)))

CXX := g++
CXXFLAGS := -g03 -std=c++0x -Weffc++ -Wall -Wextra -Wmissing-include-dirs -Wunused -Wuninitialized \
-Woverloaded-virtual -Wredundant-decls -Winline -Wctor-dtor-privacy
CXXDEFINES :=
INCLUDES := $(addprefix -I, $(CPP_HEADER_DIRS)) $(addprefix -I, $(LSIDECONF_HEADER_DIR))
SED := sed

LDFLAGS := 
LIBS_DIR := $(LSIDECONF_DIR)
LIBS := -lSDL2 -lSDL2_ttf -lSDL2_image #-lsideconf
ALL_LDFLAGS := $(LDFLAGS) $(addprefix -L, $(LIBS_DIR)) $(LIBS)


vpath %.cpp 	$(CPP_SRC_DIRS)
vpath %.h 		$(CPP_HEADER_DIRS)
vpath %.a		$(LIBS_DIR)

# All Target
all: release-make  

release-make : $(RELEASE_HEADERS) $(RELEASE_BIN)

$(RELEASE_HEADERS): $(RELEASE_DIR)/%.h : %.h
$(RELEASE_HEADERS): | $(RELEASE_DIR)
	cp $^ -t $(RELEASE_DIR)

$(RELEASE_BIN):$(BIN) | $(RELEASE_DIR)
	cp $< $@

# Tool invocations
$(BIN): $(OBJ_FILES) |$(BIN_DIR)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	@echo $(OBJ_FILES)
	g++ -o $(BIN) $(OBJ_FILES) $(ALL_LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

$(OBJ_FILES): $(OBJ_DIR)/%.o : %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CXXDEFINES)  $(INCLUDES)  -MMD -MT $@ -MT $(@:.o=.d) -c $< -o $@

$(OBJ_DIR) $(BIN_DIR) : | $(OUTPUT_DIR)
$(OBJ_DIR) $(BIN_DIR) $(OUTPUT_DIR) $(RELEASE_DIR):
	mkdir $@

# Other Targets
clean:
	$(RM) -r $(RELEASE_DIR) $(OUTPUT_DIR)

.PHONY: all clean dependents