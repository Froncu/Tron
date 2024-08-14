#ifndef FRONGE_EDITOR_HPP
#define FRONGE_EDITOR_HPP

#include "Fronge.hpp"

namespace tron
{
	class Tron final : public fro::Application
	{
	public:
		enum class Mode
		{
			SINGLE,
			COOP,
			VERSUS
		};

		enum class State
		{
			MENU,
			PLAYING,
			LOST,
			WON,
			SCORE
		};

		Tron();
		Tron(Tron const&) = default;
		Tron(Tron&&) noexcept = default;

		virtual ~Tron() override;

		Tron& operator=(Tron const&) = default;
		Tron& operator=(Tron&&) noexcept = default;

		virtual void run() override;

	private:
		void menu();
		void nextLevel();

		fro::Window mMainWindow{ "Tron", { 1280, 720 } };
		fro::Renderer mRenderer{ mMainWindow, { 480, 512 } };

		std::unique_ptr<fro::Gamepad> mGamepad1{};
		std::unique_ptr<fro::Gamepad> mGamepad2{};

		bool mIsRunning{ true };

		fro::EventListener<> mOnMainWindowCloseEvent
		{
			[this]()
			{
				mIsRunning = false;
				return true;
			}, mMainWindow.mCloseEvent
		};

		fro::EventListener<fro::InputEvent const> mOnInputEvent
		{
			fro::VariantVisitor
			{
				[this](fro::GamepadConnectedEvent const& event)
				{
					auto& gamepadToSet{ mGamepad1.get() ? mGamepad2 : mGamepad1 };
					if (gamepadToSet.get())
						return false;

					gamepadToSet = std::make_unique<fro::Gamepad>(event.deviceID);

					auto const ID{ gamepadToSet->getID() };

					std::string const number{ std::to_string(&gamepadToSet == &mGamepad1 ? 2 : 1) };
					fro::InputManager::bindActionToInput("moveRight" + number, fro::GamepadAxisInput{ ID, fro::GamepadAxis::LEFT_STICK_RIGHT });
					fro::InputManager::bindActionToInput("moveLeft" + number, fro::GamepadAxisInput{ ID, fro::GamepadAxis::LEFT_STICK_LEFT });
					fro::InputManager::bindActionToInput("moveUp" + number, fro::GamepadAxisInput{ ID, fro::GamepadAxis::LEFT_STICK_UP });
					fro::InputManager::bindActionToInput("moveDown" + number, fro::GamepadAxisInput{ ID, fro::GamepadAxis::LEFT_STICK_DOWN });
					fro::InputManager::bindActionToInput("lookRight" + number, fro::GamepadAxisInput{ ID, fro::GamepadAxis::RIGHT_STICK_RIGHT });
					fro::InputManager::bindActionToInput("lookLeft" + number, fro::GamepadAxisInput{ ID, fro::GamepadAxis::RIGHT_STICK_LEFT });
					fro::InputManager::bindActionToInput("lookUp" + number, fro::GamepadAxisInput{ ID, fro::GamepadAxis::RIGHT_STICK_UP });
					fro::InputManager::bindActionToInput("lookDown" + number, fro::GamepadAxisInput{ ID, fro::GamepadAxis::RIGHT_STICK_DOWN });
					fro::InputManager::bindActionToInput("shoot" + number, fro::GamepadAxisInput{ ID, fro::GamepadAxis::RIGHT_TRIGGER });

					return true;
				},

				[this](fro::GamepadDisconnectedEvent const& event)
				{
					if (mGamepad1.get() and event.ID == mGamepad1->getID())
						mGamepad1.reset();

					else if (mGamepad2.get() and event.ID == mGamepad2->getID())
						mGamepad2.reset();

					else
						return false;

					return true;
				},

				[this](fro::KeyDownEvent const& event)
				{
					if (event.key == fro::Key::M)
						fro::Audio::setMute(!fro::Audio::isMuted());

					else if (mState == State::MENU)
					{
						switch (event.key)
						{
						case fro::Key::S:
							mMode = Mode::SINGLE;
							mState = State::PLAYING;
							nextLevel();
							fro::Audio::playMusic("Data/Sound/music.wav");
							break;

						case fro::Key::C:
							mMode = Mode::COOP;
							mState = State::PLAYING;
							nextLevel();
							fro::Audio::playMusic("Data/Sound/music.wav");
							break;

						case fro::Key::V:
							mMode = Mode::VERSUS;
							mState = State::PLAYING;
							nextLevel();
							fro::Audio::playMusic("Data/Sound/music.wav");
							break;
						}
					}
					else if (event.key == fro::Key::F1)
					{
						nextLevel();
						return true;
					}
					
					return true;
				},

				[](auto&&)
				{
					return false;
				}
			}, fro::SystemEventManager::mInputEvent
		};

		std::size_t mCurrentLevel{ 2 };

		Mode mMode{};
		State mState{ State::MENU };

		std::vector<fro::Reference<fro::Entity>> mBlueTanks{};
		std::vector<fro::Reference<fro::Entity>> mRecognizers{};

		std::size_t mScore{};

		fro::Reference<fro::Entity> mPlayer1{};
		fro::Reference<fro::Entity> mPlayer2{};

		int mPlayer1HP{ 4 };
		int mPlayer2HP{ 4 };

		bool mPlayer1Flagged{};
		bool mPlayer2Flagged{};
	};
}

#endif