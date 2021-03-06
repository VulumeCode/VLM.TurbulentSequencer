import com.cycling74.max.*;

public class rotateSlider extends MaxObject {

    Atom[] values;
    int viewLength = 12;
    int steps = 0;

    rotateSlider() {
        declareInlets(new int[] { DataTypes.LIST, DataTypes.INT, DataTypes.LIST });
        declareOutlets(new int[] { DataTypes.LIST, DataTypes.LIST });
    }

    private boolean arrayEquals(Atom[] a, Atom[] b) {
        if (a == null || b == null || a.length != b.length) {
            return false;
        }
        for (int i = 0; i < a.length; i++) {
            if (!a[i].equals(b[i])) {
                return false;
            }
        }
        return true;
    }

    protected void list(Atom[] a) {
        int idx = getInlet();
        switch (idx) {
            case 0:
                values = Atom.rotate(a, steps);
                outlet(1, values);
                break;
            case 2:
                Atom[] updatedValues = a;
                if (arrayEquals(updatedValues, values)) {
                    return;
                } else {
                    values = updatedValues;
                    outputView();
                }
                break;
        }
    }

    protected void inlet(int v) {
        int idx = getInlet();
        switch (idx) {
            case 1:
                int l = viewLength;
                steps = (-v + l) % l;
                outputView();
                break;
        }
    }

    protected void outputView() {
        if (values != null) {
            outlet(0, Atom.rotate(values, -steps));
        }
    }
}
