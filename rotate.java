import com.cycling74.max.*;

public class rotate extends MaxObject {

    float[] seqReal, seqImg;
    float angle;

    rotate() {
        declareInlets(new int[] { DataTypes.LIST, DataTypes.LIST, DataTypes.FLOAT });
        declareOutlets(new int[] { DataTypes.LIST, DataTypes.LIST });
    }

    protected void inlet(float v) {
        final int idx = getInlet();
        switch (idx) {
            case 2:
                angle = v;
                bang();
                break;
        }
    }

    protected void list(Atom[] a) {
        final int idx = getInlet();
        switch (idx) {
            case 0:
                seqReal = Atom.toFloat(a);
                bang();
                break;
            case 1:
                seqImg = Atom.toFloat(a);
                break;
        }
    }

    private class CarToPol {
        float t, r;

        CarToPol(float x, float y) {
            t = (float) Math.atan2(y, x);
            r = (float) Math.sqrt((x * x) + (y * y));
        }
    }

    private class PolToCar {
        float x, y;

        PolToCar(float t, float r) {
            x = (float) (r * Math.cos(t));
            y = (float) (r * Math.sin(t));
        }
    }

    protected void bang() {
        int len = seqReal.length;
        float[] outSeqReal = new float[len];
        float[] outSeqImg = new float[len];

        for (int i = 0; i < len; i++) {
            CarToPol pol = new CarToPol(seqReal[i], seqImg[i]);
            PolToCar car = new PolToCar(pol.t - angle, pol.r);
            outSeqReal[i] = car.x;
            outSeqImg[i] = car.y;
        }

        outlet(1, outSeqImg);
        outlet(0, outSeqReal);
    }

}
