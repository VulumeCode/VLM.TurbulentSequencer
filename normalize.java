import com.cycling74.max.*;

public class normalize extends MaxObject {

    float[] input;

    normalize() {
        declareInlets(new int[] { DataTypes.LIST });
        declareOutlets(new int[] { DataTypes.LIST });
    }

    protected void list(final Atom[] a) {
        input = Atom.toFloat(a);
        bang();
    }

    protected void bang() {
        float sum = 0;
        float min = Float.POSITIVE_INFINITY;
        float max = Float.NEGATIVE_INFINITY;
        for (float f : input) {
            sum += f;
            if (f < min) {
                min = f;
            } else if (f > max) {
                max = f;
            }
        }
        float avg = sum / input.length;
        float deviation = Math.max(max - avg, avg - min);
        float[] output = new float[input.length];

        for (int i = 0; i < output.length; i++) {
            output[i] = (((input[i] - avg) / deviation) + 1.f) / 2.f;
        }

        outlet(0, output);
    }
}
