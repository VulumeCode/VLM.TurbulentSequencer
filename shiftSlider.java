import com.cycling74.max.*;

public class shiftSlider extends MaxObject {
    Atom[] values;
    Atom[] view;
    int steps = 0;
    int viewLength = 10;

    shiftSlider() {
        declareInlets(new int[] { DataTypes.LIST, DataTypes.INT, DataTypes.LIST });
        declareOutlets(new int[] { DataTypes.LIST, DataTypes.LIST });

        values = new Atom[viewLength * 3];
        for (int i = 0; i < values.length; i++) {
            values[i] = Atom.newAtom(0.);
        }
        view = new Atom[viewLength];
        for (int i = 0; i < view.length; i++) {
            view[i] = Atom.newAtom(0.);
        }
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
                System.arraycopy(a, 0, values, viewLength + steps, viewLength);
                outlet(1, values);
                // outlet(0, "setlist", view);
                break;
            case 2:
                if (arrayEquals(a, values)) {
                    return;
                } else {
                    values = a;
                    outputView();
                }
                break;
        }
    }

    protected void inlet(int v) {
        int idx = getInlet();
        switch (idx) {
            case 1:
                steps = -v;
                outputView();
                break;
        }
    }

    protected void outputView() {
        System.arraycopy(values, steps + viewLength, view, 0, viewLength);
        outlet(0, view);
    }

}
