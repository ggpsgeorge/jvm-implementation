import entidades.Aluno;
import entidades.Matematica;
import entidades.Calc;

public class HelloWorld {

    public static void main(String[] args) {
//        // Prints "Hello, World" to the terminal window.
//        
//        String[] string = {"VACA", "CACHORRO", "BEZERRO"};
//
//        System.out.println("Nome do aluno  : " + string[0]);
        
        Aluno aluno = new Aluno(8);
//        
        aluno.setIdade(53.32);
//        aluno.setNome("Rondinele");
//        
//        System.out.println("Nome do aluno  : " + aluno.getNome());
        System.out.println("Idade do aluno : " + aluno.getIdade());
//        System.out.println("Idade verdadeira do aluno : " + aluno.getIdadeVerdadeira());
//        System.out.println("Matricula : " + aluno.getMatricula());
        System.out.println("Serie : " + aluno.serie);
        

//        Matematica m;
//        
//        m= new Calc();
//        
//        System.out.println("Soma : " + m.soma(2,3));
//        System.out.println("PI : " + m.pi);
        
        
    }
}
