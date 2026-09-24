#pragma once

class Player3D;
class Enemy3D;
class Field;
class GameObject;
class ScoreObjectSpawner;





class GameObjectFactory
{
public:
    static Player3D* CreatePlayer();
    static Enemy3D* CreateEnemy(Player3D* player);
    static Field* CreateField();
    static ScoreObjectSpawner* CreateScoreObjectSpawner(Player3D* player);


    static GameObject* CreateBGM();
};
