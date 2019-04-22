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
FILE *log;
GLuint texturaanzol;
GLuint texturafundo;
GLuint texturapescador;
GLuint texturapescador2;
GLuint texturapescador3;
GLuint texturapeixedir[6];
GLuint texturapeixeesq[6];
GLuint texturatubaraodir[6];
GLuint texturatubaraoesq[6];
GLuint texturaaguaviva[9];
GLuint texturabaracudaesq[8];
GLuint texturabaracudadir[8];
GLuint texturaceu;
GLuint texturapeixe;
GLuint texturacaixagrande;
GLuint texturacaixapequena;
GLuint texturachao;
GLuint texturamira;
GLuint texturacaixapont;
int postexturapeixe = 0;
int postexturaaguaviva = 0;
int postexturabaracuda = 0;
float sobefundo = 0;
float sobeobjeto = 0;
float movepeixe = 0;
float xViewport = 0;
float yViewport = 0;
float wViewport = 450;
float hViewport = 800;
float xmouse;
float ymouse;
int pausa = 1;
char pontosescreve[6] = {"0"};
char pontosescreve2[6] = {"0"};
int pontostotais = 0;
int pontosparte2 = 0;
int numpeixes2 = 0;
int zerar=0;
int descer = 0;
int subir = 1;
int direita = 0;
int angulo = 0;
int reiniciar = 0;
int fechar = 0;
int telainicial = 1;
int creditos = 0;
int telafinal = 0;
int parte1 = 0;
int parte2 = 0;
int quedaparte2 = 0;
int fimdecreditos = 0;
int fimdetelafinal = 0;
int tamanhosx[10] = {10,10,10,10,10,15,15,40,40,40};
int tamanhosy[10] = {7,7,7,7,7,20,20,10,10,30};
int pontuacao[10] = {250,250,250,250,250,-500,-500,-250,-250,-1000};
char tipos[][50]={"tubarao","peixe","agua_viva","baracuda"};
struct peixe{
    int tamanhox;
    int tamanhoy;
    int tipo;
    int posicaoinicialy;
    int pontos;
    int pego;
}peixe[30];
struct fase2peixe{
    int posicaoinicialy;
    int posicaoinicialx;
    int tiro;
}peixe2[20];

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
                    peixe[i].tamanhox = tamanhosx[j];
                    peixe[i].tamanhoy = tamanhosy[j];
                    peixe[i].tipo = j;
                    peixe[i].posicaoinicialy = (i*-90);
                    peixe[i].pontos = pontuacao[j];
                    peixe[i].pego = 0;
         }
}

void criaPeixes2(){
    for(int i = 0;i < numpeixes2; i++){
                    peixe2[i].posicaoinicialx = rand()%80;
                    peixe2[i].posicaoinicialy = 160+rand()%200;
                    peixe2[i].tiro = 0;
         }
}

int testeColisao(int x1max,int x1min,int y1max,int y1min,int x2max,int x2min,int y2max,int y2min){
    //fprintf(log,"%d %d %d %d %d %d\n",x1min, y1min, x2min, x2max, y2min, y2max);
    if(x1max>=x2min&&x1min<=x2max&&
       y1max>=y2min&&y1min<=y2max){
            return 1;
       }
   return 0;
}

GLuint carregaTextura(const char* arquivo){
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
    return idTextura;
}

