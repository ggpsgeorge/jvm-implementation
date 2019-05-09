package entidades;

public abstract class Pessoa {

    private String nome;
    private final int matricula = 12345;
    private double idade;
    public int serie;
    
    
    public Pessoa(){
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
    
    public double getIdadeVerdadeira(){
        
        return idade + 20;
    }
    
    public abstract int getMatricula();
}
