#include "character.h"

Character::Character()
{
	VerticalSpeed = 0.0f;
}

Character::~Character()
{
}

void Character::SetCharacterPerspective(Perspective perspective)
{
	CharacterPerspective = perspective;
}

void Character::Update(float DeltaTime)
{
}

void Character::Draw()
{
	glPushMatrix();

	glTranslatef(Position.x, Position.y, Position.z);

	if(CharacterPerspective == FIRST_PERSON)
		FirstPersonMesh.Draw();
	else
		ThirdPersonMesh.Draw();

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
