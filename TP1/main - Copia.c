#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define LARGURA_DO_MUNDO 90
#define ALTURA_DO_MUNDO 160
#define Mov_lados 2.5;
float movanzol = 0;
FILE*log;
float sobefundo = 0;
float sobeobjeto = 0;
float movepeixe = 0;
float xViewport = 0;
float yViewport = 0;
int pausa = 0;
char pontosescreve[6] = {"0"};
int pontostotais = 0;
int zerar=0;
int descer = 0;
int subir = 1;
int direita = 0;
int reiniciar = 0;
int fechar = 0;
int tamanhosx[10] = {10,10,10,10,10,15,15,15,15,40};
int tamanhosy[10] = {7,7,7,7,7,20,20,10,10,30};
int pontuacao[10] = {250,250,250,250,250,-500,-500,-250,-250,-1000};
char tipos[][50]={"tubarao","peixe","agua_viva","baiacu"};
struct peixe{
    int tamanhox;
    int tamanhoy;
    int tipo;
    int posicaoinicialy;
    int pontos;
    int pego;
}peixe[50];

struct Cor{
    GLubyte r;
    GLubyte g;
    GLubyte b;
}cor;

void escreve(void* fonte, char* texto, float x, float y,float z) {
  glRasterPos3f(x, y, z);

  for (int i = 0; i < strlen(texto); i++) {
     glutBitmapCharacter(fonte, texto[i]);
  }
}

void criaPeixes(){
    srand(time(NULL));
    for(int i = 0;i<20;i++){
            int j = rand()%10;
                    glColor3f(1,1,1);
                    peixe[i].tamanhox = tamanhosx[j];
                    peixe[i].tamanhoy = tamanhosy[j];
                    peixe[i].tipo = j;
                    peixe[i].posicaoinicialy = (i*-90);
                    peixe[i].pontos = pontuacao[j];
                    peixe[i].pego = 0;
         }
}

/*int testeColisao(int i){
    anzolxmin = LARGURA_DO_MUNDO/2 - 5 + movanzol;
    anzolxmax = LARGURA_DO_MUNDO/2 + 2 + movanzol;
    if(anzolxmax>=LARGURA_DO_MUNDO+movepeixe&&
       anzolxmin<=LARGURA_DO_MUNDO+movepeixe+peixe[i].tamanhox&&
       anzolymax>=peixe[i].posicaoinicialy+sobeobjeto&&
       anzolymin<=peixe[i].posicaoinicialy+sobeobjeto+peixe[i].tamanhoy){
            return 1;
       }
   return 0;
}*/

int testeColisao(int x1max,int x1min,int y1max,int y1min,int x2max,int x2min,int y2max,int y2min){
    //fprintf(log," %d %d %d %d %d %d %f %f\n",x1max, y1min, x2max, x2min, y2max, y2min,xViewport,yViewport);
    if(x1max>=x2min&&x1min<=x2max&&
       y1max>=y2min&&y1min<=y2max){
            return 1;
       }
   return 0;
}

void desenhaAnzol(int x, int y,int z) {
  glPushMatrix();
    glTranslatef(x,y,0);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,0,z);
        glVertex3f(7,0,z);
        glVertex3f(7,7,z);
        glVertex3f(0,7,z);
    glEnd();
  glPopMatrix();
}

void desenhaRetangulo(int x,int y, int w,int h,int z){
    if(y+h>=0){
        glPushMatrix();
        glTranslatef(x,y,0);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0,0,z);
            glVertex3f(w,0,z);
            glVertex3f(w,h,z);
            glVertex3f(0,h,z);
        glEnd();
        glPopMatrix();
    }
}

