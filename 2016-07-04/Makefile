CXX = g++
CLXX = clang++
LLVMLD = llvm-link
foo:=load_deep2_cpp.o load_deep_cpp.o
bar:=$(foo:.o=.cpp)
run_link_model.out: load_cpp.cpp load_deep_cpp.cpp load_deep2_cpp.cpp
	# $(CLXX) -c -emit-llvm -o load_cpp_bc1.bc load_cpp.cpp
	# $(CLXX) -c -emit-llvm -o load_deep_cpp_bc.bc load_deep_cpp.cpp
	# $(CLXX) -c -emit-llvm -o load_deep2_cpp_bc.bc load_deep2_cpp.cpp
	# $(LLVMLD) load_cpp_bc1.bc load_deep_cpp_bc.bc load_deep2_cpp_bc.bc -o load_cpp_bc.bc
	# $(CLXX) -O2 run_link_module.cpp -D __STDC_LIMIT_MACROS -D __STDC_CONSTANT_MACROS `llvm-config --cxxflags --ldflags --libs`  -o $@

	$(CLXX) -c -emit-llvm -o load_cpp_bc.bc load_cpp.cpp
	$(CXX) load_deep_cpp.cpp load_deep2_cpp.cpp -shared -fPIC -o libdeep.so
	$(CXX) -O2 run_link_module.cpp -L. -ldeep -Wl,-rpath,. -D __STDC_LIMIT_MACROS -D __STDC_CONSTANT_MACROS `llvm-config --cxxflags --ldflags --libs`  -o run_link_module.out

	# $(CLXX) -c -emit-llvm -o load_cpp_bc.bc load_cpp.cpp
	# $(CXX) -O2 run_link_module.cpp ./libload_cpp_static.a -D __STDC_LIMIT_MACROS -D __STDC_CONSTANT_MACROS `llvm-config --cxxflags --ldflags --libs`  -o $@
libload_cpp_static.a:$(foo)
	$(CXX) -c $(bar)
	ar rc $@ $(foo)
.PHONY:clean
clean:
	rm *.bc *.so *.a *.out
	
