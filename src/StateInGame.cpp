#include "StateInGame.hpp"
#include "stdio.h"

MyLevel level[7] = { MyLevel{3, 3} , MyLevel{ 5, 5 }, MyLevel{ 9, 9 }, MyLevel{ 15, 15 }, MyLevel{ 20, 20 }, MyLevel{ 23, 23 }, MyLevel{ 30, 30 } };
std::shared_ptr<LevelMaze> StateInGame::levelMaze = NULL;
int StateInGame::timeCycle = 0;

//@DESCR: Initialize variables of Game
//@PARAM: None
//@RETURN: None
void StateInGame::initVariables()
{
	m_State =InGameState; //Obviously, we always in MenuState when entering game
	this->m_EndGame = false;
	this->m_Level = 0;


}

void StateInGame::initWindow()
{
	this->m_pWindow = SingletonRenderWindow::getInstance();
	this->m_pNextStage = std::shared_ptr<NextStage>(new NextStage());

	if (!textureBackground.loadFromFile(IMG_BEHINDMAZE))
	{
		std::cout << "Load file failed" << std::endl;
	}
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);
	
}


//@DESCR: Initialze settings of fonts
//@PARAM: None
//@RETURN: None
void StateInGame::initFonts()
{
	if (!this->m_Font.loadFromFile("Fonts/TitleFont.ttf"))
	{
		std::cout << " ! ERROR::GAME::INITFONTS::COULD NOT LOAD TitleFont.ttf" << "\n";
	}
}


//@DESCR: Initialize settings and parameters of buttons
//@PARAM: None
//@RETURN: None
void StateInGame::initButtons()
{

	float scSize = 1.5;

	m_Help.setButton(sf::Vector2f(OFFSET_HELP_X, OFFSET_HELP_Y), sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT),
		sf::Color::Transparent, sf::Color::Transparent,
		"", FONT_GAME, 20, sf::Text::Style::Bold, sf::Color::White, "Images/DHelp.png");
	m_Help.setScaleSprite(sf::Vector2f(m_Help.getScaleSprite().x * scSize, m_Help.getScaleSprite().y * scSize));

	m_RestartGame.setButton(sf::Vector2f(OFFSET_RESTART_X, OFFSET_RESTART_Y), sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT),
		sf::Color::Transparent, sf::Color::Transparent,
		"", FONT_GAME, 20, sf::Text::Style::Bold, sf::Color::White, "Images/DRestart.png");
	m_RestartGame.setScaleSprite(sf::Vector2f(m_RestartGame.getScaleSprite().x * scSize, m_RestartGame.getScaleSprite().y * scSize));


	m_ReturnMenu.setButton(sf::Vector2f(OFFSET_RETURN_MENU_X, OFFSET_RETURN_MENU_Y), sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT),
		sf::Color::Transparent, sf::Color::Transparent,
		"", FONT_GAME, 20, sf::Text::Style::Bold, sf::Color::White, "Images/DMenu.png");
	m_ReturnMenu.setScaleSprite(sf::Vector2f(m_ReturnMenu.getScaleSprite().x * scSize, m_ReturnMenu.getScaleSprite().y * scSize));
}


//@DESCR: Initialize settings and parameters of text
//@PARAM: None
//@RETURN: None
void StateInGame::initText()
{
	//Gui text init
	this->m_GuiText.setFont(this->m_Font);
	this->m_GuiText.setFillColor(sf::Color::White);
	this->m_GuiText.setCharacterSize(32);

	//End game text
	this->m_EndGameText.setFont(this->m_Font);
	this->m_EndGameText.setFillColor(sf::Color::Red);
	this->m_EndGameText.setCharacterSize(60);
	this->m_EndGameText.setPosition(sf::Vector2f(20, 100));
	this->m_EndGameText.setString("YOU ARE DEAD, EXIT THE GAME!");

	initTitle();
	initLevelInfo();
	initTimeInfo();
	initCoinsInfo();
}


//@DESCR: Changing from "current" level to "current + 1" level
//@PARAM: None
//@RETURN: None
void StateInGame::nextLevel(bool check)
{
	if (m_Level >= MAX_LEVEL - 1)
		return;
	m_Level++;
	setLevel(m_Level, check);
}


//@DESCR: Changing from "current" level to new level
//@PARAM: new level
//@RETURN: None
void StateInGame::setLevel(int _level, bool check)
{

	m_Level = _level;
	curMaze->UpdateMaze(check);

	m_Player->setPosition(curMaze->getStartPos());
	m_Player->setSize(curMaze->getWidthRoom(), curMaze->getHeightRoom());
	m_Player->updateDirecPlayer(1);
	m_Player->setLose(false);
	m_Player->resetTimes(check);

	m_Helper.end();
}


