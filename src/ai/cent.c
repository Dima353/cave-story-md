#include "ai.h"

#include <genesis.h>
#include "audio.h"
#include "player.h"
#include "stage.h"
#include "tables.h"
#include "tsc.h"
#include "effect.h"
#include "camera.h"
#include "system.h"

// dragonfly creature
void ai_stumpy(Entity *e) {
	e->frame ^= 1;
	// Collide up front
	e->x_next = e->x + e->x_speed;
	e->y_next = e->y + e->y_speed;
	e->x_mark = e->x_speed; // Remember speed
	e->y_mark = e->y_speed;
	u8 blockl = e->x_speed < 0 && collide_stage_leftwall(e),
		blockr = e->x_speed > 0 && collide_stage_rightwall(e),
		blocku = e->y_speed < 0 && collide_stage_ceiling(e),
		blockd = e->y_speed > 0 && collide_stage_floor(e);
	// Some of the NXEngine states weren't necessary because we deactivate offscreen anyway
	switch(e->state) {
		case 0:
		{
			e->frame &= ~2;
			e->state = 1;
			e->timer = 0;
			FACE_PLAYER(e);
		}
		case 1:
		{
			if (++e->timer > TIME(50)) e->state = 2;
		}
		break;
		case 2:
		{
			e->state = 5;
			e->frame |= 2;
			e->timer = 0;
			// TODO: throw ourselves at player
			//ThrowEntityAtPlayer(o, 3, 0x400);
			e->dir = e->x_speed >= 0;
		}
		case 3:
		{
			if(blockl || blockr) e->x_speed = -e->x_mark;
			if(blocku || blockd) e->y_speed = -e->y_mark;
			if (++e->timer > TIME(50)) {
				e->state = 0;
				e->x_speed = 0;
				e->y_speed = 0;
			}
		}
		break;
	}
	e->x = e->x_next;
	e->y = e->y_next;
}

// very happy green guy who runs back and forth
void ai_midorin(Entity *e) {
	e->x_next = e->x + e->x_speed;
	e->y_next = e->y + e->y_speed;
	if(e->grounded) e->grounded = collide_stage_floor_grounded(e);
	else e->grounded = collide_stage_floor(e);
	
	switch(e->state) {
		case 0:
		{
			e->state = 1;
			e->timer = 0;
			e->frame = 0;
			e->x_speed = 0;
		}
		case 1:
		{
			if (!(random() % 32)) {
				e->state = 2 + (random() & 1);
				e->frame = 1;
			}
		}
		break;
		
		case 2:		// blinking
		{
			if (++e->timer > TIME(8)) {
				e->state = 1;
				e->timer = 0;
				e->frame = 0;
			}
		}
		break;
		
		case 3:		// running
		{
			e->state = 4;
			e->frame = 0;	// this will be toggled into frame 2 just below
			e->animtime = 0;
			
			e->timer = 32 + (random() % 24);		// how long to run
			e->dir = random() & 1;
		}
		case 4:
		{
			if (e->animtime ^= 1) e->frame ^= 2;
			
			if ((!e->dir && collide_stage_leftwall(e)) ||
				(e->dir && collide_stage_rightwall(e))) {
				e->dir ^= 1;
			}
			
			MOVE_X(SPEED(0x400));
			
			if (--e->timer <= 0) e->state = 0;
		}
		break;
	}
	
	if(!e->grounded) e->y_speed += SPEED(0x20);
	LIMIT_Y(SPEED(0x5ff));
	e->x = e->x_next;
	e->y = e->y_next;
}

// large bat with many baby bats
void ai_orangebell(Entity *e) {
	e->x_next = e->x + e->x_speed;
	e->y_next = e->y + e->y_speed;
	switch(e->state)
	{
		case 0:
		{
			e->state = 1;
			e->y_mark = e->y;
			e->y_speed = SPEED(0x200);
			
			// create baby bats
			for(u8 i=0;i<8;i++) {
				Entity *bat = entity_create(e->x - 0x1000 + (random() % 0x2000),
										   e->y - 0x1000 + (random() % 0x2000),
										   OBJ_ORANGEBELL_BABY, 0);
				bat->linkedEntity = e;
			}
		}
		case 1:
		{
			ANIMATE(e, 5, 0,1,2);
			
			if ((!e->dir && collide_stage_leftwall(e)) ||
				(e->dir && collide_stage_rightwall(e))) {
				e->dir ^= 1;
			}
			
			MOVE_X(SPEED(0x100));
			
			e->y_speed += (e->y < e->y_mark) ? 0x08 : -0x08;
			LIMIT_Y(SPEED(0x200));
		}
		break;
	}
	e->x = e->x_next;
	e->y = e->y_next;
}

