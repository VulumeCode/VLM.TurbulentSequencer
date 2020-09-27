import com.cycling74.max.*;

public class monoNote extends MaxObject {
    int lastNote = -1;
    boolean lastTie = false;

    public void list(Atom[] a) {
        int note = a[1].getInt();
        int vel = a[2].getInt();
        float duration = a[3].getFloat();

        boolean tie = duration == 120;
        boolean rest = vel == 0;

        if (lastNote == -1 && !rest) {
            outlet(0, new int[] { note, vel });
        } else if (lastTie && !rest && lastNote != note) {
            outlet(0, new int[] { note, vel });
            outlet(0, new int[] { lastNote, 0 });
        } else if (lastNote != -1 && rest) {
            outlet(0, new int[] { lastNote, 0 });
        } else if (lastNote != -1 && !lastTie) {
            outlet(0, new int[] { lastNote, 0 });
            outlet(0, new int[] { note, vel });
        }

        lastNote = rest ? -1 : note;
        lastTie = tie;
    }
}