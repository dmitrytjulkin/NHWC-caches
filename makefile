IMPROVED_FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat 			 \
		-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts				 \
		-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral	 \
		-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor	 \
		-Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls		 \
		-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override	 \
		-Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused				 \
		-Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing				 \
		-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new					 \
		-fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer \
		-Wlarger-than=8192 -fPIE -Werror=vla \
		-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CC = g++

MAIN = main.cpp

LFU_DIR = LFU
LIRS_DIR = LIRS

OTHER = slow_get_page.cpp $(LFU_DIR)/LFU.cpp
HEADERS = $(LIRS_DIR)/LIRS.hpp

TESTS = $(LIRS_DIR)/LIRS_test.cpp

run_nhwc: $(MAIN) $(OTHER)
	$(CC) $(IMPROVED_FLAGS) $^ -o $@

GTEST_DIR = third_party/googletest
GTEST_SRCS = $(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc

TESTS_FLAGS = -std=c++17 -Wall -I$(GTEST_DIR)/include -I$(GTEST_DIR)

run_tests: $(TESTS) $(OTHER) $(GTEST_SRCS) $(HEADERS)
	$(CC) $(TESTS_FLAGS) $(TESTS) $(OTHER) $(GTEST_SRCS) -o $@

clean:
	rm -rf $(BINARY).dSYM \
	rm -f BINARY