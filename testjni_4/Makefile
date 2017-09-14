JAVAC = /usr/java/jdk1.7.0_67-cloudera/bin/javac
JAVAH = /usr/java/jdk1.7.0_67-cloudera/bin/javah
JAVA = /usr/java/jdk1.7.0_67-cloudera/bin/java
CXX = g++
test: HelloJni.h HelloJni.c HelloJni.java User.java UserBase.java
	$(JAVAH) HelloJni
	$(CXX) -I/usr/java/jdk1.7.0_67-cloudera/include/ -I/usr/java/jdk1.7.0_67-cloudera/include/linux/ -fPIC -shared -o libuserbean.so HelloJni.c	
	$(JAVAC) HelloJni.java
	$(JAVA) -Djava.library.path=. HelloJni

.PHONY:clean
clean:
	rm *.so *.class
 		
