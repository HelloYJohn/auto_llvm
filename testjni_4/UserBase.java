public class UserBase {
    private long id;
    public UserBase() {
       super(); 
       id = 0;
    }
    public UserBase(long id) {
        super();
        this.id = id;
    }
    public void setId(long id) {
        this.id = id;
    }
    public long getId() {
        return id;
    }
}
