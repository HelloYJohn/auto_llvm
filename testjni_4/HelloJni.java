import java.util.*;  
public class HelloJni{  
    static{  
        System.loadLibrary("userbean");  
    }  
    public native int get();  
    public native void set(Object i);  
    public native void setUser(String userName);  
    public native void getUser(UserBase user);  
    public native void setUserList(ArrayList<Object> userList);  
    public native ArrayList<Object> getUserList();  
    public static void main(String [] args)  
    {  
        HelloJni hello = new HelloJni();  
        /*
           UserBase userBase1 = new User();
           hello.getUser(userBase1);
           System.out.println("Id: " + userBase1.getId());
           User user = (User)userBase1;
           System.out.println("user from c/c++"); 
           System.out.println("name:"+user.getUserName()); 
           System.out.println("isMan?:"+user.isMan()); 
           System.out.println("age:"+user.getAge()); 
           */ 
        ArrayList<Object> userList = new ArrayList<Object>();  
        for(int i=0;i<10;i++){  
            // UserBase u = new User((long)(1000+i),"LiangYaotian"+i,true,21);  
            Object o = new User();  
            userList.add(o);
        } 
        hello.setUserList(userList);  
        for (Object o : userList) {
            User u = (User)o;
            System.out.print(u.getId() + " ");
            System.out.print(u.getUserName() + " ");
            System.out.print(u.isMan() + " ");
            System.out.println(u.getAge());

        }
        Object i = new Integer(0);
        hello.set(i);
        System.out.println((Integer)i);
        /*
           userList = null;  
           userList = hello.getUserList();  
           System.out.println("ArrayList<UserBase> construct from C++,then Java print it.....");  
           for(UserBase ub : userList){  
           User u = (User)ub;
           System.out.println("id:"+u.getId()+"; name:"+u.getUserName());  
           }
           */
    }
}
