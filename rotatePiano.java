import com.cycling74.max.*;

public class rotatePiano extends MaxObject {

    int[] noteToggles = new int[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    Atom[] noteView = Atom.newAtom(new int[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });
    Atom[] noteViewPrev = Atom.newAtom(new int[] { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 });

    int viewLength = 12;
    int steps = 0;

    rotatePiano() {
        declareInlets(new int[] { DataTypes.LIST, DataTypes.INT, DataTypes.LIST });
        declareOutlets(new int[] { DataTypes.LIST, DataTypes.LIST });
    }

    protected void inlet(int v) {
        int idx = getInlet();
        switch (idx) {
            case 1:
                int l = viewLength;
                steps = (-v + l) % l;
                setPiano();
                break;
        }
    }

    private boolean arrayEquals(int[] a, int[] b) {
        if (a.length != b.length) {
            return false;
        }
        for (int i = 0; i < a.length; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }

    protected void list(Atom[] a) {
        int idx = getInlet();
        switch (idx) {
            case 0:
                int[] input = Atom.toInt(a);
                int i = (input[0] + steps) % viewLength;
                int v = (input[1] == 0 ? 1 : 0);
                if (noteToggles[i] == v) {
                    return;
                } else {
                    noteToggles[i] = v;
                    outlet(2, noteToggles);
                    calcView();
                    outlet(1, noteView);
                }
                break;
            case 1:
                int[] updatedValues = Atom.toInt(a);
                if (arrayEquals(noteToggles, updatedValues)) {
                    return;
                } else {
                    noteToggles = updatedValues;
                    setPiano();
                }
                break;
        }
    }

    private void calcView() {
        noteView = Atom.rotate(Atom.newAtom(noteToggles), -steps);
    }

    private void setPiano() {
        calcView();
        for (int i = 0; i < noteView.length; i++) {
            if (!noteView[i].equals(noteViewPrev[i])) {
                outlet(0, new String[] { "set", Integer.toString(i), noteView[i].toBoolean() ? "0" : "1" });
            }
        }
        noteViewPrev = noteView;
        outlet(1, noteView);
    }
}
