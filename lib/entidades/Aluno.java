package entidades;

public class Aluno extends Pessoa {

    private String nome;
    private final int matricula = 12345;
    private double idade;
    public long serie;
    
    
    public Aluno(int serie){
        this.serie = serie;
    }
    
    public void setIdade(double idade){
        
        this.idade = idade;
    }
    
    public void setNome(String string){
        
        this.nome = string;
    }
    
    public String getNome(){
        
        return this.nome;
    }
    
    public double getIdade(){
        
        return this.idade;
    }
    
    public int getMatricula(){
        
        return this.matricula;
    }
}