void desenhaAnzol(int x, int y, int z, GLuint textura) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0); glVertex3f(0,0,z);
    glTexCoord2f(1, 0); glVertex3f(7,0,z);
    glTexCoord2f(1, 1); glVertex3f(7,7,z);
    glTexCoord2f(0, 1); glVertex3f(0,7,z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void desenhaRetangulo(int x,int y, float w,int h,int z){
    if(y+h>=0){
        //glEnable(GL_TEXTURE_2D);
        //glBindTexture(GL_TEXTURE_2D, textura);
        glPushMatrix();
        glTranslatef(x,y,0);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0,0,z);
            glVertex3f(w,0,z);
            glVertex3f(w,h,z);
            glVertex3f(0,h,z);
        glEnd();
        //glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void desenhaRetanguloTxt(int x,int y, int w,int h,int z,GLuint textura){
    if(y+h>=0){
        glPushMatrix();
        glTranslatef(x,y,0);
        glRotated(angulo,0,0,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textura);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, 0); glVertex3f(0,0,z);
            glTexCoord2f(1, 0); glVertex3f(w,0,z);
            glTexCoord2f(1, 1); glVertex3f(w,h,z);
            glTexCoord2f(0, 1); glVertex3f(0,h,z);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void desenhaJogo(){
    glColor4f(1, 1, 1, 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, 0+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -160+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -320+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -480+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -640+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -800+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -960+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -1120+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -1280+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -1440+sobefundo, 90, 160, 5,texturafundo);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0, -1600+sobefundo, 90, 160, 5,texturafundo);

    // desenha o anzol e linha no meio e o movimenta por meio do movanzol
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    desenhaRetangulo(LARGURA_DO_MUNDO/2 - 2 + movanzol,ALTURA_DO_MUNDO/2+2,0.1,90,7);
    glColor3f(0.6, 0.6, 0.6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaAnzol(LARGURA_DO_MUNDO/2 - 5 + movanzol, ALTURA_DO_MUNDO/2- 5,7,texturaanzol);

    // desenha "chao" de areia
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0,-1550+sobeobjeto,LARGURA_DO_MUNDO,80,10,texturachao);

    //desenha caixa da pontuaçao e pontuacao
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(75,150,15,10,9,texturacaixapont);
    glColor3f(1,1,1);
    escreve(GLUT_BITMAP_HELVETICA_18, pontosescreve,LARGURA_DO_MUNDO-12 ,ALTURA_DO_MUNDO-5 ,10);

    //desenha ceu
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0,130+sobefundo,LARGURA_DO_MUNDO,160,8,texturaceu);

    //desenha barco e pescador
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if(descer == 1){
        desenhaRetanguloTxt(15,130+sobefundo,60,30,10,texturapescador);
    }
    else{
        desenhaRetanguloTxt(15,130+sobefundo,60,30,10,texturapescador2);

    }

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
                    glColor3f(1,1,1);//peixe amigo 1
                    if(direita == 1 && peixe[i].pego == 0 ){
                        texturapeixe = texturapeixedir[postexturapeixe];
                    }
                    else{
                        texturapeixe = texturapeixeesq[postexturapeixe];
                    }
                    break;
                case 5:
                case 6:
                    glColor3f(1,1,1);//agua viva
                    texturapeixe = texturaaguaviva[postexturaaguaviva];
                    break;
                case 7:
                case 8:
                    glColor3f(1,1,1);//barracuda
                    if(direita == 1){
                        texturapeixe = texturabaracudadir[postexturabaracuda];
                    }
                    else{
                        texturapeixe = texturabaracudaesq[postexturabaracuda];
                    }
                    break;
                case 9:
                    glColor3f(1,1,1);//tubarao
                    if(direita == 1){
                        texturapeixe = texturatubaraodir[postexturapeixe];
                    }
                    else{
                        texturapeixe = texturatubaraoesq[postexturapeixe];
                    }
                    break;
                }
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                if(peixe[i].pego == 0||peixe[i].pontos<0){
                    angulo = 0;
                    desenhaRetanguloTxt(LARGURA_DO_MUNDO+movepeixe,peixe[i].posicaoinicialy+sobeobjeto,
                                    peixe[i].tamanhox,peixe[i].tamanhoy,6,texturapeixe);
                }
                else if(peixe[i].pego == 1){
                    angulo = -90;
                    desenhaRetanguloTxt(45+movanzol-3.5,75,
                                    peixe[i].tamanhox,peixe[i].tamanhoy,6,texturapeixe);
                }
            }
}

