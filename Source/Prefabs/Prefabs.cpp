#include "Components/AIMoveController.hpp"
#include "Components/AIShootController.hpp"
#include "Components/EnemyBulletCollider.hpp"
#include "Components/EnemyCollider.hpp"
#include "Components/Navigator.hpp"
#include "Components/PlayerBulletCollider.hpp"	
#include "Components/PlayerCanonController.hpp"
#include "Components/PlayerCollider.hpp"
#include "Components/PlayerMoveController.hpp"
#include "Components/PlayerShootController.hpp"
#include "Prefabs.hpp"

namespace tron
{
	namespace prefabs
	{
		fro::Entity world(std::size_t const which)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation({ 240.0, 288.0 });

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("world" + std::to_string(which));
			sprite.layer = 0;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::STATIC);

			NavigationMesh const& navigationMesh{ *fro::ResourceManager::find<NavigationMesh>("world" + std::to_string(which) + "Mesh") };
			for (fro::Polygon<double> polygon : navigationMesh.getPolygons())
			{
				for (fro::Vector2<double>& vertex : polygon.vertices)
				{
					vertex.x -= 240;
					vertex.y -= 288;
				}

				fro::Collider& collider{ rigidbody.addCollider() };
				collider.setShape(polygon);
				collider.setRestitution(1.0);
				collider.setFriction(0.0);
				collider.setCategoryBits(fro::createBitfield(0));
				collider.setMaskBits(fro::createBitfield(1, 2, 5));
			};

