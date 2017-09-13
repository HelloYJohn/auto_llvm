import java.util.*;  
public class HelloJni{  
    static{  
        System.loadLibrary("userbean");  
    }  
    public native int get();  
    public native void set(int i);  
    public native void setUser(String userName);  
    public native void getUser(UserBase user);  
    public native void setUserList(ArrayList<UserBase> userList);  
    public native ArrayList<UserBase> getUserList();  
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
        ArrayList<UserBase> userList = new ArrayList<UserBase>();  
        for(int i=0;i<10;i++){  
            // UserBase u = new User((long)(1000+i),"LiangYaotian"+i,true,21);  
            UserBase u = new User();  
            userList.add(u);  
        }  
        hello.setUserList(userList);  
        for (UserBase ub : userList) {
            User u = (User)ub;
            System.out.print(u.getId() + " ");
            System.out.print(u.getUserName() + " ");
            System.out.print(u.isMan() + " ");
            System.out.println(u.getAge());

        }
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