void desenha2fase(){
    //desenha ceu
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0,20,LARGURA_DO_MUNDO,160,6,texturaceu);
    //desenha barco e atirador
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(15,20,60,30,10,texturapescador3);
    //desenha agua
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(0,-140,90,160,8,texturafundo);
    //desenha mira
    if(pausa == 0){
        glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(xmouse-5,ymouse-5,10,10,10,texturamira);
    }
    for(int i = 0; i < numpeixes2; i++){
        glColor3f(1,1,1);
        texturapeixe = texturapeixeesq[0];
        if(peixe2[i].tiro == 0){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            desenhaRetanguloTxt(peixe2[i].posicaoinicialx-5,peixe2[i].posicaoinicialy-3.5+quedaparte2,
                                10,7,7,texturapeixe);
            if(testeColisao(xmouse+5,xmouse-5,ymouse+5,ymouse-5,
                         peixe2[i].posicaoinicialx+5,peixe2[i].posicaoinicialx-5,
                         peixe2[i].posicaoinicialy+3.5+quedaparte2,peixe2[i].posicaoinicialy-3.5+quedaparte2) == 1){
                                peixe2[i].tiro = 1;
                                pontosparte2 += 250;
                                sprintf(pontosescreve2,"%d",pontosparte2);
            }
        }
    }
    //desenha caixa da pontuaçao e pontuacao
    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(75,150,15,10,9,texturacaixapont);
    glColor3f(1,1,1);
    escreve(GLUT_BITMAP_HELVETICA_18, pontosescreve2,LARGURA_DO_MUNDO-12 ,ALTURA_DO_MUNDO-5 ,10);
}

void desenhaTelafinal(){
    glColor3f(0,1,0);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24, "PARABENS VOCE FEZ",25 ,80 ,10);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24, pontosescreve2,28 ,70 ,10);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24, "pontos",45 ,70 ,10);
}

void desenhaCorfirmacao(){
    glColor3f(0.63,0.32,0.17);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(5,60,80,40,8,texturacaixagrande);
    glColor3f(0.95,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(15,70,20,10,9,texturacaixapequena);
    glColor3f(0.95,1,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetanguloTxt(55,70,20,10,9,texturacaixapequena);
    glColor3f(0,1,0);
    escreve(GLUT_BITMAP_HELVETICA_18, "Tem certeza? ",35 ,90 ,10);
    escreve(GLUT_BITMAP_HELVETICA_18, "Sim",20 ,73 ,10);
    escreve(GLUT_BITMAP_HELVETICA_18, "Nao",60 ,73 ,10);
}

void desenhaTelainicial(){
    glColor3f(0,1,0);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Pescaria: o jogo",30,140,7);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Iniciar",38,108,7);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Creditos",38,78,7);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Fechar",38,48,7);
    glColor3f(1,0,0);
    desenhaRetanguloTxt(25,100,40,20,6,texturacaixagrande);
    desenhaRetanguloTxt(25,70,40,20,6,texturacaixagrande);
    desenhaRetanguloTxt(25,40,40,20,6,texturacaixagrande);
}

void desenhaCreditos(){
    glColor3f(1,1,1);
    escreve(GLUT_BITMAP_HELVETICA_18,"Direcao de Desnvolvimento",28,130,10);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Bruno Xavier",28,125,10);
    escreve(GLUT_BITMAP_HELVETICA_18,"Testes",28,115,10);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Bruno Xavier",28,110,10);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Ricardo Xavier",28,105,10);
    escreve(GLUT_BITMAP_HELVETICA_18,"Direcao de Arte",28,95,10);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Bruno Xavier",28,90,10);
    escreve(GLUT_BITMAP_HELVETICA_18,"Agradecimentos",28,80,10);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Bruno Xavier",28,75,10);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"Ricardo Xavier",28,70,10);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24,"E voce",28,65,10);
}
// Rotina de desenho
void desenhaMinhaCena(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(telainicial == 1){
        desenhaTelainicial();
        if(fechar == 1){
            desenhaCorfirmacao();
        }
    }
    else if(parte1 == 1){
        desenhaJogo();
        if(reiniciar == 1 || fechar == 1){
                desenhaCorfirmacao();
        }
    }
    else if(parte2 == 1){
        desenha2fase();
        if(reiniciar == 1 || fechar == 1){
                desenhaCorfirmacao();
        }
    }
    else if(telafinal == 1){
        desenhaTelafinal();
    }
    else if(creditos == 1){
        desenhaCreditos();
    }
    glutSwapBuffers();
}

