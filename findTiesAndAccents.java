import com.cycling74.max.*;

public class findTiesAndAccents extends MaxObject {
    Atom[] atoms = new Atom[0];
    String[] choices = new String[0];
    float bot = 0, top = 0;
    int nudge = 0;

    findTiesAndAccents() {
        declareInlets(new int[] { DataTypes.LIST, DataTypes.MESSAGE });
        declareOutlets(new int[] { DataTypes.MESSAGE });
    }

    protected void list(Atom[] a) {
        final int idx = getInlet();
        switch (idx) {
            case 0:
                atoms = a;
                bang();
                break;
        }
    }

    public void setChoices(Atom[] a) {
        choices = Atom.toString(a);
        bang();
    }

    public void setBot(float n) {
        bot = n;
        bang();
    }

    public void setTop(float n) {
        top = n;
        bang();
    }

    public void setNudge(int n) {
        nudge = n;
        bang();
    }

    protected void bang() {
        if (choices != null && choices.length > 0 && atoms != null && atoms.length > 0) {
            final float[] stages = new float[] { bot, 1 + top, 1 };
            final float[] floats = Atom.toFloat(Atom.rotate(atoms, nudge));

            int[] durations = new int[floats.length + 1];
            durations[0] = 1;
            int[] velocities = new int[floats.length + 1];
            velocities[0] = 1;
            int[] accents = new int[floats.length];

            for (int i = 0; i < floats.length; i++) {
                int j;
                float f = Math.nextDown(floats[i]);
                for (j = 0; j < stages.length; j++) {
                    if (f < stages[j]) {
                        break;
                    }
                }

                String choice = choices[j];

                velocities[i + 1] = choice.contains("R") ? 0 : choice.contains("A") ? 127 : 63;
                accents[i] = choice.contains("A") ? 1 : 0;
                durations[i + 1] = choice.contains("T") ? 120 : 60;
            }

            outlet(0, "duration", durations);
            outlet(0, "velocity", velocities);
            outlet(1, accents);
        }
    }
}