//@DESCR: Constructor of Game
//@PARAM: None
//@RETURN: None
StateInGame::StateInGame(bool isContinue)
{
	initWindow();
	initVariables();
	initFonts();
	initButtons();
	initText();

	if (isContinue)
	{
		std::cout << "LOAD\n";
		load();
	}
	else
	{
		if (StateModeSelect::pressedItem == 0)
			levelMaze = std::shared_ptr<LevelMaze>(new EasyLevelMaze());

		if (StateModeSelect::pressedItem == 1)
			levelMaze = std::shared_ptr<LevelMaze>(new MediumLevelMaze());

		if (StateModeSelect::pressedItem == 2)
			levelMaze = std::shared_ptr<LevelMaze>(new HardLevelMaze());


		curMaze = levelMaze->OrderLevelMaze();
		m_Player = std::shared_ptr<Player>(new Player(curMaze->getStartPos(), OFFSET_MAZE_X, OFFSET_MAZE_Y, curMaze->getWidthRoom(), curMaze->getHeightRoom()));
	}
}


//@DESCR: Check whether the Game is ended
//@PARAM: None
//@RETURN: the state of the game: FALSE - running | TRUE - ended
bool StateInGame::getEndGame()
{
	return this->m_EndGame;
}



//@DESCR: Update m_Player's changes, including position
//@PARAM: None
//@RETURN: None
void StateInGame::updatePlayer()
{
	this->m_Player->update(m_pWindow, m_Event.key.code, curMaze);
}


//@DESCR: Update GUI's changes
//@PARAM: None
//@RETURN: None
void StateInGame::updateGui()
{
	stringstream writer;
}


//@DESCR: Render GUi
//@PARAM: m_pWindow to render GUi on
//@RETURN: None
void StateInGame::renderGui(sf::RenderTarget* target)
{
	m_pWindow->draw(spriteBackground);
}


//@DESCR: Render Game's particular state such as: GameOver, NextStage, LevelComplete
//@PARAM: None
//@RETURN: Non
void StateInGame::renderDisplayStates(GameState state)
{

	if (state == NextStageState)
	{

		m_pNextStage->draw(*m_pWindow);
		return;
	}
}


void StateInGame::initTitle() {
	m_Title.setBox(sf::Vector2f(OFFSET_TITLE_X, OFFSET_TITLE_Y),
		sf::Vector2f(SCREEN_TITLE_WIDTH, SCREEN_TITLE_HEIGHT),
		sf::Color::Black, sf::Color::Transparent);

	m_Title.setText("MAZE RUNNER", FONT_GAME, 90, sf::Text::Italic, sf::Color::Color(253, 205, 79));
	m_Title.setCharacterSpacing(1.5);
	m_Title.alignTextCenter();
	m_Title.alignTextMiddle();
}

void StateInGame::initLevelInfo() {
	stringstream writer("");

	writer << "LEVEL " << m_Level + 1;

	m_LevelInfo.setBox(sf::Vector2f(OFFSET_LEVEL_X, OFFSET_LEVEL_Y),
		sf::Vector2f(SCREEN_INFO_WIDTH, SCREEN_INFO_HEIGHT),
		sf::Color::Black, sf::Color::White);
	m_LevelInfo.setBoxOutlineThickness(5);

	m_LevelInfo.setText(writer.str(), FONT_GAME, 40, sf::Text::Italic, sf::Color::Color(253, 205, 79));
	m_LevelInfo.setCharacterSpacing(1.5);
	m_LevelInfo.alignTextCenter();
	m_LevelInfo.alignTextMiddle();
}

