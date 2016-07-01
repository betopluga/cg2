#ifndef MARCADOR_H
#define MARCADOR_H
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>


class Marcador
{
    public:
        Marcador() {}
        int id_marcador;
        int num_frame;
        int x_centro;
        int y_centro;
        std::vector<std::string> pixels_xy;
        int id_proximo;

    protected:

    private:
};

#endif // MARCADOR_H
