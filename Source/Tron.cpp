#include "Prefabs/Prefabs.hpp"
#include "Systems/AIMoveControllerSystem.hpp"
#include "Systems/AIShootControllerSystem.hpp"
#include "Systems/NavigatorSystem.hpp"
#include "Systems/PlayerCanonControllerSystem.hpp"
#include "Systems/PlayerMoveControllerSystem.hpp"
#include "Systems/PlayerShootControllerSystem.hpp"
#include "Tron.hpp"

#if defined FRO_DEBUG
#include <vld.h>
#endif

namespace fro
{
	std::unique_ptr<fro::Application> createApplication()
	{
		InputManager::bindActionToInput("moveRight1", Key::D);
		InputManager::bindActionToInput("moveLeft1", Key::A);
		InputManager::bindActionToInput("moveUp1", Key::W);
		InputManager::bindActionToInput("moveDown1", Key::S);
		InputManager::bindActionToInput("lookRight1", Key::RIGHT);
		InputManager::bindActionToInput("lookLeft1", Key::LEFT);
		InputManager::bindActionToInput("lookUp1", Key::UP);
		InputManager::bindActionToInput("lookDown1", Key::DOWN);
		InputManager::bindActionToInput("shoot1", Key::SPACE);

		PhysicsSystem::setGravity({});

		ResourceManager::store<tron::NavigationMesh>("world1Mesh", SVGParser::parse("Data/SVGs/world1.svg")).translate({ 0.0, 64.0 });
		ResourceManager::store<tron::NavigationMesh>("world2Mesh", SVGParser::parse("Data/SVGs/world2.svg")).translate({ 0.0, 64.0 });
		ResourceManager::store<tron::NavigationMesh>("world3Mesh", SVGParser::parse("Data/SVGs/world3.svg")).translate({ 0.0, 64.0 });

		return std::make_unique<tron::Tron>();
	}
}

namespace tron
{
	struct Test final
	{
		Test() = default;
		Test(Test const&) = delete;
		Test(Test&&) noexcept = delete;

		~Test() = default;

		Test& operator=(Test const&) = delete;
		Test& operator=(Test&&) noexcept = delete;
	};

	Tron::Tron()
	{
		using namespace fro;

		ResourceManager::store<Texture>("menu", mRenderer, Surface{ "Data/Sprites/menu.png" });
		ResourceManager::store<Texture>("redTank", mRenderer, Surface{ "Data/Sprites/redTank.png" });
		ResourceManager::store<Texture>("greenTank", mRenderer, Surface{ "Data/Sprites/greenTank.png" });
		ResourceManager::store<Texture>("redCanon", mRenderer, Surface{ "Data/Sprites/redCanon.png" });
		ResourceManager::store<Texture>("greenCanon", mRenderer, Surface{ "Data/Sprites/greenCanon.png" });
		ResourceManager::store<Texture>("blueTank", mRenderer, Surface{ "Data/Sprites/blueTank.png" });
		ResourceManager::store<Texture>("recognizer", mRenderer, Surface{ "Data/Sprites/recognizer.png" });
		ResourceManager::store<Texture>("playerBullet", mRenderer, Surface{ "Data/Sprites/playerBullet.png" });
		ResourceManager::store<Texture>("enemyBullet", mRenderer, Surface{ "Data/Sprites/enemyBullet.png" });
		ResourceManager::store<Texture>("world1", mRenderer, Surface{ "Data/Sprites/world1.png" });
		ResourceManager::store<Texture>("world2", mRenderer, Surface{ "Data/Sprites/world2.png" });
		ResourceManager::store<Texture>("world3", mRenderer, Surface{ "Data/Sprites/world3.png" });

		menu();

		Logger::info("created Tron!");
	}

	Tron::~Tron()
	{
		fro::Logger::info("destroyed Tron!");
	}