void ai_orangebell_baby(Entity *e) {
	switch(e->state) {
		case 0:
		{
			e->x_speed = sintab32[random() % 1024] * 0x100;
			e->y_speed = sintab32[random() % 1024] * 0x100;
			
			e->timer = 0;	// time until can dive-bomb
			// unique target point on main bat
			e->y_next = -(32<<CSF) + (random() % (32<<CSF));
			
			e->state = 1;
		}
		case 1:
		{
			ANIMATE(e, 4, 0,1,2);
			
			if (e->linkedEntity) {
				e->x_mark = e->linkedEntity->x;
				e->y_mark = e->linkedEntity->y + e->y_next;
				e->dir = e->linkedEntity->dir;
			}
			
			e->x_speed += (e->x < e->x_mark) ? 0x08 : -0x08;
			e->y_speed += (e->y < e->y_mark) ? 0x20 : -0x20;
			LIMIT_X(SPEED(0x400));
			LIMIT_Y(SPEED(0x400));
			
			// dive-bomb
			if (e->timer) e->timer--;
			if (PLAYER_DIST_X(8 << CSF) && !e->timer) {
				if (player.y > e->y && ((player.y - e->y) < 175<<CSF)) {
					e->x_speed /= 4;
					e->y_speed = 0;
					e->state = 2;
				}
			}
		}
		break;
		
		case 2:		// dive-bombing
		{
			e->frame = 3;
			e->y_speed += SPEED(0x40);
			LIMIT_Y(SPEED(0x5ff));
			// Quick floor check
			if (blk(e->x, 0, e->y, 8) == 0x41) {
				e->x_speed *= 2;
				e->y_speed = 0;
				e->timer = TIME(120);
				
				e->state = 1;
			}
		}
		break;
	}
	e->x += e->x_speed;
	e->y += e->y_speed;
}

void ai_gunfish(Entity *e) {
	switch(e->state) {
		case 0:
		{
			e->y_speed = 0;
			e->y_mark = e->y;
			
			e->state = 1;
			e->timer = random() % TIME(50);
		}
		case 1:		// desync
		{
			if (e->timer == 0) {
				e->y_speed = SPEED(0x200);
				e->state = 2;
				e->timer = 0;
			} else e->timer--;
		}
		break;
		
		case 2:
		{
			ANIMATE(e, 4, 0,1);
			FACE_PLAYER(e);
			
			if (PLAYER_DIST_X(128<<CSF) && PLAYER_DIST_Y2(160<<CSF, 20<<CSF)) {
				if (++e->timer > TIME(80)) {
					e->state = 10;
					e->timer = 0;
					e->frame += 2;
				}
			}
		}
		break;
		
		case 10:
		{
			ANIMATE(e, 4, 2,3);
			
			if (++e->timer > TIME(20)) {
				e->state = 20;
				e->timer = 0;
				e->frame += 2;
			}
		}
		break;
		
		case 20:
		{
			ANIMATE(e, 4, 4,5);
			
			if ((++e->timer % 10) == 3) {
				Entity *shot = entity_create(e->x, e->y, OBJ_GUNFISH_SHOT, 0);
				
				shot->x_speed = e->dir ? SPEED(0x400) : -SPEED(0x400);
				shot->y_speed = -SPEED(0x400);
				
				sound_play(SND_EM_FIRE, 3);
			}
			
			if (e->timer > TIME(60)) {
				e->state = 2;
				e->timer = 0;
				e->frame -= 4;
			}
		}
		break;
	}
	
	e->y_speed += (e->y < e->y_mark) ? 0x10 : -0x10;
	LIMIT_Y(SPEED(0x100));
	e->x += e->x_speed;
	e->y += e->y_speed;
}

void ai_gunfish_shot(Entity *e) {
	e->x_next = e->x + e->x_speed;
	e->y_next = e->y + e->y_speed;
	// Delete after hitting a wall or water
	if ((e->x_speed < 0 && collide_stage_leftwall(e)) ||
		(e->x_speed > 0 && collide_stage_rightwall(e)) ||
		(e->y_speed < 0 && collide_stage_ceiling(e)) ||
		(e->y_speed > 0 && collide_stage_floor(e)) ||
		(blk(e->x, 0, e->y, -4) & 0x10)) {
		//for(u8 i=0;i<5;i++) {
		//	Caret *c = effect(e->CenterX(), e->CenterY(), EFFECT_GUNFISH_BUBBLE);
		//	c->x_speed = random(-0x400, 0x400);
		//	c->y_speed = random(-0x400, 0);
		//}
		sound_play(SND_BUBBLE, 3);
		e->state = STATE_DELETE;
	}
	
	e->y_speed += SPEED(0x20);
	LIMIT_Y(SPEED(0x5ff));
	e->x = e->x_next;
	e->y = e->y_next;
}

