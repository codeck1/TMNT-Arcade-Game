#include "Enemy.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"


Enemy::Enemy()
{
}

Enemy::Enemy(const Enemy & e) : up(e.up), upLeft(e.upLeft),right(e.right), left(e.left),
attack1(e.attack1), attack1Left(e.attack1Left), attack2(e.attack2),attack2Left(e.attack2Left),
position(e.position),eliminated(e.eliminated), colliderFeet(e.colliderFeet), colliderBody(e.colliderBody),
colliderWeapon(e.colliderWeapon),colliderJump(e.colliderJump), faceRight(e.faceRight),
jumped(e.jumped),attacking(e.attacking),currentState(e.currentState), graphics(e.graphics),
reciveDamage(e.reciveDamage), reciveDamageLeft(e.reciveDamageLeft),reciveDamage2(e.reciveDamage2),
reciveDamage2Left(e.reciveDamage2Left), reciveDamage3(e.reciveDamage3), reciveDamage3Left(e.reciveDamage3Left)
{
}

Enemy::~Enemy()
{
}

bool Enemy::Start()
{
	return true;
}

bool Enemy::Update()
{

	walk.x = 0;
	walk.y = 0;
	switch (currentState)
	{
	case ENEMYIDLE:
		if (shootImpact)
			shootImpact = false;
		if (abs(App->player->position.x - position.x) > SCREEN_WIDTH/4)
			{
				currentState = GOINGX;
			}
			else
			{
				if (abs(App->player->position.y - position.y) > 0)
				{
					currentState = GOINGY;
				}
				else
				{
					if (abs(App->player->position.x - position.x) > 0)
					{
						currentState = GOINGX;
					}
				}
			}
			break;

	case GOINGY:
		if (App->player->jumped)
			currentState = GOINGX;
		if(abs(App->player->position.x - position.x) <= SCREEN_WIDTH / 4)
		{
			if ((App->player->position.y - position.y) < 0)
			{
				
				walk.y = -1;
				if (faceRight)
				{
					if (current_animation != &up)
					{
						current_animation = &up;
					}
				}
				else
					if (current_animation != &upLeft)
					{
						current_animation = &upLeft;
					}
			}
			else
			{
				if ((App->player->position.y - position.y) > 0)
				{
					
					walk.y = 1;
					if (faceRight)
					{
						if (current_animation != &right)
						{
							current_animation = &right;
						}
					}
					else
						if (current_animation != &left)
						{
							current_animation = &left;
						}
				}
				else
					currentState = ENEMYIDLE;
			}
		}
		else
			currentState = ENEMYIDLE;
		
		break;

	case GOINGX:
		if(abs(App->player->position.y - position.y) == 0 || abs(App->player->position.x - position.x) > SCREEN_WIDTH / 4)
		{
			if ((App->player->position.x - position.x) < 0)
			{
				faceRight = false;
				if (App->player->currentState == TAKEDDOWN && abs(App->player->position.x - position.x) <= 150)
				{
					faceRight = true;
					current_animation = &right;
					walk.x = +2;
					break;
				}
				else
					if (abs(App->player->position.x - position.x) <= 100 && abs(App->player->position.x - position.x) > 90)
					{
						currentState = ENEMYATTACKING;
						break;
					}
				if ((abs(App->player->position.x - position.x) <= 25))
				{
					currentState = ENEMYATTACKING;
					break;
				}
				walk.x = -2;
				if (current_animation != &left)
				{
					current_animation = &left;
				}
			}
			else
			{
				if ((App->player->position.x - position.x) > 0)
				{
					faceRight = true;
					if (App->player->currentState == TAKEDDOWN && abs(App->player->position.x - position.x) <= 150)
					{
						faceRight = false;
						walk.x = -2;
						current_animation = &left;
						break;
					}
					else
						if (abs(App->player->position.x - position.x) <= 100 && abs(App->player->position.x - position.x) > 90)
						{
							currentState = ENEMYATTACKING;
							break;
						}
					if ((abs(App->player->position.x - position.x) <= 25))
					{
						currentState = ENEMYATTACKING;
						break;
					}
					walk.x = +2;
					if (current_animation != &right)
					{
						current_animation = &right;
					}
				}
				else
					currentState = ENEMYIDLE;
			}
		}
		else
			currentState = ENEMYIDLE;
		break;
		
	case ENEMYATTACKING:
		if (App->player->currentState == BEINGATTACKED)
		{
			if (colliderWeapon != nullptr && attacking)
			{
				if (App->player->faceRight && faceRight)
					App->player->sameDirection = true;
				else
					if(!App->player->faceRight && !faceRight)
						App->player->sameDirection = true;
					else
						App->player->sameDirection = false;
				colliderWeapon = App->collision->DeleteCollider(colliderWeapon);
			}
				
			break;
		}
		if (App->player->currentState == TAKEDDOWN)
		{
			if (colliderWeapon != nullptr)
			{
				currentState = ENEMYIDLE;
				colliderWeapon = App->collision->DeleteCollider(colliderWeapon);
			}
		}
		if ((abs(App->player->position.x - position.x) <= 150) && current_animation != &attack2 && current_animation != &attack2Left )
		{
			if ((abs(App->player->position.x - position.x) <= 25))
			{
				if (faceRight)
				{
					if (current_animation != &attack1)
					{
						attacking = true;
						colliderWeapon = App->collision->AddCollider({ position.x + 30, position.y + 10, 45, 15 }, COLLIDER_ENEMY_WEAPON, (Module*)App->enemy);
						attack1.Reset();
						current_animation = &attack1;
					}
				}
				else
				{
					if (current_animation != &attack1Left)
					{
						attacking = true;
						colliderWeapon = App->collision->AddCollider({ position.x - 15, position.y + 20, 30, 15 }, COLLIDER_ENEMY_WEAPON, (Module*)App->enemy);
						attack1Left.Reset();
						current_animation = &attack1Left;
					}
				}
			}
			else
			{
				if ((abs(App->player->position.x - position.x) <= 130) && (abs(App->player->position.x - position.x) > 70))
				{
					if (eType == TYPE1)
					{
						if (faceRight)
						{
							if (current_animation != &attack2)
							{
								attacking = true;
								jumped = true;
								colliderWeapon = App->collision->AddCollider({ position.x + 30, position.y + 20, 30, 15 }, COLLIDER_ENEMY_WEAPON, (Module*)App->enemy);
								attack2.Reset();
								current_animation = &attack2;
							}
						}
						else
						{
							if (current_animation != &attack2Left)
							{
								attacking = true;
								jumped = true;
								colliderWeapon = App->collision->AddCollider({ position.x - 30, position.y + 20, 30, 15 }, COLLIDER_ENEMY_WEAPON, (Module*)App->enemy);
								attack2Left.Reset();
								current_animation = &attack2Left;
							}
						}
					}
					else
					{
						if (faceRight)
						{
							if (current_animation != &attack2 && App->player->currentState != TAKEDDOWN)
							{
								App->particles->AddParticle(App->particles->star, position.x + 25, position.y+10, 3, this, COLLIDER_ENEMY_SHOT);
								attacking = true;
								attack2.Reset();
								colliderWeapon = App->collision->AddCollider({ position.x + 30, position.y + 20, 30, 15 }, COLLIDER_ENEMY_WEAPON, (Module*)App->enemy);
								current_animation = &attack2;
							}
						}
						else
						{
							if (current_animation != &attack2Left && App->player->currentState != TAKEDDOWN)
							{
								App->particles->AddParticle(App->particles->star, position.x - 25, position.y+10, -3, this, COLLIDER_ENEMY_SHOT);
								attacking = true;
								attack2Left.Reset();
								colliderWeapon = App->collision->AddCollider({ position.x - 30, position.y + 20, 30, 15 }, COLLIDER_ENEMY_WEAPON, (Module*)App->enemy);
								current_animation = &attack2Left;
							}
						}
					}	
				}
			}
			
		}
		else
		{

			if(!jumped && !attacking)
				currentState = ENEMYIDLE;
		}
			
		if (jumped)
		{
			if (faceRight)
			{
				if (abs(App->player->position.x - position.x) <= 25)
				{
					jumped = false;
					colliderWeapon = App->collision->DeleteCollider(colliderWeapon);
					attacking = false;
					currentState = ENEMYIDLE;

				}
				if (colliderWeapon != nullptr)
				{
					colliderWeapon->SetPos(position.x + 30, position.y + 20);
				}
				walk.x += 3;
			}
			else
			{
				if (abs(App->player->position.x - position.x) <= 25)
				{
					jumped = false;
					colliderWeapon = App->collision->DeleteCollider(colliderWeapon);
					attacking = false;
					currentState = ENEMYIDLE;
				}
				if (colliderWeapon != nullptr)
				{
					colliderWeapon->SetPos(position.x - 15, position.y + 20);
				}
				walk.x -= 3;
			}	
		}
		if ((current_animation == &attack1 || current_animation == &attack1Left || current_animation == &attack2 || current_animation == &attack2Left) && current_animation->Finished() )
		{
			jumped = false;
			current_animation = &right;
			attacking = false;
			currentState = ENEMYIDLE;
			if (colliderWeapon != nullptr)
				colliderWeapon = App->collision->DeleteCollider(colliderWeapon);
		}
		break;

		case ENEMYBEINGATTACKED:
			if (attacking)
			{
				attacking = false;
				if (colliderWeapon != nullptr)
					colliderWeapon = App->collision->DeleteCollider(colliderWeapon);
			}
			if (App->player->faceRight && faceRight)
					App->player->sameDirection = true;
			else
				if (!App->player->faceRight && !faceRight)
					App->player->sameDirection = true;
				else
					App->player->sameDirection = false;

			if (hits >= 3)
			{
				currentState = ENEMYTAKEDOWN;
				break;
			}
			if (faceRight)
			{
				walk.x -= 1;
				if (current_animation != &reciveDamage)
				{
					App->audio->PlayFx(App->audio->LoadFx("rtype/hit.wav"));
					hits += 1;
					reciveDamage.Reset();
					current_animation = &reciveDamage;
				}
			}
			else
			{
				walk.x += 1;

				if (current_animation != &reciveDamageLeft)
				{
					App->audio->PlayFx(App->audio->LoadFx("rtype/hit.wav"));
					hits += 1;
					reciveDamageLeft.Reset();
					current_animation = &reciveDamageLeft;
				}
			}
			if ((current_animation == &reciveDamage || current_animation == &reciveDamageLeft) && current_animation->Finished())
			{
				currentState = ENEMYIDLE;
			}
			break;

		case ENEMYTAKEDOWN:
			if (faceRight)
			{
				if (App->player->sameDirection)
				{
					walk.x += 3;
					if (current_animation != &reciveDamage3Left)
					{
						reciveDamage3Left.Reset();
						current_animation = &reciveDamage3Left;
					}
				}
				else
				{
					walk.x -= 3;
					if (current_animation != &reciveDamage2Left)
					{
						reciveDamage2Left.Reset();
						current_animation = &reciveDamage2Left;
					}
					
				}
			}
			else
			{
				if (App->player->sameDirection)
				{
					walk.x -= 3;
					if (current_animation != &reciveDamage3)
					{
						reciveDamage3.Reset();
						current_animation = &reciveDamage3;
					}
				}
				else
				{
					walk.x += 3;
					if (current_animation != &reciveDamage2)
					{
						reciveDamage2.Reset();
						current_animation = &reciveDamage2;
					}
				}
			}
			if ((current_animation == &reciveDamage2 || current_animation == &reciveDamage2Left || current_animation == &reciveDamage3Left || current_animation == &reciveDamage3) && current_animation->Finished())
			{
				toDelete = true;
			}
			break;
	}

	position += walk;
	return true;
}
