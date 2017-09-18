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

class LongValue {
    private long value;
    public LongValue(long i) {
        value = i;
    }

    public LongValue() {
        value = 0;
    }
    public long getValue() {
        return value;
    }
    public void setValue(long i) {
        value = i;
    }
}

class DoubleValue{
    private double value;
    public DoubleValue(double i) {
        value = i;
    }

    public DoubleValue() {
        value = 0;
    }
    public double getValue() {
        return value;
    }
    public void setValue(double i) {
        value = i;
    }
}

class JniStatus {
    private boolean m_hasWrong;
    private int m_errorCode;
    private String m_errorMsg;

    public JniStatus() {
        m_hasWrong = false;
        m_errorCode = 0;
        m_errorMsg = "NULL";
    }
    public boolean getM_hasWrong() {
        return m_hasWrong;
    }

    public void setM_hasWrong(boolean hasWrong) {
        m_hasWrong = hasWrong;
    }

    public int getM_errorCode() {
        return m_errorCode;
    }

    public void setM_errorCode(int errorCode) {
        m_errorCode = errorCode;
    }

    public String getM_errorMsg() {
        return m_errorMsg;
    }

    public void setM_errorMsg(String errorMsg) {
        m_errorMsg = errorMsg;
    }
}


public class TestJni {
    TestJni() { }
    public native void invoke(long funcId, Object ret, List<Object> paraList, JniStatus status);
    public static void main(String[] args) {
        System.loadLibrary("run_link_module");
        /*
        TestJni testjni = new TestJni();
        Object add_ret = new IntegerValue();
        List<Object> add_paraList = new ArrayList<Object>();
        add_paraList.add((Object)new IntegerValue(1));
        add_paraList.add((Object)new IntegerValue(2));
        JniStatus status = new JniStatus();
        testjni.invoke(4294967297L, add_ret, add_paraList, status);
        if (status.getM_hasWrong()) {
            System.out.println("jni invoke error: " + status.getM_errorCode() + " " + status.getM_errorMsg());
        } else {
            System.out.println("jni: " + ((IntegerValue)add_ret).getValue());
        }
        */
        TestJni testjni = new TestJni();
        Object sub_ret = new IntegerValue();
        List<Object> sub_paraList = new ArrayList<Object>();
        sub_paraList.add((Object)new IntegerValue(1));
        sub_paraList.add((Object)new IntegerValue(2));
        JniStatus status = new JniStatus();
        testjni.invoke(8589934593L, sub_ret, sub_paraList, status);
        if (status.getM_hasWrong()) {
            System.out.println("jni invoke error: " + status.getM_errorCode() + " " + status.getM_errorMsg());
        } else {
            System.out.println("jni: " + ((IntegerValue)sub_ret).getValue());
        }
    }
}

