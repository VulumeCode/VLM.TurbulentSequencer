import com.cycling74.max.*;

public class rotary extends MaxObject {

    rotary() {
        declareInlets(new int[] { DataTypes.ANYTHING, DataTypes.ANYTHING, DataTypes.ANYTHING });
        declareOutlets(new int[] { DataTypes.ANYTHING, DataTypes.ANYTHING });
    }

    rotary(Atom[] a) {
        declareInlets(new int[] { DataTypes.ANYTHING, DataTypes.ANYTHING, DataTypes.ANYTHING });
        declareOutlets(new int[] { DataTypes.ANYTHING, DataTypes.ANYTHING });
        if (a.length == 2) {
            from = a[0].toFloat();
            to = a[1].toFloat();
        } else if (a.length == 1) {
            from = -a[0].toFloat();
            to = a[0].toFloat();
        } else if (a.length == 0) {
            //
        } else {
            error(a.length + "parameters, expected 0 1 or 2");
        }
    }

    float from, to, in;

    protected void inlet(int v) {
        int idx = getInlet();
        switch (idx) {
            case 0:
                in = v;
                break;
            case 1:
                from = v;
                break;
            case 2:
                to = v;
                break;
        }

        if (in > to || in < from) {
            in = wrap();
            outlet(0, "set", (int) in);
        }
        outlet(1, (int) in);
    }

    protected void inlet(float v) {
        int idx = getInlet();
        switch (idx) {
            case 0:
                in = v;
                break;
            case 1:
                from = v;
                break;
            case 2:
                to = v;
                break;
        }
        if (in > to || in < from) {
            in = wrap();
            outlet(0, "set", (float) in);
        }
        outlet(1, (float) in);
    }

    private float wrap() {
        if (in == from) {
            return to;
        } else if (in == to) {
            return from;
        } else if (in >= to) {
            in = (in - (to - from));
            return wrap();
        } else if (in <= from) {
            in = (in + (to - from));
            return wrap();
        } else {
            return in;
        }
    }
}
