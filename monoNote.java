import com.cycling74.max.*;

public class monoNote extends MaxObject {
    int lastNote = -1;
    boolean lastTie = false;
    float velScale = 1.f;

    monoNote() {
        declareInlets(new int[] { DataTypes.LIST, DataTypes.INT });
        declareOutlets(new int[] { DataTypes.LIST });
    }

    protected void inlet(int a) {
        velScale = ((float) a) / (127.f);
    }

    protected void list(Atom[] a) {
        int note = a[1].getInt();
        int vel = a[2].getInt();
        float duration = a[3].getFloat();

        boolean tie = duration == 120;
        boolean rest = vel == 0;

        int outVel = (int) (vel * velScale);

        if (lastNote == -1 && !rest) {
            outlet(0, new int[] { note, outVel });
        } else if (lastTie && !rest && lastNote != note) {
            outlet(0, new int[] { note, outVel });
            outlet(0, new int[] { lastNote, 0 });
        } else if (lastNote != -1 && rest) {
            outlet(0, new int[] { lastNote, 0 });
        } else if (lastNote != -1 && !lastTie) {
            outlet(0, new int[] { lastNote, 0 });
            outlet(0, new int[] { note, outVel });
        }

        lastNote = rest ? -1 : note;
        lastTie = tie;
    }
}