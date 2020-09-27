import com.cycling74.max.*;

public class rotateSlider extends MaxObject {

    Atom[] values;
    Atom[] view;
    int viewLength = 12;
    int steps = 0;

    rotateSlider() {
        declareInlets(new int[] { DataTypes.LIST, DataTypes.INT, DataTypes.LIST });
        declareOutlets(new int[] { DataTypes.LIST, DataTypes.LIST });
    }

    private boolean arrayEquals(Atom[] a, Atom[] b) {
        if (a.length != b.length) {
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
                // float[] input = Atom.toFloat(a);

                // values = new float[a.length];
                // System.arraycopy(input,a.length-steps,values,0,steps);
                // System.arraycopy(input, 0, values, a.length - steps, a.length - steps);

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
        view = Atom.rotate(values, -steps);
        outlet(0, view);
    }
}
