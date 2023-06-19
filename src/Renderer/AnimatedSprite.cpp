#include "AnimatedSprite.h"
#include<iostream>
#include"Texture2D.h"

namespace Renderer
{
	AnimatedSprite::AnimatedSprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string initialSubTexture,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position,
		const glm::vec2& size,
		const float rotation)
		:Sprite(std::move(pTexture),std::move(initialSubTexture), std::move(pShaderProgram), position, size,rotation)
	{
		m_pCurrentAnimetionDurations = m_stateMap.end();
	}


	void AnimatedSprite::insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration)
	{
		m_stateMap.emplace(std::move(state), std::move(subTexturesDuration));
	}
	
	void AnimatedSprite::renderer()
	{


		if (m_dirty)
		{
			auto subTexture = m_pTexture2D->getSubTexture(m_pCurrentAnimetionDurations->second[m_currentFrame].first);


			const GLfloat texCoord[] = {
				subTexture.leftBottomUV.x,subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x,subTexture.rightTopUV.y,
				subTexture.rightTopUV.x,subTexture.rightTopUV.y,

				subTexture.rightTopUV.x,subTexture.rightTopUV.y,
				subTexture.rightTopUV.x,subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x,subTexture.leftBottomUV.y,
			};

			glBindBuffer(GL_ARRAY_BUFFER, m_texCoordVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texCoord), &texCoord);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_dirty = false;
		}
			Sprite::renderer();
	}




	void AnimatedSprite::setState(const std::string newState)
	{
		auto it = m_stateMap.find(newState);
		if (it == m_stateMap.end())
		{
			std::cout << "Cant find animation state" << std::endl;
			return;
		}
		if (it != m_pCurrentAnimetionDurations)
		{
			m_currentAnimationTime = 0;
			m_currentFrame = 0;
			m_pCurrentAnimetionDurations = it;
			m_dirty = true;
		}
	}



	void AnimatedSprite::update(const uint64_t delta)
	{

		if (m_pCurrentAnimetionDurations != m_stateMap.end())
		{
			m_currentAnimationTime += delta;

			while (m_currentAnimationTime >= m_pCurrentAnimetionDurations->second[m_currentFrame].second)
			{
				m_currentAnimationTime -= m_pCurrentAnimetionDurations->second[m_currentFrame].second;
				++m_currentFrame;
				m_dirty = true;
				if (m_currentFrame == m_pCurrentAnimetionDurations->second.size())
				{
					m_currentFrame = 0;
				}
			}
		}
	}
}