void atualiza(){
    if(pausa == 0){
        glutPostRedisplay();
        postexturapeixe++;
        postexturaaguaviva++;
        postexturabaracuda++;
        if(postexturapeixe == 6){
            postexturapeixe = 0;
        }
        if(postexturaaguaviva == 9){
            postexturaaguaviva = 0;
        }
        if(postexturabaracuda == 8){
            postexturabaracuda = 0;
        }
        if(parte1 == 1){
            //movimenta os peixes para direita
            if(movepeixe == -LARGURA_DO_MUNDO||direita==1){
                movepeixe += 1;
                direita = 1;
                if(movepeixe >= 0){
                    direita = 0;
                }
            }//movimenta os peixes para esquerda
            else{
                movepeixe -= 1;
            }
            //sobe com anzol mais rapido dq a descida
            if(sobeobjeto >= 1541.5 || descer == 1){
                for(int i = 0; i < 20 && zerar == 0; i++){//permite que os inimigos dem dano novamente na subida
                    if(peixe[i].pontos<0){
                        peixe[i].pego=0;
                    }
                }
                zerar = 1;
                sobeobjeto -= 3;
                sobefundo -= 2.5;
                descer = 1;
                if(sobefundo < -45){
                    descer = 0;
                    subir = 0;
                    parte1 = 0;
                    parte2 = 1;
                    numpeixes2 = pontostotais / 250;
                    criaPeixes2();
                    desenhaMinhaCena();
                }
            }
            //desce o anzol
            else if(subir == 1){
                sobefundo += 1.25;
                sobeobjeto += 1.5;
            }
        }
        else if(parte2 == 1){
            quedaparte2 -= 2;
            if(quedaparte2 <= -380){
                parte2 = 0;
                telafinal = 1;
            }
        }
        else if(telafinal == 1){
            fimdetelafinal++;
            if(fimdetelafinal >= 150){
                creditos = 1;
                telafinal = 0;
                fimdetelafinal = 0;
                pausa = 0;
            }
        }
        else if(creditos == 1){
            fimdecreditos++;
            if(fimdecreditos >= 150){
                reinicia();
            }
        }
        glutSwapBuffers();
        glutTimerFunc(33, atualiza, 10);
    }
}

void reinicia(){
    criaPeixes();
    glutPostRedisplay();
    pontostotais = 0;
    pontosparte2 = 0;
    sprintf(pontosescreve, "%i", pontostotais);
    sprintf(pontosescreve2, "%d",pontosparte2);
    sobefundo = 0;
    sobeobjeto = 0;
    movanzol = 0;
    pontostotais = 0;
    pontosparte2 = 0;
    descer = 0;
    subir = 1;
    zerar = 0;
    parte2 = 0;
    creditos = 0;
    quedaparte2 = 0;
    if(reiniciar == 0){
        telainicial = 1;
        pausa = 1;
    }
    else{
        parte1 = 1;
        pausa = 0;
    }
    reiniciar = 0;
    desenhaMinhaCena();
    atualiza();
    glutSwapBuffers();
}

