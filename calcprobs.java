import java.util.LinkedList;

import com.cycling74.max.*;

public class calcprobs extends MaxObject {

    float[] octProbs = new float[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    float[] noteProbs = new float[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    float[] noteToggles = new float[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    calcprobs() {
        declareInlets(new int[] { DataTypes.LIST, DataTypes.LIST, DataTypes.LIST });
        declareOutlets(new int[] { DataTypes.LIST, DataTypes.LIST });
    }

    protected void list(final Atom[] a) {
        final int idx = getInlet();
        switch (idx) {
            case 0:
                octProbs = Atom.toFloat(Atom.reverse(a));
                break;
            case 1:
                noteProbs = Atom.toFloat(a);
                break;
            case 2:
                noteToggles = Atom.toFloat(a);
                break;
        }
        bang();
    }

    protected void bang() {
        final LinkedList<Integer> usedList = new LinkedList<Integer>();
        int note = 0;
        final LinkedList<Float> accumProbsList = new LinkedList<Float>();
        float accum = 0;
        for (int i = 0; i < octProbs.length; i++) {
            for (int j = 0; j < noteProbs.length; j++) {
                final float prob = octProbs[i] * noteProbs[j] * noteToggles[j];
                if (prob > 0) {
                    usedList.add(note);
                }
                accum += prob;
                accumProbsList.add(accum);
                note++;
            }
        }

        int[] used = new int[usedList.size()];
        for (int i = 0; i < used.length; i++) {
            used[i] = usedList.get(i);
        }
        outlet(0, used);

        final float[] accumProbs = new float[accumProbsList.size()];
        for (int i = 0; i < accumProbs.length; i++) {
            accumProbs[i] = accumProbsList.get(i) / accum;
        }
        outlet(1, accumProbs);
    }
}
