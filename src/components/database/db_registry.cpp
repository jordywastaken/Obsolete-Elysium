
#include "db_registry.h"
#include "components/scheduler.h"

Font* normalFont;
Font* smallFont;
Font* bigFont;
Font* boldFont;
Material* white;
Material* gradient;
Material* darkmater_camo;

namespace db_registry
{
    void start()
    {
        scheduler::loop(scheduler::renderer, []
            {
                LUI_Interface_RegisterFont("fonts/normalFont", &normalFont);
                LUI_Interface_RegisterFont("fonts/smallFont", &smallFont);
                LUI_Interface_RegisterFont("fonts/bigFont", &bigFont);
                LUI_Interface_RegisterFont("fonts/boldFont", &boldFont);

                LUI_Interface_RegisterMaterial("white", &white);
                LUI_Interface_RegisterMaterial("gradient_gay", &gradient);
                LUI_Interface_RegisterMaterial("mc/if_you_read_this_you_are_gay", &darkmater_camo);
            }, 0);
    }
}