	void Tron::run()
	{
		auto constexpr fixedDeltaSeconds{ 1.0 / 60 };
		double fixedUpdateAccumulator{};

		auto t1{ std::chrono::steady_clock::now() };
		while (mIsRunning)
		{
			auto const t2{ std::chrono::steady_clock::now() };
			double const deltaSeconds{ std::chrono::duration<double>(t2 - t1).count() };
			t1 = t2;
			
			fro::InputManager::processInputContinous();
			fro::SystemEventManager::pollEvents();

			static double elapsedSeconds{};
			switch (mState)
			{
			case tron::Tron::State::PLAYING:
			{
				if (not mPlayer1.valid() and not mPlayer1Flagged)
				{
					fro::Logger::info(--mPlayer1HP);
					mPlayer1Flagged = true;
				}

				if (not mPlayer2.valid() and not mPlayer2Flagged)
				{
					fro::Logger::info(--mPlayer2HP);
					mPlayer2Flagged = true;
				}

				auto const newEndTanks
				{
					std::remove_if(mBlueTanks.begin(), mBlueTanks.end(),
						[](fro::Reference<fro::Entity> const& blueTank)
						{
							return not blueTank.valid();
						})
				};

				mScore += 100 * std::distance(newEndTanks, mBlueTanks.end());
				mBlueTanks.erase(newEndTanks, mBlueTanks.end());

				auto const newEndRecognizers
				{
					std::remove_if(mRecognizers.begin(), mRecognizers.end(),
						[](fro::Reference<fro::Entity> const& recognizer)
						{
							return not recognizer.valid();
						})
				};

				mScore += 250 * std::distance(newEndRecognizers, mRecognizers.end());
				mRecognizers.erase(newEndRecognizers, mRecognizers.end());

				if (not mPlayer1.valid() and not mPlayer2.valid())
				{
					fro::Audio::playSoundEffect("Data/Sound/loss.wav");
					mState = State::LOST;
					mPlayer1Flagged = false;
					mPlayer2Flagged = false;
					fro::Audio::stopMusic();
				}
				else if (mBlueTanks.empty() and mRecognizers.empty())
				{
					fro::Audio::playSoundEffect("Data/Sound/win.wav");
					mState = State::WON;
					mPlayer1Flagged = false;
					mPlayer2Flagged = false;
					fro::Audio::stopMusic();
				}
				break;
			}

			case tron::Tron::State::LOST:
				elapsedSeconds += deltaSeconds;
				if (elapsedSeconds >= 4.0)
				{
					elapsedSeconds = 0;
					if ((mMode == Mode::COOP and (mPlayer1HP or mPlayer2HP)) or
						mPlayer1HP)
					{
						mState = State::PLAYING;
						nextLevel();
						fro::Audio::playMusic("Data/Sound/music.wav");
					}
					else
					{
						mState = State::MENU;
						mPlayer1HP = 4;
						mPlayer2HP = 4;
						mPlayer1Flagged = false;
						mPlayer2Flagged = false;
						menu();
					}
				}
				else
					continue;
				break;

			case tron::Tron::State::WON:
				elapsedSeconds += deltaSeconds;
				if (elapsedSeconds >= 4.0)
				{
					elapsedSeconds = 0;
					mState = State::PLAYING;
					fro::Audio::playMusic("Data/Sound/music.wav");
					nextLevel();
				}
				else
					continue;
				break;
			}

			fixedUpdateAccumulator += deltaSeconds;
			while (fixedUpdateAccumulator >= fixedDeltaSeconds)
			{
				fixedUpdateAccumulator -= fixedDeltaSeconds;
				fro::PhysicsSystem::onFixedUpdate(fixedDeltaSeconds);
				NavigatorSystem::onFixedUpdate(deltaSeconds);
			}

			PlayerMoveControllerSystem::onUpdate(deltaSeconds);
			PlayerCanonControllerSystem::onUpdate(deltaSeconds);
			PlayerShootControllerSystem::onUpdate(deltaSeconds);

			AIMoveControllerSystem::onUpdate(deltaSeconds);
			AIShootControllerSystem::onUpdate(deltaSeconds);

			mRenderer.clear();
			fro::SpriteSystem::onRender(mRenderer);
			mRenderer.present();

			fro::SceneManager::doomAndAdd();
		}
	}

	void Tron::menu()
	{
		fro::Reference<fro::Scene const> const getActiveScene{ fro::SceneManager::getActiveScene() };
		if (getActiveScene.valid())
			fro::SceneManager::removeScene(*getActiveScene);

		fro::Scene& scene{ fro::SceneManager::addScene() };
		fro::Entity& entity{ scene.addEntity() };
		entity.attachComponent<fro::Transform>()->setLocalTranslation({ 240, 256 });
		entity.attachComponent<fro::Sprite>()->texture = fro::ResourceManager::find<fro::Texture>("menu");
		
		fro::SceneManager::setActiveScene(scene);
	}

	void Tron::nextLevel()
	{
		fro::Reference<fro::Scene const> const getActiveScene{ fro::SceneManager::getActiveScene() };
		if (getActiveScene.valid())
			fro::SceneManager::removeScene(*getActiveScene);

		mCurrentLevel = (mCurrentLevel + 1) % 3;

		fro::SceneManager::setActiveScene(fro::SceneManager::addScene(
			prefabs::level(mCurrentLevel + 1, mMode, mPlayer1, mPlayer2, mBlueTanks, mRecognizers)));
	}
}