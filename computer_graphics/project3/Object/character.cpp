#include "character.h"

Character::Character()
{
	VerticalSpeed = 0.0f;
}

Character::~Character()
{
}

void Character::SetCharacterPerspective(VIEW perspective)
{
	CharacterPerspective = perspective;
}

void Character::Jump()
{
	if(VerticalSpeed == 0.0f)
		VerticalSpeed = 2.0f;
}

void Character::Update(float DeltaTime)
{
}

void Character::Draw()
{
	glPushMatrix();

	glTranslatef(Position.x, Position.y, Position.z);

	glPopMatrix();
}

Mesh& Character::GetFirstPersonMesh()
{
	return FirstPersonMesh;
}

Mesh& Character::GetThirdPersonMesh()
{
	return ThirdPersonMesh;
}
