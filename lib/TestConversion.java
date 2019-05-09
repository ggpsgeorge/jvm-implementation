
public class TestConversion {
  public static void main(String[] args) {
    int ia = 1,ib = 1;
    long la = 1,lb = 1;
    double da = 1.1, db = 1;
    float fa = (float)1.1, fb = 1;
    char ca = 1, cb = 1;
    byte ba = 1, bb = 1;
    short sa = 1, sb = 1;

    lb = (long) ia;
    fb = (float)ia;
    db = (double)ia;
    bb = (byte) ia;
    cb = (char) ia;
    sb = (short) ia;

    System.out.println("======= Int - Valor Inicial: "+ia);
    System.out.println("Long: "+lb);
    System.out.println("Double: "+db);
    System.out.println("Float: "+fb);
    System.out.println("Byte: "+bb);
    System.out.println("Char: "+cb);
    System.out.println("Short: "+sb);

    ib = (int) la;
    fb = (float) la;
    db = (double)la;

    System.out.println("\n======= Long - Valor Inicial: "+la);
    System.out.println("Int: "+ib);
    System.out.println("Float: "+fb);
    System.out.println("Double: "+db);

    ib = (int) fa;
    lb = (long) fa;
    db = (double)fa;

    System.out.println("\n======= Float - Valor Inicial: "+fa);
    System.out.println("Int: "+ib);
    System.out.println("Long: "+lb);
    System.out.println("Double: "+db);

    ib = (int) da;
    lb = (long) da;
    fb = (float)da;

    System.out.println("\n======= Double - Valor Inicial: "+da);
    System.out.println("Int: "+ib);
    System.out.println("Long: "+lb);
    System.out.println("Float: "+fb);
  }
}
