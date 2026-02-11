#include <KML/KML.h>
#include <iostream>
#include <cstdlib>

using namespace KML;

bool exitGame = false;

void GamePhysics();

int main() {
    SetGLContextVersion(3, 3);
    InitWindow(320, 180, "Conditioned Subject", RESIZABLE | MSAA8 | ENABLE_VSYNC);
    PrintContext();

    int fps = 0;
    int tps = 0;
    double timeCounter = 0.0;

    double timeAccum = 0.0;
    Clock g_clock;

    constexpr int TARGET_TPS = 20;
    constexpr double TICK_LENGTH = 1.0 / (double)TARGET_TPS; 

    while(!exitGame) {
        double deltaTime = g_clock.Tick();
        timeAccum += deltaTime;

        if (!ProcessEvents()) exitGame = true;

        while(timeAccum >= TICK_LENGTH) {
            GamePhysics();
            timeAccum -= TICK_LENGTH;
            tps++;
        }

        fps++;
        timeCounter += deltaTime;

        if(timeCounter >= 1.0) {
            std::cout << "In " << timeCounter << " seconds the game rendered " << fps << " frames and simulated " << tps << " ticks\n";
            char buffer[64] = {0};
            sprintf(buffer, "Conditioned Subject | %d FPS - %d TPS", fps, tps);
            SetWindowTitle(buffer);
            timeCounter -= 1.0; 
            fps = 0;
            tps = 0;
        }

        PresentFrame(0.64f, 0.23f, 0.72f);
    }

    Quit();
    return 0;
}

void GamePhysics() {

}