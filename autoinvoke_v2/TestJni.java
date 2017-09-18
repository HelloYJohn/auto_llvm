import java.util.*;
class IntegerValue {
    private int value;
    public IntegerValue(int i) {
        value = i;
    }

    public IntegerValue() {
        value = 0;
    }
    public int getValue() {
        return value;
    }
    public void setValue(int i) {
        value = i;
    }
}

public class TestJni {
    TestJni() { }
    public native void invoke(long funcId, Object ret, List<Object> paraList);
    public static void main(String[] args) {
        System.loadLibrary("run_link_module");
        TestJni testjni = new TestJni();
        Object add_ret = new IntegerValue();
        List<Object> add_paraList = new ArrayList<Object>();
        add_paraList.add((Object)new IntegerValue(1));
        add_paraList.add((Object)new IntegerValue(2));
        testjni.invoke(4294967297L, add_ret, add_paraList);
        System.out.println("jni: " + ((IntegerValue)add_ret).getValue());
    }
}

