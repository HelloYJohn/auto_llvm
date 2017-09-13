CXX = g++
CLXX = clang++
LLVMLD = llvm-link
JAVAC = /usr/java/jdk1.7.0_67-cloudera/bin/javac
JAVAH = /usr/java/jdk1.7.0_67-cloudera/bin/javah
JAVA = /usr/java/jdk1.7.0_67-cloudera/bin/java
foo:=load_deep2_cpp.o load_deep_cpp.o
bar:=$(foo:.o=.cpp)
JNI_INC = -I/usr/java/jdk1.7.0_67-cloudera/include/ -I/usr/java/jdk1.7.0_67-cloudera/include/linux/
SHARED_FLAGS = -shared -fPIC
LLVM_FLAGS = -D __STDC_LIMIT_MACROS -D __STDC_CONSTANT_MACROS `llvm-config --cxxflags --ldflags --libs`
so: load_cpp.cpp load_deep_cpp.cpp load_deep2_cpp.cpp run_link_module.cpp
	# $(CLXX) -c -emit-llvm -o load_cpp_bc1.bc load_cpp.cpp
	# $(CLXX) -c -emit-llvm -o load_deep_cpp_bc.bc load_deep_cpp.cpp
	# $(CLXX) -c -emit-llvm -o load_deep2_cpp_bc.bc load_deep2_cpp.cpp
	# $(LLVMLD) load_cpp_bc1.bc load_deep_cpp_bc.bc load_deep2_cpp_bc.bc -o load_cpp_bc.bc
	# $(CLXX) -O2 run_link_module.cpp -D __STDC_LIMIT_MACROS -D __STDC_CONSTANT_MACROS `llvm-config --cxxflags --ldflags --libs`  -o $@

	$(CLXX) -c -emit-llvm -o load_cpp_bc.bc load_cpp.cpp
	$(CXX) load_deep_cpp.cpp load_deep2_cpp.cpp -shared -fPIC -o libdeep.so
	$(CXX) -O2 run_link_module.cpp TestJni.cpp $(JNI_INC) $(LLVM_FLAGS) -rdynamic $(SHARED_FLAGS) -o librun_link_module.so
	# $(CXX) main.cpp -L. -lrun_link_module -Wl,-rpath,. -o main

	# $(CLXX) -c -emit-llvm -o load_cpp_bc.bc load_cpp.cpp
	# $(CXX) -O2 run_link_module.cpp ./libload_cpp_static.a -D __STDC_LIMIT_MACROS -D __STDC_CONSTANT_MACROS `llvm-config --cxxflags --ldflags --libs`  -o $@
libload_cpp_static.a:$(foo)
	$(CXX) -c $(bar)
	ar rc $@ $(foo)
jni:TestJni.h
	/usr/java/jdk1.7.0_67-cloudera/bin/javah TestJni
	/usr/java/jdk1.7.0_67-cloudera/bin/javac TestJni.java
	/usr/java/jdk1.7.0_67-cloudera/bin/java -Djava.library.path='.' TestJni

llvm_engine: llvm_engine.cpp
	$(CXX) -O2 llvm_engine.cpp -D __STDC_LIMIT_MACROS -D __STDC_CONSTANT_MACROS `llvm-config --cxxflags --ldflags --libs`  -o $@
.PHONY:clean
clean:
	rm *.bc *.so *.a *.out *.class
	
