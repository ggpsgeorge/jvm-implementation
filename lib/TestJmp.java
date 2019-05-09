
public class TestJmp {
  public static void main(String[] args) {
    // Teste goto_
    System.out.println("======= Teste GOTO");
    int i = 0;
    while(i<10){
      System.out.println(i);
      i++;
    }
    for (i=0;i<10;i++) {
      System.out.println(i);
    }
    System.out.println("Goto funcionando");

    // Teste jsr e ret
    System.out.println("\n======= Teste jsr e ret");
    i = testeJsr(true);
    i=2;
    // Teste tableswitch
    System.out.println("\n======= Teste Tabbleswitch");
    switch (i) {
      case 1:
        System.out.println("1 "+i);
        break;
      case 2:
        System.out.println("2 "+i);
        break;
      case 3:
        System.out.println("3 "+i);
        break;
      case 4:
        System.out.println("4 "+i);
        break;
      default:
        System.out.println("default "+i);
        break;
    }

  }

  static int testeJsr(boolean valor) {
    try {
      if (valor) {
        System.out.println("Retorno correto");
        return 1;
      }
      System.out.println("Retorno errado");
      return 0;
    } finally {
      System.out.println("Jsr e ret funcionando");
    }
  }


}
