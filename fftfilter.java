import com.cycling74.max.*;

public class fftfilter extends MaxObject {
    float[] seqReal = new float[] { 0, 0 };
    float[] seqImg = new float[] { 0, 0 };
    float[] curve = new float[] { 0, 0 };

    ComplexFloatFFT_Mixed fft;

    int prevLen = -1;

    fftfilter() {
        new Factorize();
        new ComplexFloatFFT();
        new ComplexFloatFFT_Mixed();

        declareInlets(new int[] { DataTypes.LIST, DataTypes.LIST, DataTypes.LIST });
        declareOutlets(new int[] { DataTypes.LIST, DataTypes.LIST, DataTypes.LIST, DataTypes.LIST });
    }

    protected void list(Atom[] a) {
        int idx = getInlet();
        switch (idx) {
            case 0:
                seqReal = Atom.toFloat(a);
                bang();
                break;
            case 1:
                seqImg = Atom.toFloat(a);
                break;
            case 2:
                curve = Atom.toFloat(a);
                bang();
                break;
        }
    }

    protected void bang() {
        if (seqReal.length != curve.length && seqImg.length != curve.length) {
            return;
        }

        int len = seqReal.length;
        if (len == 1) {
            outlet(3, seqImg);
            outlet(2, seqReal);
            outlet(1, seqImg);
            outlet(0, seqReal);
        } else if (len > 1) {
            if (len != prevLen) {
                fft = new ComplexFloatFFT_Mixed(len);
                prevLen = len;
            }

            float[] input = new float[len * len];
            for (int i = 0, x = 0; i < len; i++) {
                input[x] = seqReal[i];
                x++;
                input[x] = seqImg[i];
                x++;
            }

            fft.transform(input);

            float[] outSeqFreqImg = new float[len];
            float[] outSeqFreqReal = new float[len];

            float[] output = new float[len * len];
            float v;
            for (int i = 0, x = 0; i < len; i++) {
                v = input[x] * curve[i];
                outSeqFreqReal[i] = v / len;
                output[x] = v;
                x++;

                v = input[x] * curve[i];
                outSeqFreqImg[i] = v / len;
                output[x] = v;
                x++;
            }

            fft.backtransform(output);

            float[] outSeqImg = new float[len];
            float[] outSeqReal = new float[len];
            for (int i = 0, x = 0; i < len; i++) {
                outSeqReal[i] = output[x++] / len;
                outSeqImg[i] = output[x++] / len;
            }

            outlet(3, outSeqFreqImg);
            outlet(2, outSeqFreqReal);
            outlet(1, outSeqImg);
            outlet(0, outSeqReal);
        }
    }
}
