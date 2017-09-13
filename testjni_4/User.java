public class User extends UserBase {  
        private String userName;  
        private boolean isMan;  
        private int age;  
        public User(){
            super();
            this.userName = "";
            this.isMan = true;
            this.age = 0;
        }  
        
        public User(long id, String userName, boolean isMan, int age) {  
                super(id);  
                this.userName = userName;  
                this.isMan = isMan;  
                this.age = age;  
        }  
        public String getUserName() {  
                return userName;  
        }  
        public void setUserName(String userName) {  
                this.userName = userName;  
        }  
        public boolean isMan() {  
                return isMan;  
        }  
        public void setMan(boolean isMan) {  
                this.isMan = isMan;  
        }  
        public int getAge() {  
                return age;  
        }  
        public void setAge(int age) {  
                this.age = age;  
        }  
}
