#include "KML/Layers.h"
#include "__KML/graphics.h"

std::vector<KML::Layer*>layers;

KML::Layer::Layer(int maxSurfaces) {
    max = maxSurfaces;
    layers.push_back(this);
}

KML::Layer::Layer() {
    max = 999999;
    layers.push_back(this);
}

void KML::Layer::Draw() {
    int n = 0;
    for(auto& i : srf) {
        if(n >= max) break;
        if(i != nullptr) i->Draw();
        n++;
    }
} 

void KML::DrawLayers(int maxLayers) {
    int n = 0;
    for(auto& layer : layers) {
        if(n >= maxLayers && maxLayers > 0) break;
        if(layer != nullptr) layer->Draw();
        n++;
    }
}

void KML::Layer::Add(Surface& __s) {
    srf.push_back(&__s);
}