import java.util.LinkedList;
import com.cycling74.max.*;

public class findnotes extends MaxObject {
    float[] probs = new float[0];
    float[] floats = new float[0];
    int[] notes = new int[0];

    findnotes() {
        declareInlets(new int[] { DataTypes.LIST, DataTypes.LIST });
        declareOutlets(new int[] { DataTypes.LIST });
    }

    protected void list(final Atom[] a) {
        final int idx = getInlet();
        switch (idx) {
            case 0:
                floats = Atom.toFloat(Atom.reverse(a));
                break;
            case 1:
                probs = Atom.toFloat(a);
                break;
        }
        calc();
        bang();
    }

    private void calc() {
        final LinkedList<Integer> noteList = new LinkedList<Integer>();
        for (int i = 0; i < floats.length; i++) {
            float v = floats[i];
            for (int j = 0; j < probs.length; j++) {
                if ((v < 1.) ? (v < probs[j]) : (v == probs[j])) {
                    noteList.push(j);
                    break;
                }
            }
        }
        notes = new int[noteList.size()];
        for (int i = 0; i < notes.length; i++) {
            notes[i] = noteList.get(i);
        }
    }

    protected void bang() {
        outlet(0, notes);
    }
}