import com.cycling74.max.*;

public class curve extends MaxObject {
    float beta = 1;
    float center = 1;
    int len = 2;

    curve() {
        declareInlets(new int[] { DataTypes.FLOAT, DataTypes.ALL, DataTypes.INT });
        declareOutlets(new int[] { DataTypes.LIST });
    }

    protected void inlet(float v) {
        final int idx = getInlet();
        switch (idx) {
            case 0:
                beta = v;
                break;
            case 1:
                center = v * len;
                break;
        }
        bang();
    }

    protected void inlet(int v) {
        final int idx = getInlet();
        switch (idx) {
            case 0:
                beta = v;
                break;
            case 1:
                center = v;
                break;
            case 2:
                len = v;
                break;
        }
        bang();
    }

    protected void bang() {
        float[] curve = new float[len];
        float max = 0;

        int i = 1;
        for (; i < center; i++) {
            float e = len * ((center - i) / center);
            float s = (float) (1. / Math.pow(e, beta));
            max = s > max ? s : max;
            curve[i] = s;
        }

        for (; i < len; i++) {
            float e = i - center + 1;
            float s = (float) (1. / Math.pow(e, beta));
            max = s > max ? s : max;
            curve[i] = s;
        }

        for (int j = 1; j < curve.length; j++) {
            curve[j] = curve[j] / max;
        }

        curve[0] = 1;

        outlet(0, curve);
    }
}
