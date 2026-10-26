ifeq ($(origin CXX) ,default)
CXX = g++
endif

OUT_O_DIR ?= build
COMMONINC = -I./. -I./LFU -I./2Q -I./ARC -I./LIRS

TESTS = ./LIRS/LIRS_test				# !
SRC = ./source							# ?
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST)))) # ?

override CFLAGS += $(COMMONINC)

CSRC = main.cpp slow_get_page.cpp get_params.cpp

# reproducing source tree in object tree
COBJ := $(addprefix $(OUT_O_DIR)/, $(CSRC:.cpp=.o))
DEPS = $(COBJ:.o=.d)

.PHONY: all
all: $(OUT_O_DIR)/cache.x

$(OUT_O_DIR)/cache.x: $(COBJ)
	(CXX) $^ -o $@ $(LDFLAGS)

# static pattern rule to not redefine generic one
$(COBJ) : $(OUT_O_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

$(DEPS) : $(OUT_O_DIR)/%.d : %.cpp
	@mkdir -p $(@D)
	$(CXX) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

# parse tests
TSRC = $(wildcard $(TESTS)/*.cpp)
TOBJ = $(addprefix $(OUT_O_DIR)/, $(TSRC:.cpp=.o))
TEST_EXE = $(OUT_O_DIR)/run_tests

GTEST_LIBS = -lgtest -lgtest_main -pthread

CORE_OBJ = $(filter-out $(OUT_O_DIR)/main.o, $(COBJ))

$(OUT_O_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: testrun
testrun: $(TEST_EXE)
	./$(TEST_EXE)

$(TEST_EXE): $(TOBJ) $(CORE_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS) $(GTEST_LIBS)

PHONY: clean
clean:
	rm -rf $(COBJ) $(DEPS) $(OUT_O_DIR)/*.x $(OUT_O_DIR)/*.log

# targets which we have no need to recollect deps
NODEPS = clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif
