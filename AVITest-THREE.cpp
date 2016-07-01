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
void Mediana()
{
    cout << "Iniciou Mediana..." << endl;

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

    cout << "Concluiu Mediana." << endl;

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

int distanciaEuclidianaEntrePontos(int x1, int y1, int x2, int y2) {
    int distancia;
    distancia = sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
    return distancia;
}

// **********************************************************************
//  void carregaMarcadores(ImageClass *Img)
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
    int distancia_temp;
    int distancia_menor_temp = 999999;
    int x_centro_dest;
    int y_centro_dest;

    stringstream ss;
    ss << frame;
    string frame_desc = ss.str();
    ss.str( std::string() );
    ss.clear();

    //flood fill guardando os marcadores
    for(x=0;x<V.SizeX();x++){
        for(y=0;y<V.SizeY();y++){
            i = V.GetPointIntensity(x,y);
            V.ReadPixel(x,y, r,g,b);
            if (i>235 && g<255){
                Marcador *m0 = new Marcador;
                m0->id_marcador = k+1;
                m0->num_frame = frame;
                leMarcador(x,y,V,*m0);
                marcadores.push_back(*m0);
                k++;
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
        if(it->x_centro<=640 && it->y_centro<=480){
            //V.DrawPixel(it->x_centro,  it->y_centro, 0, 0, 0 );
        }
    }
    outputFile.close();

    //desenha esqueleto
    //para cada ponto calcula a distancia euclidiana do mais proximo
    /*for (it = marcadores.begin(); it != marcadores.end(); ++it) {
        for (it2 = marcadores.begin(); it2 != marcadores.end(); ++it2) {
            if(it->id_marcador != it2->id_marcador && it2->id_proximo!=it->id_marcador){

                distancia_temp = distanciaEuclidianaEntrePontos(it->x_centro, it->y_centro, it2->x_centro, it2->y_centro);
                if(distancia_temp<distancia_menor_temp){
                    distancia_menor_temp = distancia_temp;
                    x_centro_dest = it2->x_centro;
                    y_centro_dest = it2->y_centro;
                    it->id_proximo = it2->id_marcador;
                }
            }
        }

        if(it->x_centro!=500000||it->y_centro!=500000){
            cout << x_centro_dest << "-" << y_centro_dest << " x " << it->x_centro << "-" << it->y_centro << endl;
            V.DrawLine(it->x_centro, it->y_centro, x_centro_dest, y_centro_dest, 255, 255, 255);
        }

        distancia_menor_temp = 999999;
        x_centro_dest = 0;
        y_centro_dest = 0;
    }*/

    //a partir do 3o frame identifica um dos marcadores e passa a acompanhá-lo frame a frame
    //usando os algoritmos solicitados
    if(frame==3){
        for (it = marcadores.begin(); it != marcadores.end(); ++it) {
            std::vector<std::string> rtemp;
            std::string ctemp;
            if(it->id_marcador==14){
                x_mar14 = it->x_centro;
                y_mar14 = it->y_centro;

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
                    V.DrawPixel(x, y, 255, 0, 0 );
                }
            }

            if(it->id_marcador==11){
                x_mar11 = it->x_centro;
                y_mar11 = it->y_centro;

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
                    V.DrawPixel(x, y, 100, 50, 0 );
                }
            }

/*            if(it->id_marcador==15){
                x_mar15 = it->x_centro;
                y_mar15 = it->y_centro;

                //pinta marcador de azul
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
                    V.DrawPixel(x, y, 0, 50, 100 );
                }
            }
*/

            if(it->id_marcador==15){
                x_mar15 = it->x_centro;
                y_mar15 = it->y_centro;

                const int x_mar15aux = x_mar15;
                const int y_mar15aux = y_mar15;

                x_mar15aux2 = x_mar15aux;
                y_mar15aux2 = y_mar15aux;

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
                    V.DrawPixel(x, y, 50, 0, 250 );
                }
            }

            if(it->id_marcador==8){
                x_mar8 = it->x_centro;
                y_mar8 = it->y_centro;

                const int x_mar8aux = x_mar8;
                const int y_mar8aux = y_mar8;

                //monta matriz real do marcador 8 para execução do algoritmo 2
                int k1=0;
                int k2=0;
                for(int x2=x_mar8-7; x2<=x_mar8+7;++x2){
                    k2=0;
                    for(int y2=y_mar8-7; y2<=y_mar8+7;++y2){
                        if(V.GetPointIntensity(x2,y2)>100){
                            matrizAlgo2[k1][k2] = 1;
                        } else {
                            matrizAlgo2[k1][k2] = 0;
                        }
                        cout << matrizAlgo2[k1][k2] << " ";
                        k2++;
                    }
                    cout << endl;
                    k1++;
                } cout << "ANTES " << x_mar8aux << endl;

                x_mar8aux2 = x_mar8aux;
                y_mar8aux2 = y_mar8aux;

                //pinta marcador de azul
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
                    V.DrawPixel(x, y, 50, 0, 250 );
                }
            }
        }
    }

    //localiza o marcador 15 (pé) nos frames 4 e 5 com base na distancia entre os pontos
    if(frame>=4){

        int distancia_menor = 999999;
        int distancia;
        vector<string> pixels;
        for (it = marcadores.begin(); it != marcadores.end(); ++it) {
            if(it->num_frame==frame){
                distancia = distanciaEuclidianaEntrePontos(x_mar14, y_mar14, it->x_centro, it->y_centro);
                if(distancia<distancia_menor){
                    distancia_menor = distancia;
                    pixels = it->pixels_xy;
                }
            }
        }

        std::vector<std::string> rtemp;
        std::string ctemp;
        for(int q=0; q<pixels.size(); ++q){
            ctemp = pixels[q];
            rtemp = explode(ctemp, '-');
            std::istringstream sx(rtemp[0]);
            sx >> x;
            sx.str( std::string() );
            sx.clear();
            std::istringstream sy(rtemp[1]);
            sy >> y;
            sy.str( std::string() );
            sy.clear();
            V.DrawPixel(x, y, 255, 0, 0 );
            x_mar14 = x;
            y_mar14 = y;
        }

        distancia_menor = 999999;
        distancia;
        vector<string> pixels2;
        for (it = marcadores.begin(); it != marcadores.end(); ++it) {
            if(it->num_frame==frame){
                distancia = distanciaEuclidianaEntrePontos(x_mar11, y_mar11, it->x_centro, it->y_centro);
                if(distancia<distancia_menor){
                    distancia_menor = distancia;
                    pixels2 = it->pixels_xy;
                }
            }
        }

        std::vector<std::string> rtemp2;
        std::string ctemp2;
        for(int q=0; q<pixels2.size(); ++q){
            ctemp2 = pixels2[q];
            rtemp2 = explode(ctemp2, '-');
            std::istringstream sx(rtemp2[0]);
            sx >> x;
            sx.str( std::string() );
            sx.clear();
            std::istringstream sy(rtemp2[1]);
            sy >> y;
            sy.str( std::string() );
            sy.clear();
            V.DrawPixel(x, y, 100, 50, 0 );
            x_mar11 = x;
            y_mar11 = y;
        }

        distancia_menor = 999999;
        distancia;
        vector<string> pixels3;
        for (it = marcadores.begin(); it != marcadores.end(); ++it) {
            if(it->num_frame==frame){
                distancia = distanciaEuclidianaEntrePontos(x_mar8, y_mar8, it->x_centro, it->y_centro);
                if(distancia<distancia_menor){
                    distancia_menor = distancia;
                    pixels3 = it->pixels_xy;
                }
            }
        }

        std::vector<std::string> rtemp3;
        std::string ctemp3;
        for(int q=0; q<pixels3.size(); ++q){
            ctemp3 = pixels3[q];
            rtemp3 = explode(ctemp3, '-');
            std::istringstream sx(rtemp3[0]);
            sx >> x;
            sx.str( std::string() );
            sx.clear();
            std::istringstream sy(rtemp3[1]);
            sy >> y;
            sy.str( std::string() );
            sy.clear();
            V.DrawPixel(x, y, 50, 0, 250 );
            x_mar8 = x;
            y_mar8 = y;
        }

        distancia_menor = 999999;
        distancia;
        vector<string> pixels4;
        for (it = marcadores.begin(); it != marcadores.end(); ++it) {
            if(it->num_frame==frame){
                distancia = distanciaEuclidianaEntrePontos(x_mar15, y_mar15, it->x_centro, it->y_centro);
                if(distancia<distancia_menor){
                    distancia_menor = distancia;
                    pixels4 = it->pixels_xy;
                }
            }
        }

        std::vector<std::string> rtemp4;
        std::string ctemp4;
        for(int q=0; q<pixels4.size(); ++q){
            ctemp4 = pixels4[q];
            rtemp4 = explode(ctemp4, '-');
            std::istringstream sx(rtemp4[0]);
            sx >> x;
            sx.str( std::string() );
            sx.clear();
            std::istringstream sy(rtemp4[1]);
            sy >> y;
            sy.str( std::string() );
            sy.clear();
            V.DrawPixel(x, y, 50, 0, 250 );
            x_mar15 = x;
            y_mar15 = y;
        }

        V.DrawLine(x_mar8, y_mar8, x_mar15, y_mar15, 255, 255, 255);
        distancia = distanciaEuclidianaEntrePontos(x_mar8, y_mar8, x_mar15, y_mar15);
        cout << "Frame " << frame << ": 8x15 => " << distancia << endl;

        distancia = distanciaEuclidianaEntrePontos(x_mar8, y_mar8, x_mar11, y_mar11);
        cout << "Frame " << frame << ": 8x11 => " << distancia << endl;

        distancia = distanciaEuclidianaEntrePontos(x_mar8, y_mar8, x_mar14, y_mar14);
        cout << "Frame " << frame << ": 8x14 => " << distancia << endl;


        //para cada frame deve executar 5 vezes a busca por comparação (meio e cruz)
        //monta nova matriz do marcador 8 para execução do algoritmo 2
        //cout << "centro" << endl;
        bool loop1=true;
        int x_novoponto = 0;
        int y_novoponto = 0;
        int k1 = 0;
        int k2 = 0;
        for(int x2=x_mar8aux2-7; x2<=x_mar8aux2+7;++x2){
            k2 = 0;
            for(int y2=y_mar8aux2-7; y2<=y_mar8aux2+7;++y2){
                if(V.GetPointIntensity(x2,y2)>100){
                    matrizAlgo21[k1][k2] = 1;
                } else {
                    matrizAlgo21[k1][k2] = 0;
                }
                if(k==113){  x_novoponto=x2; y_novoponto=y2; }
                //cout << matrizAlgo21[k1][k2] << " ";
                k2++;
            }
            //cout << endl;
            k1++;
        }

        //cout << endl;
        int result1=0;
        for(int c1=0; c1<=15;++c1){
            for(int c2=0; c2<=15;++c2){
                if(matrizAlgo2[c1][c2]!=matrizAlgo21[c1][c2]){
                    result1 = result1 + 1;
                }
            }
        }
        //cout << endl;
        //cout << "Resultado centro: " << result1 << " " <<endl ;



        //cout << "1" << endl;
        bool loop4=true;
        x_novoponto = 0;
        y_novoponto = 0;
        k1 = 0;
        k2 = 0;
        for(int x2=x_mar8aux2-14; x2<=x_mar8aux2;++x2){
            k2 = 0;
            for(int y2=y_mar8aux2-7; y2<=y_mar8aux2+7;++y2){
                if(V.GetPointIntensity(x2,y2)>100){
                    matrizAlgo24[k1][k2] = 1;
                } else {
                    matrizAlgo24[k1][k2] = 0;
                }
                if(k==113){  x_novoponto=x2; y_novoponto=y2; }
                //cout << matrizAlgo24[k1][k2] << " ";
                k2++;
            }
            //cout << endl;
            k1++;
        }

        //cout << endl;
        int result2=0;
        for(int c1=0; c1<=15;++c1){
            for(int c2=0; c2<=15;++c2){
                if(matrizAlgo2[c1][c2]!=matrizAlgo22[c1][c2]){
                    result2 = result2 + 1;
                }
            }
        }
        //cout << endl;
        //cout << "Resultado 1: " << result2 << " " <<endl ;


        //cout << "2" << endl;
        bool loop5=true;
        x_novoponto = 0;
        y_novoponto = 0;
        k1 = 0;
        k2 = 0;
        for(int x2=x_mar8aux2; x2<=x_mar8aux2+14;++x2){
            k2 = 0;
            for(int y2=y_mar8aux2-7; y2<=y_mar8aux2+7;++y2){
                if(V.GetPointIntensity(x2,y2)>100){
                    matrizAlgo25[k1][k2] = 1;
                } else {
                    matrizAlgo25[k1][k2] = 0;
                }
                if(k==113){  x_novoponto=x2; y_novoponto=y2; }
                //cout << matrizAlgo25[k1][k2] << " ";
                k2++;
            }
            //cout << endl;
            k1++;
        }

        //cout << endl;
        int result3=0;
        for(int c1=0; c1<=15;++c1){
            for(int c2=0; c2<=15;++c2){
                if(matrizAlgo2[c1][c2]!=matrizAlgo23[c1][c2]){
                    result3 = result3 + 1;
                }
            }
        }
        //cout << endl;
        //cout << "Resultado 2: " << result3 << " " <<endl ;


        //cout << "3" << endl;
        bool loop6=true;
        x_novoponto = 0;
        y_novoponto = 0;
        k1 = 0;
        k2 = 0;
        for(int x2=x_mar8aux2-7; x2<=x_mar8aux2+7;++x2){
            k2 = 0;
            for(int y2=y_mar8aux2-14; y2<=y_mar8aux2;++y2){
                if(V.GetPointIntensity(x2,y2)>100){
                    matrizAlgo26[k1][k2] = 1;
                } else {
                    matrizAlgo26[k1][k2] = 0;
                }
                if(k==113){  x_novoponto=x2; y_novoponto=y2; }
                //cout << matrizAlgo26[k1][k2] << " ";
                k2++;
            }
            //cout << endl;
            k1++;
        }

        //cout << endl;
        int result4=0;
        for(int c1=0; c1<=15;++c1){
            for(int c2=0; c2<=15;++c2){
                if(matrizAlgo2[c1][c2]!=matrizAlgo24[c1][c2]){
                    result4 = result4 + 1;
                }
            }
        }
        //cout << endl;
        //cout << "Resultado 3: " << result4 << " " <<endl ;


        //cout << "4" << endl;
        bool loop7=true;
        x_novoponto = 0;
        y_novoponto = 0;
        k1 = 0;
        k2 = 0;
        for(int x2=x_mar8aux2-7; x2<=x_mar8aux2+7;++x2){
            k2 = 0;
            for(int y2=y_mar8aux2; y2<=y_mar8aux2+14;++y2){
                if(V.GetPointIntensity(x2,y2)>100){
                    matrizAlgo27[k1][k2] = 1;
                } else {
                    matrizAlgo27[k1][k2] = 0;
                }
                if(k==113){  x_novoponto=x2; y_novoponto=y2; }
                //cout << matrizAlgo27[k1][k2] << " ";
                k2++;
            }
            //cout << endl;
            k1++;
        }

        //cout << endl;
        int result5=0;
        for(int c1=0; c1<=15;++c1){
            for(int c2=0; c2<=15;++c2){
                if(matrizAlgo2[c1][c2]!=matrizAlgo25[c1][c2]){
                    result5 = result5 + 1;
                }
            }
        }
        //cout << endl;
        //cout << "Resultado 4: " << result5 << " " <<endl ;
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