void StateInGame::initTimeInfo() {
	// Time title
	m_TimeInfo[0].setBox(sf::Vector2f(OFFSET_TIME_X, OFFSET_TIME_Y),
		sf::Vector2f(SCREEN_INFO_WIDTH / 2, SCREEN_INFO_HEIGHT / 2),
		sf::Color::Black, sf::Color::White);
	m_TimeInfo[0].setBoxOutlineThickness(5);

	m_TimeInfo[0].setText("LIVES", FONT_GAME, 30, sf::Text::Italic, sf::Color::Color(253, 205, 79));
	m_TimeInfo[0].setCharacterSpacing(1.5);
	m_TimeInfo[0].alignTextCenter();
	m_TimeInfo[0].alignTextMiddle();

	// Time info
	stringstream writer("");

	m_TimeInfo[1].setBox(sf::Vector2f(OFFSET_TIME_INFO_X, OFFSET_TIME_INFO_Y),
		sf::Vector2f(SCREEN_INFO_WIDTH / 2, SCREEN_INFO_HEIGHT / 2),
		sf::Color::Black, sf::Color::White);
	m_TimeInfo[1].setBoxOutlineThickness(5);

	//writer << m_Time.getCDTime();

	m_TimeInfo[1].setText(writer.str(), FONT_GAME, 30, sf::Text::Bold, sf::Color::Color(253, 205, 79));
	m_TimeInfo[1].setCharacterSpacing(1.5);
	m_TimeInfo[1].alignTextCenter();
	m_TimeInfo[1].alignTextMiddle();
}

void StateInGame::initCoinsInfo() {
	// Coins title
	m_CoinsInfo[0].setBox(sf::Vector2f(OFFSET_COINS_X, OFFSET_COINS_Y),
		sf::Vector2f(SCREEN_INFO_WIDTH / 2, SCREEN_INFO_HEIGHT / 2),
		sf::Color::Black, sf::Color::White);
	m_CoinsInfo[0].setBoxOutlineThickness(5);

	m_CoinsInfo[0].setText("COINS", FONT_GAME, 30, sf::Text::Italic, sf::Color::Color(253, 205, 79));
	m_CoinsInfo[0].setCharacterSpacing(1.5);
	m_CoinsInfo[0].alignTextCenter();
	m_CoinsInfo[0].alignTextMiddle();

	// Coins info
	stringstream writer("");
	m_CoinsInfo[1].setBox(sf::Vector2f(OFFSET_COINS_INFO_X, OFFSET_COINS_INFO_Y),
		sf::Vector2f(SCREEN_INFO_WIDTH / 2, SCREEN_INFO_HEIGHT / 2),
		sf::Color::Black, sf::Color::White);
	m_CoinsInfo[1].setBoxOutlineThickness(5);

	writer << m_Points;

	m_CoinsInfo[1].setText(writer.str(), FONT_GAME, 30, sf::Text::Bold, sf::Color::Color(253, 205, 79));
	m_CoinsInfo[1].setCharacterSpacing(1.5);
	m_CoinsInfo[1].alignTextCenter();
	m_CoinsInfo[1].alignTextMiddle();
}

void StateInGame::updateTimeInfo() {
	m_TimeInfo[1].setContent(std::to_string(m_Player->getTimesPlayed()));
	m_TimeInfo[1].alignTextCenter();
	m_TimeInfo[1].alignTextMiddle();
}

void StateInGame::updateCoinsInfo() {
	stringstream writer("");
	writer << m_Player->getTotalCoin();

	m_CoinsInfo[1].setContent(writer.str());
	m_CoinsInfo[1].alignTextCenter();
	m_CoinsInfo[1].alignTextMiddle();
}

void StateInGame::updateLevel() {
	stringstream writer("");

	writer << "LEVEL " << curMaze->getLevel() + 1;

	m_LevelInfo.setContent(writer.str());
	m_LevelInfo.alignTextCenter();
	m_LevelInfo.alignTextMiddle();
}

void StateInGame::resetGame()
{
	this->m_EndGame = false;
	setLevel(0, true);
}


