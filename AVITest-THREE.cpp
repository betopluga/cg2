// **********************************************************************
// PUCRS/FACIN
// COMPUTAÇÃO GRÁFICA
//
// Arquivo: AVITest.cpp
//
// Programa de testes para carga de Imagens e vídeos AVI
//
//  Este programa deve ser compilador junto com a classe "ImageClass",
//  que está implementada no arquivo "ImageClass.cpp"
//
//  Para a carga das imagens este programa utiliza a biblioteca
//  IM http://www.tecgraf.puc-rio.br/im de autoria de Antonio Scuri
//
// Marcio Serolli Pinho
// 12/08/2003
//  pinho@inf.pucrs.br
// **********************************************************************
#include <explode.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>
#include <Marcador.h>
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

#include "AVIClass.h"

int cinza = false;

AVIClass Video;
vector<Marcador> marcadores;
vector<Marcador>::iterator it;
vector<Marcador>::iterator it2;
vector<Marcador> marcadorGlobal;
int matrizAlgo2[15][15];
int matrizAlgo21[15][15];
int matrizAlgo22[15][15];
int matrizAlgo23[15][15];
int matrizAlgo24[15][15];
int matrizAlgo25[15][15];
int matrizAlgo26[15][15];
int matrizAlgo27[15][15];
int x_mar14;
int y_mar14;
int x_mar11;
int y_mar11;
int x_mar8;
int y_mar8;
int x_mar7;
int y_mar7;
int x_mar8aux2;
int y_mar8aux2;
int x_mar15aux2;
int y_mar15aux2;
int x_mar15;
int y_mar15;

// **********************************************************************
//  void init(void)
//  Inicializa os parâmetros globais de OpenGL
//
// **********************************************************************
void init(void){

    cout << "Init..." ;
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    imFormatRegisterAVI();
    //imFormatRegisterWMV();
    if(Video.openVideoFile("Videos\\VideoBike.avi") == 0)
    //if(Video.openVideoFile("Videos\\video_original.avi") == 0)
    {
       cout << "Problemas na abertura do video" << endl;
    }
    else cout << "Abertura OK ! - Nro de Frames:" << Video.getTotalFrames() << endl;
    cout << "Dimensoes do Video:" << Video.SizeX() << " x " << Video.SizeY() <<endl;
    cout << "Fim." << endl;
    remove( "saida.txt" );

}

