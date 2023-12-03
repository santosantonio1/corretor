/********************************************************
 *              CORRETOR DE EXERCICIOS EM C             *
 *                                                      *        
 * Construído por @Antônio dos Santos                   *
 * Versão 12/08/2023                                    *
 * Esse programa testa exercícios em .c na pasta Aluno, *
 * utilizando inputs da pasta input e compara com as    *
 * respostas na pasta output. No final do processo, a   *
 * pontuação é salva em overall.txt e também é impressa *
 * na tela.                                             *
 ********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main() {

    printf("\n*******************************************\n");

    DIR *aluno = opendir("Aluno");
    struct dirent *prog_aluno;
    readdir(aluno);
    readdir(aluno);

    FILE *overall = fopen("overall.txt", "w+");

    int acertos = 0;

    DIR *local = opendir("input");
    struct dirent *dp;

    readdir(local);
    readdir(local);
    dp =  readdir(local);
    
    while(prog_aluno = readdir(aluno)) {

        char num_exercicio[3];

             num_exercicio[0] = prog_aluno->d_name[9];
             num_exercicio[1] = prog_aluno->d_name[10];
             num_exercicio[2] = prog_aluno->d_name[11];

        acertos = 0;
        char guarda_nome_prog[40] = "Aluno\\";
        strcat(guarda_nome_prog, prog_aluno->d_name);
        rename(guarda_nome_prog, "Aluno\\prog.c");
        system("gcc -o testaInputC Aluno\\prog.c");
        rename("Aluno\\prog.c", guarda_nome_prog);

        int qts_testes = 0;
        
    while(dp) {
            
               if(!(dp->d_name[9]  == num_exercicio[0] &&
                    dp->d_name[10] == num_exercicio[1] &&
                    dp->d_name[11] == num_exercicio[2]))

                {    
                    break;
                }

            qts_testes++;

            char local_input[100]  = "input\\";
            char local_output[100] = "output\\";
            strcat(local_input, dp->d_name);
            strcat(local_output, dp->d_name);

            rename(local_input, "input\\input" );
            rename(local_output, "output\\gabarito" );

            system("testaInputC < input\\input > output.txt");
            
            int igual = 1;

                FILE *output   = fopen("output.txt", "r");
                FILE *gabarito = fopen("output\\gabarito", "r");


            while(igual) {
                
                char c_output, c_gabarito;

                int r_output   = fscanf(output, "%c ", &c_output), 
                    r_gabarito = fscanf(gabarito, "%c ", &c_gabarito);
               
                if((c_output != c_gabarito) && r_output == 1 && r_gabarito == 1) {
                    igual = 0;
                }

                if(r_output != 1 || r_gabarito != 1) {
                    if(r_output != 1 && r_gabarito != 1) 
                            break;
                    else {
                        igual = 0;
                        if(r_output != 1 && r_gabarito == 1) {
                            if (c_gabarito == '\n')
                            igual = 1;
                        }
                        if(r_output == 1 && r_gabarito != 1) {
                            if (c_output == '\n')
                            igual = 1;
                        }
                        break;
                    }
                }
            }

            if (igual) 
                acertos++;
            
            FILE *score = fopen("score.txt", "w+");
            fprintf(score, "%d", acertos);

            fclose(score);
            fclose(output);
            fclose(gabarito);

            rename("input\\input", local_input );
            rename("output\\gabarito", local_output );

                dp = readdir(local);

        }
        
        FILE *score = fopen("score.txt","r");
        fscanf(score, "%d", &acertos);
        fprintf(overall, "%s: %3d/%3d    (%.2f%%)\n", prog_aluno->d_name, acertos, qts_testes, (float)acertos/qts_testes*100);

        printf("%s: %3d/%3d    (%.2f%%)\n", prog_aluno->d_name, acertos, qts_testes, (float)acertos/qts_testes*100);
        fclose(score);
    }   

        fclose(overall);

        closedir(local);
        closedir(aluno);
    
    system("del score.txt");
    system("del output.txt");
    system("del testaInputC.exe");

    printf("\n*******************************************\n");


    return 0;
}