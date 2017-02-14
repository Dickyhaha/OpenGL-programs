#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object.h"
#include "mesh.h"

class Character: public Object
{
	private:
		Perspective CharacterPerspective;

		Mesh FirstPersonMesh;
		Mesh ThirdPersonMesh;

		float VerticalSpeed;
		
	public:
		Character();
		~Character();

		void SetCharacterPerspective(Perspective perspective);

		virtual void Update(float DeltaTime);
		void Draw();

		Mesh& GetFirstPersonMesh();
		Mesh& GetThirdPersonMesh();
};

#endif