void ai_droll(Entity *e) {
	e->x_next = e->x + e->x_speed;
	e->y_next = e->y + e->y_speed;
	if(e->grounded) e->grounded = collide_stage_floor_grounded(e);
	else e->grounded = collide_stage_floor(e);
	switch(e->state) {
		case 0:
		{
			e->x_mark = e->x;
			e->state = 1;
		}
		case 1:
		{
			ANIMATE(e, 40, 0,1);
			FACE_PLAYER(e);
			
			if (e->damage_time) e->state = 10;
		}
		break;
		
		case 10:	// prepare to jump
		{
			e->state = 11;
			e->frame = 2;
			e->timer = 0;
		}
		case 11:
		{
			if (++e->timer > 10) {	// jump
				e->state = 12;
				e->frame = 6;
				e->timer2 = 0;		// have not fired yet
				
				e->x_speed = (e->x >= e->x_mark) ? -SPEED(0x200) : SPEED(0x200);
				e->y_speed = -SPEED(0x600);
			}
		}
		break;
		
		case 12:	// in air
		{
			if (e->y_speed >= 0) {
				if (!e->timer2)	{	// have fired yet?
					FACE_PLAYER(e);
					e->timer2 = 1;
					e->frame = 7;
					
					FIRE_ANGLED_SHOT(OBJ_DROLL_SHOT, e->x, e->y, e->dir ? A_RIGHT : A_LEFT, 0x600);
					sound_play(SND_EM_FIRE, 5);
				} else if (e->y_speed > SPEED(0x200)) {	// after-fire frame
					e->frame = 3;
				}
				
				if (e->grounded) {
					//e->y >>= CSF; e->y <<= CSF;
					e->x_speed = 0;
					
					e->state = 13;
					e->frame = 2;
					e->timer = 0;
				}
			}
		}
		break;
		
		case 13:	// landed
		{
			e->x_speed /= 2;
			
			if (++e->timer > 10) {
				e->x_speed = 0;
				e->state = 1;
			}
		}
		break;
	}
	
	if(!e->grounded) e->y_speed += SPEED(0x55);
	LIMIT_Y(SPEED(0x5ff));
	e->x = e->x_next;
	e->y = e->y_next;
}

void ai_droll_shot(Entity *e) {
	e->x += e->x_speed;
	e->y += e->y_speed;
	ANIMATE(e, 4, 0,1,2);
	
	if ((++e->timer % 8) == 0) sound_play(SND_DROLL_SHOT_FLY, 3);
	
	if (blk(e->x, 0, e->y, 0) & 0x41) {
		effect_create_smoke(e->x >> CSF, e->y >> CSF);
		//effect(e->CenterX(), e->CenterY(), EFFECT_BOOMFLASH);
		e->state = STATE_DELETE;
	}
}

// stomps you in Teleporter Room
void ai_droll_guard(Entity *e) {
	e->x_next = e->x + e->x_speed;
	e->y_next = e->y + e->y_speed;
	if(e->grounded) e->grounded = collide_stage_floor_grounded(e);
	else e->grounded = collide_stage_floor(e);
	switch(e->state) {
		case 0:
		{
			e->x += (8 << CSF);
			e->state = 1;
		}
		case 1:
		{
			ANIMATE(e, 30, 0,1);
			FACE_PLAYER(e);
		}
		break;
		
		case 10:
		{
			e->timer = 0;
			e->state = 11;
		}
		case 11:
		{
			e->frame = 2;
			if (++e->timer > 10) {
				e->state = 12;
				e->frame = 3;
				e->y_speed = -0x600;
				MOVE_X(0x200);
			}
		}
		break;
		
		case 12:	// in air...
		{
			if (e->y_speed >= 0 && e->grounded) {
				e->frame = 2;
				e->state = 13;
				e->timer = 0;
			}
		}
		break;
		
		case 13:	// landed
		{
			e->x_speed /= 2;
			if (++e->timer > 10) {
				e->x_speed = 0;
				e->frame = 0;
				e->state = 1;
			}
		}
		break;
	}
	
	if(!e->grounded) e->y_speed += 0x40;
	LIMIT_Y(0x5ff);
	e->x = e->x_next;
	e->y = e->y_next;
}

