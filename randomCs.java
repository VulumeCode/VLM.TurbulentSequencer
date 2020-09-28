import com.cycling74.max.*;
import java.security.SecureRandom;
import java.util.Arrays;

public class randomCs extends MaxObject {

    int maxLength = 64;

    final int[] order = new int[maxLength];

    randomCs() {
        order[0] = 0;
        order[1] = maxLength - 1;
        int pos = 2;
        search: for (double denominator = 2; denominator < 100; denominator++) {
            for (double numerator = 1; numerator < denominator; numerator++) {
                double fraction = numerator / denominator;
                int number = (int) Math.round(fraction * maxLength);
                if (!Arrays.stream(order).anyMatch(i -> i == number)) {
                    order[pos++] = number;
                    if (pos == maxLength) {
                        break search;
                    }
                }
            }
        }

        declareInlets(new int[] { DataTypes.INT, DataTypes.INT });
        declareOutlets(new int[] { DataTypes.LIST, DataTypes.LIST });
    }

    int seed = 0;
    int take = 16;
    SecureRandom rng;

    float[] allAs = new float[maxLength];
    float[] allBs = new float[maxLength];

    protected void inlet(int b) {
        int idx = getInlet();
        switch (idx) {
            case 0:
                take = b;
                break;
            case 1:
                seed = b;
                regenerate();
                break;
        }
        bang();
    }

    void regenerate() {
        rng = new SecureRandom(new byte[] { (byte) (seed - 128) });
        for (int i = 0; i < maxLength; i++) {
            float a, b;
            do {
                a = 2f * (rng.nextFloat() - 0.5f);
                b = 2f * (rng.nextFloat() - 0.5f);
            } while (1 < Math.sqrt(a * a + b * b));
            allAs[i] = a;
            allBs[i] = b;
        }
    }

    protected void bang() {
        int[] indices = new int[take];
        System.arraycopy(order, 0, indices, 0, take);
        Arrays.sort(indices);

        float[] as = new float[take];
        float[] bs = new float[take];
        for (int i = 0; i < take; i++) {
            as[i] = allAs[indices[i]];
            bs[i] = allBs[indices[i]];
        }
        outlet(1, as);
        outlet(0, bs);
    }
}