// **********************************************************************
// void OrdenaVetor()
// **********************************************************************
void OrdenaVetor(int window[])
{
    int temp, i , j;
    for(i = 0; i < 9; i++)
    {
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--)
        {
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}

// **********************************************************************
// void MontaVetor()
// **********************************************************************
void MontaVetor(int Px, int Py, int Vetor[9])
{
    Vetor[0] = Video.GetPointIntensity(Px-1,Py-1);
    Vetor[1] = Video.GetPointIntensity(Px-1,Py);
    Vetor[2] = Video.GetPointIntensity(Px-1,Py+1);
    Vetor[3] = Video.GetPointIntensity(Px,Py-1);
    Vetor[4] = Video.GetPointIntensity(Px,Py);
    Vetor[5] = Video.GetPointIntensity(Px,Py+1);
    Vetor[6] = Video.GetPointIntensity(Px+1,Py-1);
    Vetor[7] = Video.GetPointIntensity(Px+1,Py);
    Vetor[8] = Video.GetPointIntensity(Px+1,Py+1);
}
// **********************************************************************
// void Mediana()
// **********************************************************************
void Mediana(){

    int Vetor[9];
    int x, y, mediana;

    for(x=1;x<Video.SizeX();x++){
        for(y=1;y<Video.SizeY();y++){
            MontaVetor(x,y, Vetor); // Coloca em VETOR os valores das intensidades ao redor do ponto x,y.
            OrdenaVetor(Vetor);
            mediana = Vetor[5];

            Video.DrawPixel(x,y,mediana,mediana,mediana);
        }
    }
}


// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    gluOrtho2D(0,w,0,h);

    // Set the clipping volume
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

// **********************************************************************
//  void ConverteCinza(ImageClass *Img)
//
//
// **********************************************************************
void ConverteCinza(AVIClass V){

    int x,y;
    int i;
    unsigned char r,g,b;
    //cout << "Iniciou Black & White....";
    //NovaImagem->DrawPixel(20, 1,100,255,0,0 );

    for(x=0;x<V.SizeX();x++)
    {
        for(y=0;y<V.SizeY();y++)
        {
            i = V.GetPointIntensity(x,y);
            //Img->ReadPixel(x,y, r,g,b);
            if (i> 200)
              V.DrawPixel(x, y,255,255,255);
            else V.DrawPixel(x, y,0,0,0);
        }
    }
}
// **********************************************************************
//  void CalculaNivelDeZoom(float &ZoomH, float &ZoomV)
// **********************************************************************
void CalculaNivelDeZoom(float &ZoomH, float &ZoomV)
{
     ZoomH = (float)glutGet(GLUT_WINDOW_WIDTH)/Video.SizeX();
     ZoomV = (float)glutGet(GLUT_WINDOW_HEIGHT)/Video.SizeY();
}

int distanciaEuclidianaEntrePontos(int x1, int y1, int x2, int y2) {
    int distancia;
    distancia = sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
    return distancia;
}

// **********************************************************************
//  void DesenhaEsqueleto(int frame)
// **********************************************************************
void DesenhaEsqueleto(int pframe){

    int distancia_menor_temp = 999999;
    int x_centro_dest = 0;
    int y_centro_dest = 0;
    int distancia_temp;
    int proximo = 0;

    //desenha esqueleto
    for (it = marcadorGlobal.begin(); it != marcadorGlobal.end(); ++it) {
        for (it2 = marcadorGlobal.begin(); it2 != marcadorGlobal.end(); ++it2) {
            if(it->id_proximo==it2->id_marcador && pframe==it->num_frame && pframe==it2->num_frame){
                Video.DrawLine(it->x_centro, it->y_centro, it2->x_centro, it2->y_centro, 255, 255, 255);
            }
        }
    }
}

// **********************************************************************
//  void leMarcador(int x, int y, AVIClass V, std::vector< vector<string> > &Marcador)
// **********************************************************************
void leMarcador(int x, int y, AVIClass V, Marcador &marcador){
    double i;
    std::ostringstream stream;
    std::string ctemp;
    unsigned char r,g,b;

    i = V.GetPointIntensity(x-1,y);
    V.ReadPixel(x-1,y, r,g,b);
    if(i>=180 && g<255){

        //guarda pixel do marcador
        stream << x-1 << "-" << y;
        ctemp = stream.str();
        marcador.pixels_xy.push_back(ctemp);
        stream.str( std::string() );
        stream.clear();
        ctemp = "";

        //pinta pixel e chama leMarcador com nova posicao
        V.DrawPixel(x-1,y,0,255,0);
        leMarcador(x-1,y,V,marcador);
    }

    i = V.GetPointIntensity(x+1,y);
    V.ReadPixel(x+1,y, r,g,b);
    if(i>=180 && g<255){

        //guarda pixel do marcador
        stream << x+1 << "-" << y;
        ctemp = stream.str();
        marcador.pixels_xy.push_back(ctemp);
        stream.str( std::string() );
        stream.clear();
        ctemp = "";

        //pinta pixel e chama leMarcador com nova posicao
        V.DrawPixel(x+1,y,0,255,0);
        leMarcador(x+1,y,V,marcador);
    }

    i = V.GetPointIntensity(x,y-1);
    V.ReadPixel(x,y-1, r,g,b);
    if(i>=180 && g<255){

        //guarda pixel do marcador
        stream << x << "-" << y-1;
        ctemp = stream.str();
        marcador.pixels_xy.push_back(ctemp);
        stream.str( std::string() );
        stream.clear();
        ctemp = "";

        //pinta pixel e chama leMarcador com nova posicao
        V.DrawPixel(x,y-1,0,255,0);
        leMarcador(x,y-1,V,marcador);
    }

    i = V.GetPointIntensity(x,y+1);
    V.ReadPixel(x,y+1, r,g,b);
    if(i>=180 && g<255){

        //guarda pixel do marcador
        stream << x << "-" << y+1;
        ctemp = stream.str();
        marcador.pixels_xy.push_back(ctemp);
        stream.str( std::string() );
        stream.clear();
        ctemp = "";

        //pinta pixel e chama leMarcador com nova posicao
        V.DrawPixel(x,y+1,0,255,0);
        leMarcador(x,y+1,V,marcador);
    }
}

void calculaCentro(Marcador &marcador){
    int x_menor = 999999;
    int x_maior = 0;
    int y_menor = 999999;
    int y_maior = 0;
    int x,y;

    //imprime os pixels do marcador
    std::vector<std::string> rtemp;
    std::string ctemp;
    for(int q=0; q<marcador.pixels_xy.size(); ++q){
        ctemp = marcador.pixels_xy[q];
        rtemp = explode(ctemp, '-');
        std::istringstream sx(rtemp[0]);
        sx >> x;
        sx.str( std::string() );
        sx.clear();
        std::istringstream sy(rtemp[1]);
        sy >> y;
        sy.str( std::string() );
        sy.clear();

        //pega pixel mais a esquerda e mais a direita
        if(x>x_maior){ x_maior = x; }
        if(x<x_menor){ x_menor = x; }
        if(y>y_maior){ y_maior = y; }
        if(y<y_menor){ y_menor = y; }
    }

    int x_raio = ((int)(x_maior - x_menor) / 2);
    int y_raio = ((int)(y_maior - y_menor) / 2);
    if(x_raio!=500000&&y_raio!=500000){
        marcador.x_centro = x_raio + x_menor;
        marcador.y_centro = y_raio + y_menor;
    } else {
        marcador.x_centro = 1;
        marcador.y_centro = 1;
    }
}

int retornarRGB(int id, char cor){
    switch(id){
        case 1: if(cor=='r'){ return 255; } else if(cor=='g'){ return 100; } else { return 150; } break;
        case 2: if(cor=='r'){ return 100; } else if(cor=='g'){ return 200; } else { return 250; } break;
        case 3: if(cor=='r'){ return 0; } else if(cor=='g'){ return 150; } else { return 50; } break;
        case 4: if(cor=='r'){ return 150; } else if(cor=='g'){ return 255; } else { return 120; } break;
        case 5: if(cor=='r'){ return 75; } else if(cor=='g'){ return 50; } else { return 140; } break;
        case 6: if(cor=='r'){ return 60; } else if(cor=='g'){ return 150; } else { return 100; } break;
        case 7: if(cor=='r'){ return 40; } else if(cor=='g'){ return 100; } else { return 250; } break;
        case 8: if(cor=='r'){ return 20; } else if(cor=='g'){ return 255; } else { return 80; } break;
        case 9: if(cor=='r'){ return 90; } else if(cor=='g'){ return 50; } else { return 175; } break;
        case 10: if(cor=='r'){ return 10; } else if(cor=='g'){ return 20; } else { return 130; } break;
        case 11: if(cor=='r'){ return 250; } else if(cor=='g'){ return 10; } else { return 180; } break;
        case 12: if(cor=='r'){ return 200; } else if(cor=='g'){ return 150; } else { return 100; } break;
        case 13: if(cor=='r'){ return 75; } else if(cor=='g'){ return 100; } else { return 90; } break;
        case 14: if(cor=='r'){ return 80; } else if(cor=='g'){ return 90; } else { return 50; } break;
        case 15: if(cor=='r'){ return 55; } else if(cor=='g'){ return 255; } else { return 120; } break;
        case 16: if(cor=='r'){ return 25; } else if(cor=='g'){ return 200; } else { return 255; } break;
        case 17: if(cor=='r'){ return 35; } else if(cor=='g'){ return 50; } else { return 25; } break;
        case 18: if(cor=='r'){ return 90; } else if(cor=='g'){ return 20; } else { return 120; } break;
        case 19: if(cor=='r'){ return 10; } else if(cor=='g'){ return 10; } else { return 75; } break;
        case 20: if(cor=='r'){ return 15; } else if(cor=='g'){ return 90; } else { return 120; } break;
        case 21: if(cor=='r'){ return 100; } else if(cor=='g'){ return 120; } else { return 120; } break;
        case 22: if(cor=='r'){ return 250; } else if(cor=='g'){ return 180; } else { return 100; } break;
        case 23: if(cor=='r'){ return 200; } else if(cor=='g'){ return 160; } else { return 20; } break;
        case 24: if(cor=='r'){ return 120; } else if(cor=='g'){ return 90; } else { return 30; } break;
        case 25: if(cor=='r'){ return 80; } else if(cor=='g'){ return 50; } else { return 255; } break;
    }
}

void pintaMarcador(AVIClass V, Marcador &marcador){
    std::vector<std::string> rtemp;
    std::string ctemp;
    int x,y;
    for(int q=0; q<marcador.pixels_xy.size(); ++q){
        ctemp = marcador.pixels_xy[q];
        rtemp = explode(ctemp, '-');
        std::istringstream sx(rtemp[0]);
        sx >> x;
        sx.str( std::string() );
        sx.clear();
        std::istringstream sy(rtemp[1]);
        sy >> y;
        sy.str( std::string() );
        sy.clear();
        V.DrawPixel(x, y, marcador.r, marcador.g, marcador.b);
    }
}

// **********************************************************************
//  void carregaMarcadores(ImageClass *Img)//pinta o marcador no novo frame
// **********************************************************************
void carregaMarcadores(AVIClass V, int frame){

    //declaração de variaveis
    int x,y;
    std::ostringstream stream;
    std::string ctemp;
    int i, j=0, k=0;
    unsigned char r,g,b;
    vector<Marcador> marcadores;
    vector<Marcador>::iterator it;
    vector<Marcador>::iterator it2;
    int x_mar8aux;

    stringstream ss;
    ss << frame;
    string frame_desc = ss.str();
    ss.str( std::string() );
    ss.clear();

    //flood fill guardando os marcadores
    if(frame==1){
        for(x=0;x<V.SizeX();x++){
            for(y=0;y<V.SizeY();y++){
                if(x==159&&y==250||x==195&&y==271||x==199&&y==239||x==200&&y==356||x==246&&y==212||x==255&&y==325||x==257&&y==382||x==279&&y==349||x==284&&y==92||x==291&&y==209||x==299&&y==312||x==318&&y==135){
                    i = V.GetPointIntensity(x,y);
                    V.ReadPixel(x,y, r,g,b);
                    if (i>235 && g<255){
                        Marcador *m0 = new Marcador;
                        m0->id_marcador = k+1;
                        m0->num_frame = frame;
                        leMarcador(x,y,V,*m0);
                        if(frame==1){
                            m0->r = retornarRGB(k+1, 'r');
                            m0->g = retornarRGB(k+1, 'g');
                            m0->b = retornarRGB(k+1, 'b');
                            pintaMarcador(V, *m0);
                        }

                        marcadores.push_back(*m0);
                        k++;
                    }
                }
            }
        }

        //gera arquivo de saida
        ofstream outputFile;
        outputFile.open("saida.txt", std::ios_base::app);
        outputFile << endl << ">>>>>>FRAME " + frame_desc << endl;
        int x_maior=0, x_menor=999999;
        int y_maior=0, y_menor=999999;
        int x_centro_ant;
        int y_centro_ant;

        //imprime o array de marcadores
        for (it = marcadores.begin(); it != marcadores.end(); ++it) {
            outputFile << "Marcador: " << it->id_marcador << endl;
            outputFile << "Tamanho: ";
            outputFile <<  it->pixels_xy.size();
            outputFile << " px" << endl;

            //atribui esqueleto
            if(it->id_marcador==11){ it->id_proximo = 6; }
            if(it->id_marcador==6){ it->id_proximo = 8; }
            if(it->id_marcador==8){ it->id_proximo = 7; }
            if(it->id_marcador==7){ it->id_proximo = 4; }
            if(it->id_marcador==4){ it->id_proximo = 2; }
            if(it->id_marcador==2){ it->id_proximo = 1; }
            if(it->id_marcador==1){ it->id_proximo = 3; }
            if(it->id_marcador==3){ it->id_proximo = 5; }
            if(it->id_marcador==5){ it->id_proximo = 10; }
            if(it->id_marcador==10){ it->id_proximo = 12; }
            if(it->id_marcador==12){ it->id_proximo = 9; }

            x_menor = 999999;
            x_maior = 0;
            y_menor = 999999;
            y_maior = 0;

            //imprime os pixels do marcador
            std::vector<std::string> rtemp;
            std::string ctemp;
            for(int q=0; q<it->pixels_xy.size(); ++q){
                ctemp = it->pixels_xy[q];
                rtemp = explode(ctemp, '-');
                std::istringstream sx(rtemp[0]);
                sx >> x;
                sx.str( std::string() );
                sx.clear();
                std::istringstream sy(rtemp[1]);
                sy >> y;
                sy.str( std::string() );
                sy.clear();

                //pega pixel mais a esquerda e mais a direita
                if(x>x_maior){ x_maior = x; }
                if(x<x_menor){ x_menor = x; }
                if(y>y_maior){ y_maior = y; }
                if(y<y_menor){ y_menor = y; }
            }

            int x_raio = ((int)(x_maior - x_menor) / 2);
            int y_raio = ((int)(y_maior - y_menor) / 2);
            if(x_raio!=500000&&y_raio!=500000){
                it->x_centro = x_raio + x_menor;
                it->y_centro = y_raio + y_menor;
            } else {
                it->x_centro = 1;
                it->y_centro = 1;
            }

            outputFile << "Centro: ";
            outputFile <<  it->x_centro;
            outputFile << " x ";
            outputFile <<  it->y_centro << endl;
            outputFile << "Proximo: ";
            outputFile <<  it->id_proximo << endl;
            if(it->x_centro<=640 && it->y_centro<=480){
                //V.DrawPixel(it->x_centro,  it->y_centro, 0, 0, 0 );
            }
            marcadorGlobal.push_back(*it);
        }
        outputFile.close();
    }

    int distancia_menor = 999999;
    int distancia;
    int r1;
    int g1;
    int b1;
    int x_centro;
    int y_centro;
    int id_proximo;
    int m=0;
    k=0;
    vector<string> pixels_xytemp;

    if(frame>=2){
        k = 0;
        //para cada marcador no frame anterior
        for (int h = 0; h < marcadorGlobal.size(); ++h) {
            if(marcadorGlobal[h].num_frame==frame-1){
                distancia_menor = 999999;
                distancia = 0;
                x_centro = marcadorGlobal[h].x_centro;
                y_centro = marcadorGlobal[h].y_centro;

                //busca pelo marcador mais proximo usando limiar e flood fill
                for(int x=x_centro-20;x<x_centro+20;x++){
                    for(int y=y_centro-20;y<y_centro+20;y++){
                        i = V.GetPointIntensity(x,y);
                        V.ReadPixel(x,y, r,g,b);
                        if (i>235 && g<255){
                            Marcador *m0 = new Marcador;
                            m0->id_marcador = k+1;
                            m0->num_frame = frame;
                            leMarcador(x,y,V,*m0);
                            calculaCentro(*m0);
                            marcadores.push_back(*m0);
                            k++;
                        }
                    }
                }

                for (it = marcadores.begin(); it != marcadores.end(); ++it) {

                    //se for no frame corrente
                    if(it->num_frame==frame){
                        distancia = distanciaEuclidianaEntrePontos(marcadorGlobal[h].x_centro, marcadorGlobal[h].y_centro, it->x_centro, it->y_centro);

                        if(distancia<distancia_menor){
                            distancia_menor = distancia;

                            //pega a cor do marcador no frame anterior
                            r1 = marcadorGlobal[h].r;
                            g1 = marcadorGlobal[h].g;
                            b1 = marcadorGlobal[h].b;
                            pixels_xytemp = it->pixels_xy;
                            x_centro = it->x_centro;
                            y_centro = it->y_centro;
                            id_proximo = marcadorGlobal[h].id_proximo;
                        }
                    }



                }

                Marcador *m1 = new Marcador;
                m1->id_marcador = m+1;
                m1->r = r1;
                m1->g = g1;
                m1->b = b1;
                m1->num_frame = frame;
                m1->x_centro = x_centro;
                m1->y_centro = y_centro;
                m1->id_proximo = id_proximo;
                m1->pixels_xy = pixels_xytemp;
                //marcadores.push_back(*m1);
                marcadorGlobal.push_back(*m1);
                m++;

                //pinta o marcador no novo frame
                std::vector<std::string> rtemp;
                std::string ctemp;
                for(int q=0; q<pixels_xytemp.size(); ++q){
                    ctemp = pixels_xytemp[q];
                    rtemp = explode(ctemp, '-');
                    std::istringstream sx(rtemp[0]);
                    sx >> x;
                    sx.str( std::string() );
                    sx.clear();
                    std::istringstream sy(rtemp[1]);
                    sy >> y;
                    sy.str( std::string() );
                    sy.clear();
                    V.DrawPixel(x, y, r1, g1, b1);
                }
            }
        }
    }
}

// **********************************************************************
//  void display( void )
// **********************************************************************
int frame = 1;
void display( void )
{

    int loadFrameOK;
    float ZoomH, ZoomV;

    glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10,10,-10,10,-1,1);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

    loadFrameOK = Video.loadImageFrame(frame);

    if(frame>=1){
        //cinza = true;
        Mediana();
        carregaMarcadores(Video, frame);
        DesenhaEsqueleto(frame);
    }

    // avança o nro do frame
    cout << "Frame: " << frame << endl;
    frame ++;
    // se atingiu o final do vídeo, então recomeça
    if (frame == Video.getTotalFrames())
    {
         frame = 0;
        // getchar();
         return;
    }
    if (loadFrameOK)
    {

      // cout << "Imagem Exibida !!" << endl;
       Video.SetPos(-10,-10);
       // acerta o zoom da imagem para que ocupe toda a janela
       CalculaNivelDeZoom(ZoomH, ZoomV);
       Video.SetZoomH(ZoomH);
       Video.SetZoomV(ZoomV);
       if (cinza)
       {
          ConverteCinza(Video);
       }
       Video.Display();
    }
    else cout << "Erro..." << endl;

    glutSwapBuffers();
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{

    switch ( key )
    {
        case 'c': cinza = !cinza;
                  glutPostRedisplay();
                  break;

        case ' ':
                   glutPostRedisplay();
                   break;
        case 27: // Termina o programa qdo
             Video.closeVideoFile();
        exit ( 0 );   // a tecla ESC for pressionada
        break;
        default:
        break;
    }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch ( a_keys )
    {
        case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
        break;
        case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...

        break;
        default:
        break;
    }
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
    init ();

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (300,10);

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize  ( 640, 480);
    glutCreateWindow    ( "T2 CG - Carlos Alberto dos Santos" );

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );
    //glutIdleFunc ( display );

    //MessageBox(NULL, "Pressione a barra de espaço para avancar o vídeo.", "Mensagem", MB_OK);
    //cout << "Pressione a barra de espaço para avancar o vídeo." << endl;

    glutMainLoop ();
    cout <<"FIM\n";

    return 0;
}