void ai_mimiga_farmer(Entity *e) {
	e->x_next = e->x + e->x_speed;
	e->y_next = e->y + e->y_speed;
	if(e->grounded) e->grounded = collide_stage_floor_grounded(e);
	else e->grounded = collide_stage_floor(e);
	switch(e->state) {
		case 0:
		{
			e->frame = 0;
			e->x_speed = 0;
			e->state = 1;
		}
		case 1:
		{
			if (!(random() % 60)) {
				if (e->type != OBJ_MIMIGA_FARMER_STANDING && (random() & 1)) {	// walk
					e->state = 10;
				} else {	// blink
					e->state = 2;
					e->frame = 1;
				}
			}
		}
		break;
		case 2:		// blinking
		{
			if (++e->timer >= 8) {
				e->state = 0;
				e->timer = 0;
			}
		}
		break;
		
		case 10:	// walk
		{
			e->state = 11;
			e->frame = 2;
			e->animtime = 0;
			
			e->timer = 16 + (random() % 16);
			e->dir = random() & 1;
		}
		case 11:
		{
			if ((!e->dir && collide_stage_leftwall(e)) ||
				(e->dir && collide_stage_rightwall(e))) {
				e->dir ^= 1;
			}
			
			MOVE_X(SPEED(0x200));
			ANIMATE(e, 4, 2,3,4,5);
			
			if (!--e->timer)
				e->state = 0;
		}
		break;
	}
	
	if(!e->grounded) e->y_speed += 0x20;
	LIMIT_Y(SPEED(0x5ff));
	e->x = e->x_next;
	e->y = e->y_next;
}


void onspawn_mimiga_cage(Entity *e) {
	e->state = 1;
	e->x -= (16 << CSF);
}

void ai_npc_itoh(Entity *e) {
	e->x_next = e->x + e->x_speed;
	e->y_next = e->y + e->y_speed;
	if(e->grounded) e->grounded = collide_stage_floor_grounded(e);
	else e->grounded = collide_stage_floor(e);
	switch(e->state) {
		case 0:
		{
			e->state = 1;
		}
		case 1:
		{
			e->x_speed = 0;
			e->frame = 0;
			RANDBLINK(e, 1, 200);
		}
		break;
		
		case 10:		// cower
		{
			e->frame = 2;
			e->x_speed = 0;
		}
		break;
		
		case 20:		// leap away
		{
			e->state = 21;
			e->frame = 2;
			e->x_speed = 0x200;
			e->y_speed = -0x400;
		}
		case 21:
		{
			if (e->grounded && e->y_speed >= 0) {
				e->x_speed = 0;
				
				e->frame = 3;
				e->state = 30;
				e->timer = 0;
			}
		}
		break;
		
		case 30:		// shaking after leap
		{
			e->x_mark = e->x;
			e->state = 31;
		}
		case 31:
		{
			e->x = e->x_mark;
			if (++e->timer & 2) e->x += (1 << CSF);
		}
		break;
		
		case 40:	// small vertical hop (from when he jumps out of helicopter at end)
		{
			e->state = 41;
			e->y_speed = -0x200;
			e->frame = 2;
		}
		case 41:
		{
			if (e->grounded && e->y_speed > 0) {
				e->x_speed = 0;
				e->state = 42;
				e->frame = 0;
				e->dir = 1;
			}
		}
		break;
		case 42:	// stop begin shocked after blocks fall (same as 0 except he doesn't blink)
		{
			e->frame = 0;
			e->dir = 1;
		}
		break;
		
		case 50:	// run a moment then stop
		{
			e->state = 51;
			e->timer = 0;
			e->animtime = 0;
			e->dir = 1;
			e->frame = 5;
		}
		case 51:
		{
			ANIMATE(e, 4, 4,5,6,7);
			MOVE_X(0x200);
			
			if (++e->timer > 32) {
				e->frame = 0;
				e->x_speed = 0;
				e->state = 52;
			}
		}
		break;
	}
	
	if(!e->grounded) e->y_speed += 0x40;
	LIMIT_Y(SPEED(0x5ff));
	e->x = e->x_next;
	e->y = e->y_next;
}

void ai_kanpachi_standing(Entity *e) {
	switch(e->state) {
		case 10:	// walking
		case 11:
		{
			e->state = 3;
		}
		break;
		
		case 20:	// face away/enter door
		{
			e->x_speed = 0;
			e->frame = 6;
		}
		break;
		
		default:
			generic_npc_states(e);
		break;
	}
}


