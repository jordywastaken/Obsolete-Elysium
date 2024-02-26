
#pragma once

#include "globals.h"
#include "gui/offhost.h"

namespace submenus
{
    void aimassist(OffhostMenu& menu)
    {
        menu.Title("Aim assistance");
        menu.Toggle("No recoil", Config->NoRecoil);
        menu.Toggle("No spread", Config->NoSpread);
        menu.Toggle("No flinch", Config->NoFlinch);
        menu.Toggle("No kick", Config->NoKick);
        menu.Toggle("No sway", Config->NoSway);
        menu.Toggle("No bob", Config->NoBob);
        menu.Toggle("No weapon lean", Config->NoLean);

        menu.Option("Dump bones", []
            {
                centity_t* entity = 0;

                for (int i = 2; i < 700; i++)
                {
                    if (centity[i].pose.eType == ET_PLAYER)
                    {
                        entity = &centity[i];
                        break;
                    }
                }

                if (!entity)
                    return;

                printf("entity: 0x%p\n", entity);
                DObj* dobj = Com_GetClientDObj(entity->nextState.number, entity->pose.localClientNum);

                if (!dobj)
                    return;

                printf("dobj: 0x%p\n", dobj);
                XModel* model = dobj->models[0];

                if (!model)
                    return;

                printf("model: 0x%p '%s'\n", model, model->name);

                printf("index - sl name - bone name\n");
                for (int i = 0; i < model->numBones; i++)
                {
                    printf("%i - 0x%p - '%s'\n", i, model->boneNames[i], SL_ConvertToString(model->boneNames[i]));
                }
            });
    }
}