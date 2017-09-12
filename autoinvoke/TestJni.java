import java.util.*;
public class TestJni {
    TestJni() { }
    public native void invoke(String str);
    public static void main(String[] args) {
        System.loadLibrary("run_link_module");
        TestJni testjni = new TestJni();
        testjni.invoke("add");
    }
}