void ai_npc_momorin(Entity *e) {
	switch(e->state) {
		case 0:
			e->frame = 0;
			RANDBLINK(e, 1, 200);
			FACE_PLAYER(e);
		break;
		
		case 3:		// surprised
			e->frame = 2;
		break;
	}
}

void ai_proximity_press_hoz(Entity *e) {
	switch(e->state) {
		case 0:
		{
			//if (!e->dir) e->x -= (8 << CSF);
			e->x_mark = e->x;
			
			e->state = 1;
		}
		case 1:
		{
			if (PLAYER_DIST_Y2(0x800, 0x1000)) {
				if (!e->dir) {
					if (player.x < e->x) {
						if ((e->x - player.x) <= (192<<CSF)) {
							e->state = 2;
							e->frame = 2;
							e->timer = 0;
						}
					}
				} else {
					if (player.x > e->x) {
						if ((player.x - e->x) <= (192<<CSF)) {
							e->state = 2;
							e->frame = 2;
							e->timer = 0;
						}
					}
				}
			}
		}
		break;
		
		case 2:		// activated
		{
			e->attack = 127;
			MOVE_X(SPEED(0xC00));
			
			if (++e->timer == TIME(8)) {
				sound_play(SND_BLOCK_DESTROY, 5);
				//SmokeSide(o, 4, e->dir);
			}
			
			if (e->timer > TIME(8)) {
				e->attack = 0;
				e->x_speed = 0;
				e->state = 3;
				e->timer = 0;
			}
		}
		break;
		
		case 3:		// hit other press
		{
			if (++e->timer > TIME(50)) {
				e->state = 4;	// return
				e->frame = 1;
				e->timer = 0;
			}
		}
		break;
		
		case 4:		// return to start pos
		{
			MOVE_X(-SPEED(0x800));
			
			if (++e->timer > TIME(10)) {
				e->frame = 0;
				e->x_speed = 0;
				e->x = e->x_mark;
				
				e->state = 1;
				e->timer = 0;
			}
		}
		break;
	}
}

void ai_rocket(Entity *e) {
	if(e->state) {
		e->x_next = e->x + e->x_speed;
		e->y_next = e->y + e->y_speed;
	}
	switch(e->state) {
		case 10:	// activated
		{
			e->state = 11;
			e->timer = 0;
			e->alwaysActive = TRUE;
		}
		case 11:
		{
			e->timer++;
			e->y_speed += SPEED(8);
			
			if (collide_stage_floor(e)) {
				if (e->timer < 10) {
					e->state = 12;
				} else {
					e->state = 0;
				}
			}
		}
		break;
		
		case 12:
		{
			e->eflags &= ~NPC_INTERACTIVE;
			e->state = 13;
			e->timer = 0;
			e->frame = 1;
			
			//SmokeClouds(o, 10, 16, 8);
			sound_play(SND_BLOCK_DESTROY, 5);
		}
		case 13:
		{
			e->y_speed -= SPEED(0x08);
			e->timer++;
			//SpawnRocketTrail(o, (e->timer & 1) ? RIGHT : LEFT);
			
			if ((e->timer % 4) == 1) sound_play(SND_FIREBALL, 3);
			
			if (collide_stage_ceiling(e)) {
				//SmokeClouds(o, 6, 16, 8);
				sound_play(SND_BLOCK_DESTROY, 5);
				
				e->y_speed = 0;
				e->state = 14;
			}
		}
		break;
		
		case 14:		// falling back down
		{
			e->y_speed += 8;
			e->timer++;
			
			if (e->y_speed < 0) {
				if ((e->timer % 16) == 1) sound_play(SND_FIREBALL, 3);
			} else if (collide_stage_floor(e)) {
				//e->y >>= CSF; e->y <<= CSF;
				e->eflags |= NPC_INTERACTIVE;
				e->frame = 0;
				e->state = 0;
				e->alwaysActive = FALSE;
			}
		}
		break;
	}
	LIMIT_Y(SPEED(0x5ff));
	if(e->state) {
		e->x = e->x_next;
		e->y = e->y_next;
		// Hack to prevent player from clipping off to the side
		if(!player.jump_time && PLAYER_DIST_X(18 << CSF) && PLAYER_DIST_Y2(18 << CSF, 0)) {
			player.y = e->y - (16 << CSF);
		}
	}
}

// in the Statue Room, if you enter via the Plantation
void ai_numahachi(Entity *e) {
	ANIMATE(e, 50, 0,1);
}