// mantendo o aspecto
void redimensionada(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARGURA_DO_MUNDO, 0, ALTURA_DO_MUNDO, -10, 10);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float) LARGURA_DO_MUNDO)/ ALTURA_DO_MUNDO;
    if (razaoAspectoJanela < razaoAspectoMundo) {
        hViewport = width / razaoAspectoMundo;
        yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
        wViewport = width;
    }
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        wViewport = ((float)height) * razaoAspectoMundo;
        xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
        hViewport = height;
    }
    else {
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
    }
    //pause
    if(key == 'p' && telainicial == 0 && creditos == 0){
        if(pausa==0)
            pausa = 1;
        else{
            pausa = 0;
            atualiza();
        }

    }
    //reiniciar
    if(key == 'r'){
        if(reiniciar == 0 && telainicial == 0 && creditos == 0){
        reiniciar  = 1;
        pausa = 1;
        desenhaMinhaCena();
        }
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
    float x0 = xViewport*90/wViewport;
    float y0 = yViewport*160/hViewport;

    //fprintf(log,"%f %f xv=%f yv=%f wv=%f hv=%f\n", x, y, xViewport, yViewport, wViewport, hViewport);

    y = hViewport - y;
    y = y*160/hViewport;
    x = x*90/wViewport;

   // fprintf(log,"%f %f\n",x,y);
    x -= x0;
    y -= y0;

    xmouse = x;
    ymouse = y;

    if (button == GLUT_LEFT_BUTTON && testeColisao(x,x,y,y,35,15,80,70)){
        if(reiniciar == 1){
            reinicia();
        }
        if(fechar == 1){
            exit(0);
        }
    }
    else if(button == GLUT_LEFT_BUTTON && testeColisao(x,x,y,y,75,55,80,70) == 1
            &&(reiniciar==1||fechar==1)){
        reiniciar = 0;
        fechar = 0;
        pausa = 0;
        atualiza();
    }
    else if(button == GLUT_LEFT_BUTTON && testeColisao(x,x,y,y,65,25,120,100) == 1
            && telainicial == 1){
                telainicial = 0;
                parte1 = 1;
                parte2 = 0;
                creditos = 0;
                pausa = 0;
                sobefundo = 0;
                sobeobjeto = 0;
                atualiza();
            }
    else if(button == GLUT_LEFT_BUTTON && testeColisao(x,x,y,y,65,25,90,70) == 1
            && telainicial == 1){
                telainicial = 0;
                creditos = 1;
                parte1 = 0;
                parte2 = 0;
                pausa = 0;
                fimdecreditos = 0;
                fprintf(log,"%d ",fimdecreditos);
                atualiza();
            }
    else if(button == GLUT_LEFT_BUTTON && testeColisao(x,x,y,y,65,25,60,40) == 1
            && telainicial == 1){
                exit(0);
            }

}