void StateInGame::pollEvents()
{
	
		sf::Event temp;
		while (this->m_pWindow->pollEvent(temp))
		{
			if (temp.type != sf::Event::MouseButtonPressed) {
				m_Help.makeNormal();
				m_ReturnMenu.makeNormal();
				m_RestartGame.makeNormal();
			}

			switch (temp.type)
			{
			case sf::Event::Closed:
				this->m_pWindow->close();
				break;

				//------------------CLICK EVENT------------------
			case sf::Event::MouseButtonPressed:
				if (m_State == GameState::InGameState)
				{
					cout << "YES" << endl;
					if (m_ReturnMenu.isClickV1(m_pWindow))
					{
						m_ReturnMenu.makeChosen();
					}
					else {
						m_ReturnMenu.makeNormal();
					}

					if (m_Helper.isShowing == false || m_Helper.isFinished == true) {
						if (m_Help.isClickV1(m_pWindow))
						{
							m_Helper.end();
							m_Help.makeChosen();
						}
						else {
							m_Help.makeNormal();
						}

						if (m_RestartGame.isClickV1(m_pWindow))
						{
							m_Helper.end();
							m_RestartGame.makeChosen();
						}
						else {
							m_RestartGame.makeNormal();
						}
					}
					break;
				}
				break;

				//------------------BUTTON EVENT------------------
			case sf::Event::KeyPressed:

				//In Game
				if (m_State == InGameState)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
					{
						//m_State = GameState::LevelCompleteState;
					}
					break;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					resetGame();
					//m_State = GameState::MenuState;
					break;
				}
				break;

			}
		}//
		this->m_Event = temp;

		if (m_ReturnMenu.isChosen())
		{
			save();
			context_->TransitionTo(new StateMenu);
			return;
		}

		if (m_Help.isChosen())
		{
			m_Help.makeNormal();
			if (m_Player->getTotalCoin() >= 3)
			{
				
				m_Player->setTotalCoin(m_Player->getTotalCoin() - 3);

				shared_ptr<Room> destination;;
				if (curMaze->CheckGetAllKey()) {
					
					destination = curMaze->FindRoomByPos(curMaze->getFinalPos());
				}
				else {
					BFS nearestKeyFinder;
					destination = nearestKeyFinder.findNearestItem(curMaze->FindRoomByPos(m_Player->getPosition()), RoomType::KEY);
				}
				m_Helper.start(AlgorithmIndex::iAStar,
					curMaze->FindRoomByPos(m_Player->getPosition()),
					destination);
				return;
			}
			return;
		}

		if (m_RestartGame.isChosen())
		{
			m_RestartGame.makeNormal();
			context_->TransitionTo(new StateInGame);
			return;
		}

		return;
	
}

void StateInGame::update()
{
	if (this->m_EndGame == false && m_State == InGameState)
	{
		if (m_Event.type == sf::Event::KeyPressed)
			this->updatePlayer();
		this->updateGui();
	}
}

void StateInGame::render()
{

	m_pWindow->clear(sf::Color(128, 128, 128));

	//Render Gui
	this->renderGui(this->m_pWindow);

	//Render end text
	if (this->m_EndGame == true)
		this->m_pWindow->draw(this->m_EndGameText);

	//Render helping path
	if (m_Helper.isShowing) {
		m_Helper.showPathOnly(*m_pWindow);
	}

	//Render Maze
	curMaze->AddMazeRoomsToRenderer(*m_pWindow);
	curMaze->AddMazeObstaclesToRenderer(*m_pWindow);
	if (timeCycle % 50 == 0) {
		curMaze->NextMazeCycle();
	}
	timeCycle++;
	if (timeCycle == 100) {
		//Save after 100 timcyles
		save();
		timeCycle = 0;
	}
	m_Player->render(*m_pWindow);
	m_Player->checkCllisionObject(curMaze);

	// Info display
	updateTimeInfo();
	updateCoinsInfo();

	m_Title.drawMyText(*m_pWindow);

	for (int i(0); i < 2; i++) {
		m_CoinsInfo[i].drawMyText(*m_pWindow);
		m_TimeInfo[i].drawMyText(*m_pWindow);
	}


	//-----------------------------Next Stage-----------------------------
	if (m_Player->loseLevel()) { //GameOver
		remove("save.dat");
		remove("temp.dat");
		this->context_->TransitionTo(new StateOverGame);
		return;
	}
	if (m_State == NextStageState)
	{
		delay(0.7);

		//Win a difficulty mode
		if (curMaze->isWin() == true) {
			remove("save.dat");
			remove("temp.dat");
			this->context_->TransitionTo(new StateDifficultyComplete);
			return;
		}
		
		nextLevel(true);
		m_State = InGameState;
	}


	if (m_Player->getLose() == true) //Restart
	{
		delay(0.7);

		//Win a difficulty mode
		if (m_Level == MAX_LEVEL - 1)
		{
			//m_State = DifficultyCompleteState;
		}
		else
		{
			nextLevel(false);
			m_State = InGameState;
		}
	}


	//-----------------------------Next Stage-----------------------------

	// Update level
	updateLevel();
	m_LevelInfo.drawMyText(*m_pWindow);

	if (m_State == LevelCompleteState || curMaze->completeLevel() == true)
	{
		renderDisplayStates(NextStageState);
		m_State = NextStageState;
	}

	// Button display
	m_Help.drawButton(*m_pWindow);
	m_RestartGame.drawButton(*m_pWindow);
	m_ReturnMenu.drawButton(*m_pWindow);

	this->m_pWindow->display();

}