void desenhaJogo(){
    glColor3f(0, 0, 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetangulo(0, 0, LARGURA_DO_MUNDO, ALTURA_DO_MUNDO+sobefundo,1);

    // desenha o anzol no meio e o movimenta por meio do movanzol
    glColor3f(1, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaAnzol(LARGURA_DO_MUNDO/2 - 5 + movanzol, ALTURA_DO_MUNDO/2- 5,7);

    // desenha "chao" de areia
    glColor3f(0,0,0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetangulo(0,-1600+sobeobjeto,LARGURA_DO_MUNDO,100,10);

    //desenha caixa da pontuaçao e pontuacao
    glColor3f(0,1,0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetangulo(LARGURA_DO_MUNDO-15,ALTURA_DO_MUNDO-10,15,10,9);
    glColor3f(1,1,1);
    escreve(GLUT_BITMAP_HELVETICA_18, pontosescreve,LARGURA_DO_MUNDO-12 ,ALTURA_DO_MUNDO-5 ,10);

    //desenha ceu
    glColor3f(0,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetangulo(0,130+sobefundo,LARGURA_DO_MUNDO,160,5);

    //desenha barco e pescador
    glColor3f(1,1,0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetangulo(LARGURA_DO_MUNDO/2-10,130+sobefundo,20,10,10);


    //desenha peixes
    for(int i=0; i<20;i++){
       if(testeColisao(47 + movanzol,40+ movanzol,82,75,
                       90+movepeixe+peixe[i].tamanhox,90+movepeixe,
                       peixe[i].posicaoinicialy+sobeobjeto+peixe[i].tamanhoy,
                        peixe[i].posicaoinicialy+sobeobjeto)==1&&peixe[i].pego==0){
                peixe[i].pego = 1;
                pontostotais += peixe[i].pontos;
                if(pontostotais<0){
                    pontostotais = 0;
                }
                sprintf(pontosescreve, "%i", pontostotais);
           }
        switch(peixe[i].tipo){
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            glColor3f(1,0,1);//peixe amigo
            break;
        case 5:
        case 6:
            glColor3f(0,0,0);//agua viva
            break;
        case 7:
        case 8:
            glColor3f(0.6,0.6,0.6);//baiacu
            break;
        case 9:
            glColor3f(1,1,1);//tubarao
            break;
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if(peixe[i].pego==0||peixe[i].pontos<0){
            desenhaRetangulo(LARGURA_DO_MUNDO+movepeixe,peixe[i].posicaoinicialy+sobeobjeto,
                            peixe[i].tamanhox,peixe[i].tamanhoy,6);
        }
        else if(peixe[i].pego==1){
            desenhaRetangulo(LARGURA_DO_MUNDO/2+movanzol,ALTURA_DO_MUNDO/2-5,
                            peixe[i].tamanhox,peixe[i].tamanhoy,6);
        }
    }
}

void desenhaCorfirmacao(){
    glColor3f(0,0,0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetangulo(5,60,80,40,8);
    //glColor3f(1,1,0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //desenhaRetangulo(15,70,20,10,9);
    //glColor3f(1,1,1);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //desenhaRetangulo(55,70,20,10,9);
    glColor3f(0,1,0);
    escreve(GLUT_BITMAP_HELVETICA_18, "Aperte o mesmo botao ",10 ,90 ,10);
    escreve(GLUT_BITMAP_HELVETICA_18, "para confirmar a acao",10 ,85 ,10);
    escreve(GLUT_BITMAP_HELVETICA_18, "aperte n para cancelar",10 ,80 ,10);
}

// Rotina de desenho
void desenhaMinhaCena(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    desenhaJogo();
    if(reiniciar == 1 || fechar == 1){
            desenhaCorfirmacao();
    }
    glutSwapBuffers();
}

void atualiza(){
    if(pausa==0){
        glutPostRedisplay();
        //movimenta os peixes para direita
        if(movepeixe==-LARGURA_DO_MUNDO||direita==1){
            movepeixe += 1;
            direita = 1;
            if(movepeixe>=0){
                direita = 0;
            }
        }//movimenta os peixes para esquerda
        else{
            movepeixe -= 1;
        }
        //sobe com anzol mais rapido dq a descida
        if(sobeobjeto>=LARGURA_DO_MUNDO/2-3.5+1500||descer==1){
            for(int i=0;i<20&&zerar==0;i++){
                if(peixe[i].pontos<0){
                    peixe[i].pego=0;
                }
            }
            zerar=1;
            sobeobjeto -= 3;
            sobefundo -= 2.5;
            descer=1;
            if(sobefundo<-45){
                descer=0;
                subir=0;
            }
        }
        //desce o anzol
        else if(subir==1){
            sobefundo += 1.25;
            sobeobjeto += 1.5;
        }
        glutSwapBuffers();
        glutTimerFunc(33, atualiza, 10);
    }
}

void reinicia(){
    criaPeixes();
    glutPostRedisplay();
    pontostotais = 0;
    sprintf(pontosescreve, "%i", pontostotais);
    sobefundo = 0;
    sobeobjeto = 0;
    movanzol = 0;
    pontostotais = 0;
    descer = 0;
    subir = 1;
    zerar = 0;
    pausa = 0;
    reiniciar = 0;
    desenhaMinhaCena();
    atualiza();
    glutSwapBuffers();
}

void setup() {
    glClearColor(1, 1, 1, 0);
}

// mantendo o aspecto
void redimensionada(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARGURA_DO_MUNDO, 0, ALTURA_DO_MUNDO, -10, 10);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float) LARGURA_DO_MUNDO)/ ALTURA_DO_MUNDO;
    if (razaoAspectoJanela < razaoAspectoMundo) {
        float hViewport = width / razaoAspectoMundo;
        yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        float wViewport = ((float)height) * razaoAspectoMundo;
        xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y){

    //sair
    if (key == 27) {
      if(fechar == 0){
        pausa = 1;
        fechar = 1;
        desenhaMinhaCena();
      }
      else if(fechar == 1){
        exit(0);
      }
    }
    //pause
    if(key == 'p'){
        if(pausa==0)
            pausa = 1;
        else{
            pausa = 0;
            atualiza();
        }

    }
    //reiniciar
    if(key == 'r'){
        if(reiniciar == 0){
        reiniciar  = 1;
        pausa = 1;
        desenhaMinhaCena();
        }
        else{
            reinicia();
            reiniciar = 0;
        }
    }
    if(key == 'n'){
        fechar = 0;
        reiniciar = 0;
        pausa = 0;
        atualiza();
    }
}

void teclasEspeciais(int key, int x, int y){
    // movimenta o anzol para direita
    if (key == GLUT_KEY_RIGHT&&pausa==0){
        if(movanzol<45){
           movanzol += Mov_lados;
        }
        glutPostRedisplay();
        desenhaMinhaCena();
        glutSwapBuffers();
    }
    // movimenta o anzol para esquerda
    if(key == GLUT_KEY_LEFT&&pausa==0){
        if(movanzol>-45){
            movanzol -= Mov_lados;
        }
        glutPostRedisplay();
        desenhaMinhaCena();
        glutSwapBuffers();
    }
}

void GerenciaMouse(int button, int state, int x_i, int y_i){
    float x = x_i;
    float y = y_i;
    float x0 = xViewport*90/450;
    float y0 = yViewport*160/800;
    //fprintf(log,"%f %f\n",x,y);
   // y = 800 - y;
    //y = y*160/800;
    //x = x*90/450;

   // fprintf(log,"%f %f\n",x,y);
    //x -= x0;
    //y -= y0;
    //y = yViewport;
    //fprintf(log,"mouse %d %d %d %f %f\n",button,x+xViewport,y+yViewport,xViewport,yViewport);
    unsigned char cor2[3];
    glReadPixels(45,45,1,1,GL_RGB,GL_UNSIGNED_BYTE,cor2);
    fprintf(log,"%d %d %d\n",cor2[0],cor2[1],cor2[2]);
    if (button == GLUT_LEFT_BUTTON && cor2[0] == 1 && cor2[1] == 1 && cor2[2] == 0){
        if(reiniciar == 1){
            reinicia();
        }
        if(fechar == 1){
            exit(0);
        }
    }
    else if(button == GLUT_LEFT_BUTTON && testeColisao(x,x,y,y,75,55,80,70) == 1){
        reiniciar = 0;
        fechar = 0;
        pausa = 0;
        atualiza();
    }
}

// Função principal
int main(int argc, char** argv){
    log=fopen("peixes.log","w");
    setbuf(log,NULL);
    criaPeixes();

    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(450, 800);

    glutCreateWindow("Jogo");
    //init();
    // Registra callbacks para eventos
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);
    glutSpecialFunc(teclasEspeciais);
    glutMouseFunc(GerenciaMouse);
    atualiza();

    setup();

    glutMainLoop();
    glDisable(GL_DEPTH_TEST);
    fclose(log);
    return 0;
}