void setup(){
    glClearColor(0, 0, 0, 1);

    // habilita mesclagem de cores, para termos suporte a texturas (semi-)transparentes
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void iniciatexturas(){
    texturaanzol = SOIL_load_OGL_texture(
                           "anzol2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixeesq[0] = SOIL_load_OGL_texture(
                           "peixeesq1.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixeesq[1] = SOIL_load_OGL_texture(
                           "peixeesq2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixeesq[2] = SOIL_load_OGL_texture(
                           "peixeesq3.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixeesq[3] = SOIL_load_OGL_texture(
                           "peixeesq4.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixeesq[4] = SOIL_load_OGL_texture(
                           "peixeesq5.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixeesq[5] = SOIL_load_OGL_texture(
                           "peixeesq6.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixedir[0] = SOIL_load_OGL_texture(
                           "peixedir1.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixedir[1] = SOIL_load_OGL_texture(
                           "peixedir2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixedir[2] = SOIL_load_OGL_texture(
                           "peixedir3.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixedir[3] = SOIL_load_OGL_texture(
                           "peixedir4.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixedir[4] = SOIL_load_OGL_texture(
                           "peixedir5.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapeixedir[5] = SOIL_load_OGL_texture(
                           "peixedir6.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturaaguaviva[0] = SOIL_load_OGL_texture(
                           "aguaviva1.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturaaguaviva[1] = SOIL_load_OGL_texture(
                           "aguaviva2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturaaguaviva[2] = SOIL_load_OGL_texture(
                            "aguaviva3.png",
                            SOIL_LOAD_AUTO,
                            SOIL_CREATE_NEW_ID,
                            SOIL_FLAG_INVERT_Y
                        );
   texturaaguaviva[3] = SOIL_load_OGL_texture(
                        "aguaviva4.png",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_INVERT_Y
                       );
   texturaaguaviva[4] = SOIL_load_OGL_texture(
                           "aguaviva5.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturaaguaviva[5] = SOIL_load_OGL_texture(
                           "aguaviva6.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturaaguaviva[6] = SOIL_load_OGL_texture(
                           "aguaviva7.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturaaguaviva[7] = SOIL_load_OGL_texture(
                           "aguaviva8.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturaaguaviva[8] = SOIL_load_OGL_texture(
                           "aguaviva9.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturaaguaviva[9] = SOIL_load_OGL_texture(
                           "aguaviva10.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturatubaraodir[0] = SOIL_load_OGL_texture(
                           "tubaraodir1.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturatubaraodir[1] = SOIL_load_OGL_texture(
                           "tubaraodir2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
    texturatubaraodir[2] = SOIL_load_OGL_texture(
                           "tubaraodir3.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                        );
    texturatubaraodir[3] = SOIL_load_OGL_texture(
                           "tubaraodir4.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturatubaraodir[4] = SOIL_load_OGL_texture(
                           "tubaraodir5.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturatubaraodir[5] = SOIL_load_OGL_texture(
                           "tubaraodir6.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturatubaraoesq[0] = SOIL_load_OGL_texture(
                           "tubaraoesq1.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturatubaraoesq[1] = SOIL_load_OGL_texture(
                           "tubaraoesq2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
    texturatubaraoesq[2] = SOIL_load_OGL_texture(
                           "tubaraoesq3.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                        );
    texturatubaraoesq[3] = SOIL_load_OGL_texture(
                           "tubaraoesq4.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
  texturatubaraoesq[4] = SOIL_load_OGL_texture(
                           "tubaraoesq5.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturatubaraoesq[5] = SOIL_load_OGL_texture(
                           "tubaraoesq6.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudadir[0] = SOIL_load_OGL_texture(
                           "baracudadir1.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudadir[1] = SOIL_load_OGL_texture(
                           "baracudadir2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudadir[2] = SOIL_load_OGL_texture(
                           "baracudadir3.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudadir[3] = SOIL_load_OGL_texture(
                           "baracudadir4.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudadir[4] = SOIL_load_OGL_texture(
                           "baracudadir5.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
    texturabaracudadir[5] = SOIL_load_OGL_texture(
                           "baracudadir6.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudadir[6] = SOIL_load_OGL_texture(
                           "baracudadir7.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudadir[7] = SOIL_load_OGL_texture(
                           "baracudadir8.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudaesq[0] = SOIL_load_OGL_texture(
                           "baracudaesq1.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudaesq[1] = SOIL_load_OGL_texture(
                           "baracudaesq2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudaesq[2] = SOIL_load_OGL_texture(
                           "baracudaesq3.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudaesq[3] = SOIL_load_OGL_texture(
                           "baracudaesq4.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudaesq[4] = SOIL_load_OGL_texture(
                           "baracudaesq5.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudaesq[5] = SOIL_load_OGL_texture(
                           "baracudaesq6.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudaesq[6] = SOIL_load_OGL_texture(
                           "baracudaesq7.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturabaracudaesq[7] = SOIL_load_OGL_texture(
                           "baracudaesq8.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapescador = SOIL_load_OGL_texture(
                           "pescador.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapescador2 = SOIL_load_OGL_texture(
                           "pescador2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturapescador3 = SOIL_load_OGL_texture(
                           "pescador3.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturaceu = SOIL_load_OGL_texture(
                           "ceu.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturacaixagrande = SOIL_load_OGL_texture(
                           "caixagrande.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturacaixapequena = SOIL_load_OGL_texture(
                           "caixapequena.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturafundo = SOIL_load_OGL_texture(
                           "fundo2.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturachao = SOIL_load_OGL_texture(
                           "chao.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturamira = SOIL_load_OGL_texture(
                           "mira.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
   texturacaixapont = SOIL_load_OGL_texture(
                           "caixapont.png",
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );
}
// Função principal
int main(int argc, char** argv){
    //log=fopen("peixes.log","w");
    //setbuf(log,NULL);
    criaPeixes();

    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(450, 800);

    glutCreateWindow("Pescaria: o jogo");
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
    iniciatexturas();

    glutMainLoop();
    glDisable(GL_DEPTH_TEST);
    //fclose(log);
    return 0;
}