			return entity;
		}

		fro::Entity tank(fro::Vector2<double> const position, bool const red)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation(position);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>(red ? "redTank" : "greenTank");
			sprite.layer = red ? 2 : 4;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::DYNAMIC);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Circle<double>{ { -3.0, 0.0 }, 16.0 });
			collider.setDensity(1.0);
			collider.setSensor(true);
			collider.setCategoryBits(fro::createBitfield(3));
			collider.setMaskBits(fro::createBitfield(1, 2, 4, 5));

			entity.attachComponent<Navigator>();
			entity.attachComponent<PlayerCollider>();

			PlayerMoveController& controller{ *entity.attachComponent<PlayerMoveController>() };
			std::string const number{ std::to_string(red ? 1 : 2) };
			controller.moveRightAction = "moveRight" + number;
			controller.moveLeftAction = "moveLeft" + number;
			controller.moveUpAction = "moveUp" + number;
			controller.moveDownAction = "moveDown" + number;

			return entity;
		}

		fro::Entity canon(fro::Reference<fro::Transform> const tankTransform, bool const red)
		{
			fro::Entity entity{};

			fro::Transform& transform{ *entity.attachComponent<fro::Transform>() };
			transform.setParent(&*tankTransform, false);
			transform.setLocalTranslation({ -3.0, 0.0 });

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>(red ? "redCanon" : "greenCanon");
			sprite.layer = red ? 3 : 5;
 
			PlayerCanonController& controller{ *entity.attachComponent<PlayerCanonController>() };
			std::string const number{ std::to_string(red ? 1 : 2) };
			controller.lookRightAction = "lookRight" + number;
			controller.lookLeftAction = "lookLeft" + number;
			controller.lookUpAction = "lookUp" + number;
			controller.lookDownAction = "lookDown" + number;
			controller.shootAction = "shoot" + number;

			return entity;
		}

		fro::Entity bullet(fro::Reference<fro::Transform const> const canonTransform)
		{
			fro::Entity entity{};

			fro::Vector2<double> const canonTranslation{ canonTransform->world().getTranslation() };
			double const canonRotation{ canonTransform->world().getRotation() };
			fro::Vector2<double> const direction{ std::cos(canonRotation), std::sin(canonRotation) };

			entity.attachComponent<fro::Transform>()->setLocalTranslation(canonTranslation + direction * 20.0);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("playerBullet");
			sprite.layer = 1;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::DYNAMIC);
			rigidbody.setFixedRotation(true);
			rigidbody.setLinearVelocity(direction * 256.0);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Circle<double>{.radius{ 2.0 } });
			collider.setDensity(1.0);
			collider.setRestitution(1.0);
			collider.setCategoryBits(fro::createBitfield(1));
			collider.setMaskBits(fro::createBitfield(0, 3, 4));

			entity.attachComponent<PlayerBulletCollider>();

			return entity;
		}

		fro::Entity blueTank(fro::Vector2<double> const position)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation(position);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("blueTank");
			sprite.layer = 6;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::KINEMATIC);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Rectangle<double>{.width{ 32.0 }, .height{ 32.0 } });
			collider.setDensity(1.0);
			collider.setCategoryBits(fro::createBitfield(4));
			collider.setMaskBits(fro::createBitfield(1, 3));

			entity.attachComponent<Navigator>();
			entity.attachComponent<EnemyCollider>();

			return entity;
		}

		fro::Entity blueTankAI(fro::Vector2<double> const position, fro::Reference<fro::Transform const> const redTankTransform)
		{
			fro::Entity entity{ blueTank(position) };

			AIMoveController& controller{ *entity.attachComponent<AIMoveController>() };
			controller.target = redTankTransform;
			controller.targetPosition = redTankTransform->world().getTranslation();

			entity.attachComponent<AIShootController>();

			return entity;
		}

		fro::Entity blueTankPlayer(fro::Vector2<double> const position)
		{
			fro::Entity entity{ blueTank(position) };

			PlayerMoveController& controller{ *entity.attachComponent<PlayerMoveController>() };
			controller.moveRightAction = "moveRight2";
			controller.moveLeftAction = "moveLeft2";
			controller.moveUpAction = "moveUp2";
			controller.moveDownAction = "moveDown2";

			entity.attachComponent<PlayerShootController>()->shootAction = "shoot2";

			return entity;
		}

		fro::Entity blueTankBullet(fro::Reference<fro::Transform const> const blueTankTransform)
		{
			fro::Entity entity{};

			fro::Vector2<double> const tankTranslation{ blueTankTransform->world().getTranslation() };
			double const tankRotation{ blueTankTransform->world().getRotation() };
			fro::Vector2<double> const direction{ std::cos(tankRotation), std::sin(tankRotation) };

			fro::Transform& transform{ *entity.attachComponent<fro::Transform>() };
			transform.setLocalTranslation(tankTranslation + direction * 20.0);
			transform.setLocalRotation(tankRotation);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("enemyBullet");
			sprite.layer = 1;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::DYNAMIC);
			rigidbody.setFixedRotation(true);
			rigidbody.setLinearVelocity(direction * 192.0);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Circle<double>{.radius{ 3.0 } });
			collider.setDensity(1.0);
			collider.setCategoryBits(fro::createBitfield(5));
			collider.setMaskBits(fro::createBitfield(0, 3));
			collider.setSensor(true);

			entity.attachComponent<EnemyBulletCollider>();

			return entity;
		}

		fro::Entity recognizer(fro::Vector2<double> const position, fro::Reference<fro::Transform const> const redTankTransform)
		{
			fro::Entity entity{};

			entity.attachComponent<fro::Transform>()->setLocalTranslation(position);

			fro::Sprite& sprite{ *entity.attachComponent<fro::Sprite>() };
			sprite.texture = fro::ResourceManager::find<fro::Texture>("recognizer");
			sprite.layer = 6;

			fro::Rigidbody& rigidbody{ *entity.attachComponent<fro::Rigidbody>() };
			rigidbody.setType(fro::Rigidbody::Type::KINEMATIC);

			fro::Collider& collider{ rigidbody.addCollider() };
			collider.setShape(fro::Rectangle<double>{.x{ -3.0 }, .width{ 26.0 }, .height{ 32.0 } });
			collider.setDensity(1.0);
			collider.setCategoryBits(fro::createBitfield(4));
			collider.setMaskBits(fro::createBitfield(1, 3));

			entity.attachComponent<Navigator>()->mVelocity *= 2.0;
			entity.attachComponent<EnemyCollider>();

			AIMoveController& controller{ *entity.attachComponent<AIMoveController>() };
			controller.target = redTankTransform;
			controller.targetPosition = redTankTransform->world().getTranslation();

			return entity;
		}

		fro::Scene level(std::size_t const which, Tron::Mode const mode,
			fro::Reference<fro::Entity>& player1, fro::Reference<fro::Entity>& player2,
			std::vector<fro::Reference<fro::Entity>>& blueTanks, std::vector<fro::Reference<fro::Entity>>& recognizers)
		{
			blueTanks.clear();
			recognizers.clear();

			fro::Scene scene{};

			fro::Reference<fro::Entity const> const worldEntity{ scene.addEntity(world(which)) };

			NavigationMesh const& navigationMesh{ *fro::ResourceManager::find<NavigationMesh>("world" + std::to_string(which) + "Mesh") };
			auto const& nodes{ navigationMesh.getNodes() };
			std::size_t const redTankNodeIndex{ std::rand() % nodes.size() };

			player1 = scene.addEntity(tank(nodes[redTankNodeIndex].first));
			player1->findComponent<Navigator>()->setNavigationMesh(navigationMesh);

			scene.addEntity(canon(player1->findComponent<fro::Transform>()));

			if (mode == Tron::Mode::COOP)
			{
				std::size_t const greenTankNodeIndex{ nodes[redTankNodeIndex].second.front() };
				player2 = scene.addEntity(tank(nodes[greenTankNodeIndex].first, false));
				player2->findComponent<Navigator>()->setNavigationMesh(navigationMesh);

				scene.addEntity(canon(player2->findComponent<fro::Transform>(), false));
			}
			else
				player2.reset();

			fro::Reference<fro::Transform const> player1Transform{ player1->findComponent<fro::Transform>() };

			fro::Entity* enemyEntity{};
			std::unordered_set<std::size_t> takenIndices{};
			std::size_t const amountOfBlueTanks{ 4 - which };
			for (std::size_t i{}; i < amountOfBlueTanks; ++i)
			{
				std::size_t nodeIndex;
				do
				{
					nodeIndex = std::rand() % nodes.size();
				} while (takenIndices.contains(nodeIndex) or
					(nodes[nodeIndex].first - nodes[redTankNodeIndex].first).getMagnitude() < 256.0);

				takenIndices.insert(nodeIndex);

				if (mode == Tron::Mode::VERSUS and i == amountOfBlueTanks - 1)
					enemyEntity = &scene.addEntity(prefabs::blueTankPlayer(nodes[nodeIndex].first));
				else
					enemyEntity = &scene.addEntity(prefabs::blueTankAI(nodes[nodeIndex].first, player1Transform));

				enemyEntity->findComponent<Navigator>()->setNavigationMesh(navigationMesh);
				blueTanks.push_back(enemyEntity);
			}

			for (std::size_t i{}; i < which; ++i)
			{
				std::size_t nodeIndex;
				do
				{
					nodeIndex = std::rand() % nodes.size();
				} while (takenIndices.contains(nodeIndex) or
					(nodes[nodeIndex].first - nodes[redTankNodeIndex].first).getMagnitude() < 256.0);

				takenIndices.insert(nodeIndex);
				enemyEntity = &scene.addEntity(prefabs::recognizer(nodes[nodeIndex].first, player1Transform));
				enemyEntity->findComponent<Navigator>()->setNavigationMesh(navigationMesh);

				recognizers.push_back(enemyEntity);
			}

			return scene;
		}
	}
}