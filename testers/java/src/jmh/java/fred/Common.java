package fred;

import java.io.IOException;

public abstract class Common {
    protected void value(String format, String implementation, String id, Integer value)
    {
        System.out.printf("value: %s java %s %s %d\n", format, implementation, id, value);
    }
    protected void environment(String format, String implementation, String key, String value)
    {
        System.out.printf("environment: %s java %s %s %s\n", format, implementation, key, value);
    }
}
