#include "LevelComplete.hpp"

LevelComplete::LevelComplete()
{


	// Load BackGround
	if (!textureBackground.loadFromFile(IMG_LEVELCOMPLETE))
	{
		std::cout << "Load file failed" << std::endl;
	}

	spriteBackground.setTexture(textureBackground);

	sf::Vector2u textureBackgroundSize = textureBackground.getSize();
	textureBackgroundSize.x /= 2;
	textureBackgroundSize.y /= 2;
	spriteBackground.setOrigin(textureBackgroundSize.x, textureBackgroundSize.y);
	spriteBackground.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	spriteBackground.scale(1, 1);


	// Load total time text
	smallText.resize(2);
	
	smallText[0].setCharacterSpacing(1.5);
	smallText[0].alignTextCenter();
	smallText[0].setTextPosition(550, 500);
	

	smallText[1].setCharacterSpacing(1.5);
	smallText[1].alignTextCenter();
	smallText[1].setTextPosition(400, 550);

}

LevelComplete::~LevelComplete()
{
}


//@DESCR: Render the level complete screen
//@PARAM: m_pWindow to render GUi on, time: total time stat of the player
//@RETURN: None
void LevelComplete::draw(sf::RenderWindow& window, int time)
{
	//Draw background
	window.draw(spriteBackground);


}
