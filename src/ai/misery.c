#include "ai.h"

#include <genesis.h>
#include "audio.h"
#include "player.h"
#include "stage.h"
#include "tables.h"
#include "tsc.h"
#include "vdp_ext.h"

void ai_misery_float(Entity *e) {
	e->x_next = e->x + e->x_speed;
	e->y_next = e->y + e->y_speed;
	switch(e->state) {
		case 0:
		{
			e->state = 1;
			e->x_next += (1 << CSF);	// check Undead Core intro to prove this is correct
			e->x_mark = e->x_next;
			e->y_mark = e->y_next;
			e->frame = 0;
			e->timer = 0;
		}
		/* no break */
		case 1:
		{
			//if (DoTeleportIn(o, 1)) {
				e->state = 10;
			//}
		}
		break;
		case 10:	// floating
		{
			e->state = 11;
			e->timer = 0;
			e->y_speed = SPEED(1 << CSF);
		}
		/* no break */
		case 11:
		{
			if (e->y_next > e->y_mark) e->y_speed -= SPEED(16);
			if (e->y_next < e->y_mark) e->y_speed += SPEED(16);
			if (e->y_speed > SPEED(0x100)) e->y_speed = SPEED(0x100);
			if (e->y_speed < SPEED(-0x100)) e->y_speed = SPEED(-0x100);
		}
		break;
		case 13:	// fall from floaty
			e->frame = 1;
			e->y_speed += SPEED(0x40);
			LIMIT_Y(SPEED(0x5FF));
			
			if ((e->grounded = collide_stage_floor(e))) {
				sound_play(SND_THUD, 5);
				e->state = 14;
			}
		break;
		case 14: break;			// standing
		// spawn the bubble which picks up Toroko in Shack
		case 15:
		{
			e->frame = 2;
			e->timer = 0;
			e->state = 16;
		}
		/* no break */
		case 16:
		{
			if (++e->timer >= TIME(20)) {
				sound_play(SND_BUBBLE, 5);
				entity_create(e->x, e->y - (16<<CSF), OBJ_MISERYS_BUBBLE, 0);
				e->state = 17;
				e->timer = 0;
			}
		}
		break;
		case 17:
		{
			if (++e->timer >= TIME(50)) e->state = 14;
		}
		/* no break */
		case 20: 	// fly away
		{
			e->state = 21;
			e->frame = 0;
			e->y_speed = 0;
		}
		/* no break */
		case 21:
		{
			e->y_speed -= SPEED(0x20);
			if (e->y_next < -0x1000) e->state = STATE_DELETE;
		}
		break;
		case 25:	// big spell
		{
			e->state = 26;
			e->timer = 0;
			e->frame = 2;
		}
		/* no break */
		case 26:	// she flashes, then a clap of thunder
		{
			if (++e->timer >= TIME(20)) {
				sound_play(SND_LIGHTNING_STRIKE, 5);
				// Flash screen white
				VDP_setPaletteColors(0, PAL_FullWhite, 64);
				VDP_fadeTo(0, 63, VDP_getCachedPalette(), 10, true);
				e->state = 27;
				e->timer = 0;
			}
		}
		break;
		case 27:	// return to standing after lightning strike
		{
			if (++e->timer > TIME(16)) e->state = 14;
		}
		break;
	}
	e->x = e->x_next;
	e->y = e->y_next;
}

void ai_misery_bubble(Entity *e) {
	// find the Toroko object we are to home in on
	Entity *target = entity_find_by_type(0x3C);
	if(target == NULL) {
		e->state = STATE_DELETE;
		return;
	}
	switch(e->state) {
		case 0:
		{
			// Wait a bit
			if(++e->timer > TIME(20)) e->state = 1;
		}
		break;
		case 1:
		{
			// Calculate the speed it will take to reach the target in 1 second
			// Genesis can't divide 32 bit integers so here is a fun hack have fun deciphering it
			e->x_speed = (((s32)((u16)(abs(target->x - e->x) >> 5)) / TIME(50))) << 5;
			e->y_speed = (((s32)((u16)(abs(target->y - e->y) >> 5)) / TIME(50))) << 5;
			if(e->x > target->x) e->x_speed = -e->x_speed;
			if(e->y > target->y) e->y_speed = -e->y_speed;
			e->state = 2;
			e->timer = 0;
		}
		/* no break */
		case 2:
		{
			e->x += e->x_speed;
			e->y += e->y_speed;
			// Did we reach the target?
			if(++e->timer == TIME(50)) {
				sound_play(SND_BUBBLE, 5);
				e->state = 3;
				e->x = target->x;
				e->y = target->y;
				e->x_speed = 0;
				e->y_speed = 0;
				target->eflags |= NPC_IGNORESOLID;
				//SPR_SAFEANIM(target->sprite, 1);
			}
		}
		break;
		case 3: // Carry Toroko away
		{
			e->y_speed -= SPEED(0x1C);
			e->y += e->y_speed;
			target->x = e->x + 0x200;
			target->y = e->y - 0x200;
		}
		break;
	}
}
