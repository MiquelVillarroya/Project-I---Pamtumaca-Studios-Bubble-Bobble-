#include "Game.h"
#include "MemLeaks.h"
#include "Globals.h"
#include "time.h"

int main()
{
    ReportMemoryLeaks();

    Game *game;
    AppStatus status;
    int main_return = EXIT_SUCCESS;

    LOG("Application start");
    SetRandomSeed(time(NULL));

    InitAudioDevice();
    if (!IsAudioDeviceReady()) LOG("Failed to initialise Audio Device");

    game = new Game();
    status = game->Initialise(GAME_SCALE_FACTOR);
    if (status != AppStatus::OK)
    {
        LOG("Failed to initialise game");
        main_return = EXIT_FAILURE;
    }
        
    while (status == AppStatus::OK)
    {
        status = game->Update(GAME_SCALE_FACTOR);
        if(status != AppStatus::OK)
        {
            if(status == AppStatus::ERROR)      main_return = EXIT_FAILURE;
            else if(status == AppStatus::QUIT)  main_return = EXIT_SUCCESS;
            break;
        }
        game->Render(GAME_SCALE_FACTOR);
    }

    LOG("Application finish");
    game->Cleanup();

    CloseAudioDevice();
    if (IsAudioDeviceReady()) LOG("Failed to close Audio Device");

    LOG("Bye :)");
    delete game;

    return main_return;
}
