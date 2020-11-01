#pragma once

#include <vector>

#include "SuSpriteRenderer.h"
#include "ShaderUtil.h"
#include "Transform.h"
#include "SuTexture2D.h"

using namespace Sungine;

namespace Sungine
{
	class Entity
	{
	private:
		int mEntityID;

		static std::vector<Entity*> msAllEntities;

		static int msGameObjectCount;

		bool mDestroyed = false;

	protected:
		std::string mName;
		Transform mTransform;
		SuTexture2D mTexture;

	public:
		Entity();
		Entity(SuTexture2D sprite, glm::vec2 pos, glm::vec2 size = glm::vec2(1.f));

		~Entity();

		virtual void Draw(SuSpriteRenderer& renderer);

		static int GetGameObjectCount() { return msGameObjectCount; }
		static std::vector <Entity*> GetAllEntities() { return msAllEntities; }

		void SetName(std::string name) { mName = name; }
		void SetDestroyed(bool destroyed) { mDestroyed = destroyed; }
		void SetTexture(SuTexture2D texture) { mTexture = texture; }
		void SetPosition(glm::vec2 position) { mTransform.position = position; }
		void SetSize(glm::vec2 size) { mTransform.size = size; }
		void SetRotation(float rotation) { mTransform.rotation = rotation; }
		void SetScale(float scale) { mTransform.scale = scale; }

		std::string GetName() { return mName; }
		bool GetDestroyed() { return mDestroyed; }
		SuTexture2D GetTexture() { return mTexture; }
		glm::vec2 GetPosition() { return mTransform.position; }
		glm::vec2 GetSize() { return mTransform.size; }
		float GetRotation() { return mTransform.rotation; }
		float GetScale() { return mTransform.scale; }
	};
}

