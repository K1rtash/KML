#ifndef KML_LAYERS_H
#define KML_LAYERS_H

#include <vector>

#include "Surface.h"

namespace KML {
    class Layer {
        std::vector<Surface*> srf;
        int max;
      public:
        Layer();
        Layer(int maxSurfaces);
        void Draw();
        void Add(Surface& __s);
    };

    void DrawLayers(int maxLayers = 0);

}

#endif